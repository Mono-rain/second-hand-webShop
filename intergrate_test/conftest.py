# testFolder/conftest.py
import pytest
import sys
import os
from datetime import datetime

# 添加项目根目录到Python路径
sys.path.insert(0, os.path.abspath(os.path.dirname(os.path.dirname(__file__))))

from app import create_app
from models.database import db

# 修正：从正确的位置导入模型
from models.user import User, Admin
from models.item import Item, ItemImage
from models.order import Order, Logistics, Arbitration
from werkzeug.security import generate_password_hash

@pytest.fixture(scope='session')
def app():
    """使用独立的内存数据库"""
    app = create_app()
    app.config.update({
        'TESTING': True,
        'SQLALCHEMY_DATABASE_URI': 'sqlite:///:memory:', # 使用内存数据库
        'WTF_CSRF_ENABLED': False,
        'SQLALCHEMY_TRACK_MODIFICATIONS': False
    })
    with app.app_context():
        db.create_all()
        yield app
        db.drop_all()

@pytest.fixture
def client(app):
    return app.test_client()

@pytest.fixture(autouse=True)
def clean_db(app):
    """每个测试后自动清理数据"""
    with app.app_context():
        yield
        # 清理所有表数据但保留表结构
        for table in reversed(db.metadata.sorted_tables):
            db.session.execute(table.delete())
        db.session.commit()

@pytest.fixture
def test_user_data(app):
    """创建测试用户数据"""
    with app.app_context():
        # 注意：这里创建的是普通用户和管理员用户，用于不同场景的测试
        test_user = User(
            username='test_buyer',
            email='buyer@example.com',
            phone='13800138001'
        )
        test_user.set_password('password123') # 使用模型方法设置密码

        admin_user = User(
            username='test_seller',
            email='seller@example.com',
            phone='13800138002'
        )
        admin_user.set_password('password123')

        db.session.add_all([test_user, admin_user])
        db.session.commit()

        # 为卖家创建管理员记录 (注意：这里的 Admin 模型关联的是 User.id)
        # 但通常 Admin 表是为管理后台设计的。这里我们只为测试卖家用户创建商品。
        # 如果需要测试 Admin 权限，可以单独创建 Admin 记录。
        # seller_admin = Admin(user_id=admin_user.id, permissions='{"sell": true}')
        # db.session.add(seller_admin)
        # db.session.commit()

        return {
            'buyer': test_user,
            'seller': admin_user # 重命名为 seller 以更清晰
        }

@pytest.fixture
def test_item_data(app, test_user_data):
    """创建测试商品数据"""
    with app.app_context():
        seller = test_user_data['seller']
        test_item = Item(
            title='Test Product',
            description='A test product for unit testing',
            price=100.0,
            original_price=120.0,
            category='Electronics',
            condition='new',
            seller_id=seller.id
        )
        db.session.add(test_item)
        db.session.commit()
        return test_item

@pytest.fixture
def test_order_data(app, test_user_data, test_item_data):
    """创建测试订单数据"""
    with app.app_context():
        buyer = test_user_data['buyer']
        item = test_item_data
        test_order = Order(
            buyer_id=buyer.id,
            item_id=item.id,
            total_price=item.price,
            quantity=1,
            shipping_address='Test Buyer, 123 Test St, Test City',
            status='pending'
        )
        db.session.add(test_order)
        db.session.commit()
        return test_order