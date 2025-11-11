from functools import wraps
from flask import session, redirect, url_for, flash, request  # 添加request导入

def login_required(f):
    """登录装饰器"""
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if 'user_id' not in session:
            flash('请先登录', 'error')
            return redirect(url_for('auth.login', next=request.url))
        return f(*args, **kwargs)
    return decorated_function

def admin_required(f):
    """管理员权限装饰器"""
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if 'user_id' not in session:
            flash('请先登录', 'error')
            return redirect(url_for('auth.login', next=request.url))
        
        # 检查是否为管理员（简化版）
        from models.user import Admin
        is_admin = Admin.query.filter_by(user_id=session['user_id']).first()
        if not is_admin:
            flash('需要管理员权限', 'error')
            return redirect(url_for('home.homepage'))
        
        return f(*args, **kwargs)
    return decorated_function

def get_current_user():
    """获取当前用户"""
    from models.user import User
    if 'user_id' in session:
        return User.query.get(session['user_id'])
    return None

def format_price(price):
    """格式化价格显示"""
    return f"¥{price:,.2f}"

def truncate_text(text, length=100):
    """截断文本"""
    if len(text) <= length:
        return text
    return text[:length] + '...'