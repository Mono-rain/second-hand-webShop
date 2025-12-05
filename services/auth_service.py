from models.user import User, Admin
from models.database import db
from datetime import datetime
import re

class AuthService:
    @staticmethod
    def register_user(username, email, password, phone=None):
        """注册新用户"""
        # 验证输入
        if not AuthService._validate_username(username):
            return None, "用户名格式无效（3-20位字母数字）"
        
        if not AuthService._validate_email(email):
            return None, "邮箱格式无效"
        
        if not AuthService._validate_password(password):
            return None, "密码必须至少6位"
        
        # 检查是否已存在
        if User.query.filter_by(username=username).first():
            return None, "用户名已存在"
        
        if User.query.filter_by(email=email).first():
            return None, "邮箱已注册"
        
        # 创建用户
        user = User(
            username=username,
            email=email,
            phone=phone
        )
        user.set_password(password)
        
        try:
            db.session.add(user)
            db.session.commit()
            return user, "注册成功"
        except Exception as e:
            db.session.rollback()
            return None, f"注册失败: {str(e)}"
    
    @staticmethod
    def login_user(email, password):
        """用户登录"""
        user = User.query.filter_by(email=email).first()
        
        if not user or not user.check_password(password):
            return None, "邮箱或密码错误"
        
        user.update_login_time()
        db.session.commit()
        
        return user, "登录成功"
    
    @staticmethod
    def make_admin(user_id):
        """将用户设为管理员"""
        if Admin.query.filter_by(user_id=user_id).first():
            return False, "用户已是管理员"
        
        admin = Admin(user_id=user_id)
        
        try:
            db.session.add(admin)
            db.session.commit()
            return True, "成功设为管理员"
        except Exception as e:
            db.session.rollback()
            return False, f"操作失败: {str(e)}"
    
    @staticmethod
    def _validate_username(username):
        return re.match(r'^[a-zA-Z0-9_]{3,20}$', username) is not None
    
    @staticmethod
    def _validate_email(email):
        return re.match(r'^[^@]+@[^@]+\.[^@]+$', email) is not None
    
    @staticmethod
    def _validate_password(password):
        return len(password) >= 6