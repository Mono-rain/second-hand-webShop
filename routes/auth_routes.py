from flask import Blueprint, request, render_template, redirect, url_for, session, flash, jsonify
from services.auth_service import AuthService

auth_bp = Blueprint('auth', __name__)

@auth_bp.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        email = request.form.get('email')
        password = request.form.get('password')
        
        user, message = AuthService.login_user(email, password)
        
        if user:
            session['user_id'] = user.id
            session['username'] = user.username
            flash('登录成功！', 'success')
            
            next_page = request.args.get('next')
            return redirect(next_page or url_for('home.homepage'))
        else:
            flash(message, 'error')
    
    return render_template('login.html')

@auth_bp.route('/register', methods=['GET', 'POST'])
def register():
    if request.method == 'POST':
        username = request.form.get('username')
        email = request.form.get('email')
        password = request.form.get('password')
        phone = request.form.get('phone')
        
        user, message = AuthService.register_user(username, email, password, phone)
        
        if user:
            session['user_id'] = user.id
            session['username'] = user.username
            flash('注册成功！', 'success')
            return redirect(url_for('home.homepage'))
        else:
            flash(message, 'error')
    
    return render_template('register.html')

@auth_bp.route('/logout')
def logout():
    session.clear()
    flash('您已成功退出登录', 'info')
    return redirect(url_for('home.homepage'))

@auth_bp.route('/api/check_email')
def check_email():
    """检查邮箱是否已注册（AJAX接口）"""
    from models.user import User
    email = request.args.get('email')
    
    if not email:
        return jsonify({'available': False})
    
    user = User.query.filter_by(email=email).first()
    return jsonify({'available': user is None})

@auth_bp.route('/api/check_username')
def check_username():
    """检查用户名是否可用（AJAX接口）"""
    from models.user import User
    username = request.args.get('username')
    
    if not username:
        return jsonify({'available': False})
    
    user = User.query.filter_by(username=username).first()
    return jsonify({'available': user is None})