from flask_sqlalchemy import SQLAlchemy
from werkzeug.security import generate_password_hash

db = SQLAlchemy()

def init_db(app):
    db.init_app(app)
    
    with app.app_context():
        # 导入所有模型以确保它们被注册
        # from . import item, user
        # from . import order
        db.create_all()
        
        from .user import User, Admin
        from models.item import Item
        # 检查是否已有数据，避免重复插入
        if not User.query.first():
            # 创建测试用户
            users = [
                User(
                    username='user1',
                    email='231220027@smail.com',
                    password_hash=generate_password_hash('123456'),
                    phone='13800138001',
                    credit_score=85
                ),
                User(
                    username='user2',
                    email='user2@example.com',
                    password_hash=generate_password_hash('password123'),
                    phone='13800138002',
                    credit_score=90
                ),
                User(
                    username='user3',
                    email='user3@example.com',
                    password_hash=generate_password_hash('password123'),
                    phone='13800138003',
                    credit_score=95
                )
            ]
            
            # 创建测试商品
            items = [
                Item(
                    title='二手iPhone 12',
                    description='九成新，无划痕，电池健康度90%',
                    price=2999.00,
                    original_price=3999.00,
                    category='手机',
                    condition='used',
                    seller=users[0]
                ),
                Item(
                    title='MacBook Pro 2020',
                    description='13寸，16GB内存，512GB SSD',
                    price=7999.00,
                    original_price=10999.00,
                    category='笔记本电脑',
                    condition='used',
                    seller=users[1]
                ),
                Item(
                    title='索尼WH-1000XM4耳机',
                    description='几乎全新，降噪效果优秀',
                    price=1499.00,
                    original_price=2499.00,
                    category='耳机',
                    condition='like new',
                    seller=users[2]
                )
            ]
            
            # 添加到会话并提交
            db.session.add_all(users)
            db.session.add_all(items)
            db.session.commit()

        