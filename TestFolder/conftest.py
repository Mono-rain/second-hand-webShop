import pytest
import sys
import os
from flask import Flask
from datetime import datetime

# 添加项目根目录到 Python 路径
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

# 现在可以直接导入 models 模块
from models.database import db
from models.user import User, Admin
from models.item import Item, ItemImage

@pytest.fixture(scope='function')
def app():
    """创建测试用的 Flask 应用"""
    app = Flask(__name__)
    app.config['TESTING'] = True
    app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///:memory:'
    app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
    app.config['WTF_CSRF_ENABLED'] = False
    
    with app.app_context():
        db.init_app(app)
        db.create_all()
        
        # 创建测试数据
        create_test_data()
        
        yield app
        
        db.session.remove()
        db.drop_all()

@pytest.fixture(scope='function')
def client(app):
    """创建测试客户端"""
    return app.test_client()

@pytest.fixture(scope='function')
def session(app):
    """创建数据库会话"""
    with app.app_context():
        yield db.session

def create_test_data():
    """创建测试数据"""
    # 确保所有模型都已经正确注册
    from models.user import User
    from models.item import Item, ItemImage
    
    # 创建测试用户 - 使用 set_password 方法
    user1 = User(
        username='test_user1',
        email='test1@example.com',
        phone='13800138001',
        credit_score=85
    )
    user1.set_password('testpassword123')
    
    user2 = User(
        username='test_user2', 
        email='test2@example.com',
        phone='13800138002',
        credit_score=90
    )
    user2.set_password('testpassword123')
    
    # 先提交用户，获取ID
    db.session.add(user1)
    db.session.add(user2)
    db.session.commit()
    
    # 创建测试商品
    items = [
        Item(
            title='测试商品1',
            description='测试描述1',
            price=100.0,
            original_price=150.0,
            category='电子产品',
            condition='used',
            seller_id=user1.id  # 使用实际的用户ID
        ),
        Item(
            title='测试商品2',
            description='测试描述2',
            price=200.0,
            original_price=250.0,
            category='图书',
            condition='new',
            seller_id=user2.id  # 使用实际的用户ID
        )
    ]
    
    db.session.add_all(items)
    db.session.commit()
    
    # 创建商品图片
    images = [
        ItemImage(
            item_id=items[0].id,  # 使用实际的商品ID
            image_url='/static/images/test1.jpg',
            is_primary=True
        )
    ]
    
    db.session.add_all(images)
    db.session.commit()

def sample_user():
    """创建测试用的用户实例 - 正确版本"""
    from models.user import User
    from datetime import datetime
    
    user = User(
        username="test_user",
        email="test@example.com",
        phone="13800138000",
        credit_score=85
    )
    user.set_password('testpassword')
    user.created_at = datetime.utcnow()
    user.last_login = datetime.utcnow()
    
    return user

def sample_item():
    """创建测试用的商品实例 - 安全版本"""
    item = Item.__new__(Item)
    item.id = 999
    item.title = "测试商品"
    item.description = "测试描述"
    item.price = 100.0
    item.original_price = 150.0
    item.category = "电子产品"
    item.condition = "used"
    item.status = "available"
    item.seller_id = 1
    item.view_count = 0
    item.created_at = datetime.utcnow()
    item.updated_at = datetime.utcnow()
    return item