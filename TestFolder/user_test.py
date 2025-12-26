import pytest
from datetime import datetime, timedelta
from werkzeug.security import check_password_hash

# 从 conftest 导入 fixture 和测试数据
from conftest import sample_user

# 直接从 models 导入
from models.user import User, Admin

class TestUserModel:
    """User 模型测试类"""
    
    def test_user_creation(self, session):
        """测试用户创建"""
        user = User(
            username='new_user',
            email='new@example.com',
            phone='13800138003'
        )
        user.set_password('newpassword123')
        
        session.add(user)
        session.commit()
        
        assert user.id is not None
        assert user.username == 'new_user'
        assert user.email == 'new@example.com'
        assert user.credit_score == 100  # 默认值
    
    def test_password_hashing(self, session):
        """测试密码哈希"""
        user = User.query.get(1)
        
        # 测试正确密码
        assert user.check_password('testpassword123') is True
        
        # 测试错误密码
        assert user.check_password('wrongpassword') is False
    
    def test_password_hashing_consistency(self):
        """测试密码哈希一致性 - 不依赖数据库"""
        # 创建临时用户测试密码功能
        user = User(
            username="temp_user",
            email="temp@example.com", 
            phone="13800138000"
        )
        user.set_password('testpassword')
        
        # 测试密码功能（不需要保存到数据库）
        assert user.check_password('testpassword') is True
        assert user.check_password('wrongpassword') is False
    
    def test_user_to_dict(self, session):
        """测试用户字典转换"""
        user = User.query.get(1)
        user_dict = user.to_dict()
        
        expected_keys = ['id', 'username', 'email', 'phone', 'credit_score', 'created_at']
        assert all(key in user_dict for key in expected_keys)
        assert user_dict['username'] == 'test_user1'
        assert user_dict['credit_score'] == 85
    
    def test_user_relationships(self, session):
        """测试用户关系"""
        user = User.query.get(1)
        
        # 测试商品关系
        assert user.items.count() >= 1
        assert user.items.first().title == '测试商品1'
        
        # 测试收藏关系
        assert hasattr(user, 'favorites')
    
    def test_unique_constraints(self, session):
        """测试唯一约束"""
        # 测试用户名唯一性
        user1 = User(
            username='test_user1',  # 重复用户名
            email='unique@example.com',
            phone='13800138004'
        )
        user1.set_password('password123')
        
        session.add(user1)
        
        with pytest.raises(Exception):
            session.commit()
        
        session.rollback()
    
    def test_email_unique_constraint(self, session):
        """测试邮箱唯一性约束"""
        user = User(
            username='unique_user',
            email='test1@example.com',  # 重复邮箱
            phone='13800138005'
        )
        user.set_password('password123')
        
        session.add(user)
        
        with pytest.raises(Exception):
            session.commit()
        
        session.rollback()
    
    def test_credit_score_validation(self, session):
        """测试信用分数验证"""
        user = User.query.get(1)
        
        # 测试有效信用分数
        user.credit_score = 95
        session.commit()
        
        updated_user = User.query.get(1)
        assert updated_user.credit_score == 95
    
    def test_last_login_update(self, session):
        """测试最后登录时间更新"""
        user = User.query.get(1)
        original_login_time = user.last_login
        
        user.update_login_time()
        session.commit()
        
        updated_user = User.query.get(1)
        assert updated_user.last_login is not None
        assert updated_user.last_login != original_login_time
    
    def test_phone_number_format(self, session):
        """测试电话号码格式"""
        user = User.query.get(1)
        
        # 测试有效电话号码
        user.phone = '13800138000'
        session.commit()
        
        updated_user = User.query.get(1)
        assert updated_user.phone == '13800138000'
    
    def test_user_string_representation(self, session):
        """测试用户字符串表示"""
        user = User.query.get(1)
        
        # 虽然没有定义 __repr__，但可以测试基本属性
        assert str(user.id) in str(user)
        assert isinstance(user, User)
    
    def test_admin_relationship(self, session):
        """测试管理员关系"""
        user = User.query.get(1)
        admin = Admin(user_id=user.id)
        
        session.add(admin)
        session.commit()
        
        # 测试反向关系
        assert hasattr(user, 'admin')
        assert user.admin is not None
        assert user.admin.user_id == user.id

class TestAdminModel:
    """Admin 模型测试类"""
    
    def test_admin_creation(self, session):
        """测试管理员创建"""
        user = User.query.get(2)
        admin = Admin(user_id=user.id, permissions='full')
        
        session.add(admin)
        session.commit()
        
        assert admin.id is not None
        assert admin.user_id == user.id
        assert admin.permissions == 'full'
    
    def test_admin_user_relationship(self, session):
        """测试管理员与用户的关系"""
        user = User.query.get(1)
        admin = Admin(user_id=user.id)
        
        session.add(admin)
        session.commit()
        
        # 测试从用户访问管理员
        assert user.admin is not None
        assert user.admin.user_id == user.id
        
        # 测试从管理员访问用户
        assert admin.user.id == user.id
        assert admin.user.username == user.username