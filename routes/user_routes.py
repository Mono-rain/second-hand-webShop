from flask import Blueprint, render_template, session, redirect, url_for, flash, request, jsonify
from sqlalchemy.orm import joinedload  # 正确的导入名称
from utils.helpers import login_required
from services.item_service import ItemService
from models.order import Order
from models.user import User, Favorite
from models.item import Item  # 导入Item模型

user_bp = Blueprint('user', __name__)

def login_required(f):
    """自定义登录装饰器"""
    from functools import wraps
    
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if 'user_id' not in session:
            flash('请先登录', 'error')
            return redirect(url_for('auth.login'))
        return f(*args, **kwargs)
    return decorated_function

def get_current_user():
    """获取当前用户"""
    if 'user_id' in session:
        return User.query.get(session['user_id'])
    return None

@user_bp.route('/profile')
@login_required
def profile():
    """用户资料页面"""
    user = User.query.get(session['user_id'])
    user_items = ItemService.get_user_items(session['user_id'])
    user_orders = Order.query.filter_by(buyer_id=session['user_id']).order_by(
        Order.created_at.desc()
    ).all()
    
    # 获取收藏数量
    favorite_count = Favorite.query.filter_by(user_id=session['user_id']).count()
    
    return render_template('profile.html', 
                          user=user, 
                          items=user_items, 
                          orders=user_orders,
                          favorite_count=favorite_count)

@user_bp.route('/favorites')
@login_required
def favorites():
    try:
        # 简化查询，避免复杂的关联加载问题
        favorites = (Favorite.query
                    .filter_by(user_id=session['user_id'])
                    .order_by(Favorite.created_at.desc())
                    .all())
        
        # 准备收藏项数据
        favorite_items = []
        price_drops = 0
        
        for fav in favorites:
            if fav.item:
                # 检查是否有降价
                has_price_drop = fav.price_snapshot and fav.item.price < fav.price_snapshot
                if has_price_drop:
                    price_drops += 1
                
                favorite_items.append({
                    'favorite': fav,
                    'item': fav.item,
                    'has_price_drop': has_price_drop
                })
        
        return render_template('favorites.html', 
                             favorite_items=favorite_items,
                             price_drops=price_drops,
                             new_items=0)
    
    except Exception as e:
        print(f"收藏列表加载错误: {str(e)}")
        import traceback
        traceback.print_exc()
        flash('加载收藏列表时发生错误', 'error')
        return redirect(url_for('home.homepage'))
    

@user_bp.route('/favorites/<int:favorite_id>', methods=['DELETE'])
@login_required
def remove_favorite(favorite_id):
    from models.database import db
    """删除收藏"""
    try:
        favorite = Favorite.query.filter_by(
            id=favorite_id, 
            user_id=session['user_id']
        ).first()
        
        if favorite:
            db.session.delete(favorite)
            db.session.commit()
            return jsonify({'success': True, 'message': '取消收藏成功'})
        else:
            return jsonify({'success': False, 'message': '收藏记录不存在'})
    
    except Exception as e:
        db.session.rollback()
        return jsonify({'success': False, 'message': '取消收藏失败'})



@user_bp.route('/items')
@login_required
def my_items():
    """用户商品管理页面"""
    items = ItemService.get_user_items(session['user_id'], include_hidden=True)
    
    return render_template('my_items.html', items=items)

@user_bp.route('/orders')
@login_required
def my_orders():
    """用户订单页面"""
    orders = Order.query.filter_by(buyer_id=session['user_id']).order_by(
        Order.created_at.desc()
    ).all()
    
    return render_template('my_orders.html', orders=orders)

@user_bp.route('/settings', methods=['GET', 'POST'])
@login_required
def settings():
    """用户设置页面"""
    from models.user import User
    from models.database import db
    
    user = User.query.get(session['user_id'])
    
    if request.method == 'POST':
        # 更新用户信息
        user.phone = request.form.get('phone')
        # 可以添加更多字段
        
        try:
            db.session.commit()
            flash('设置已更新', 'success')
        except Exception as e:
            db.session.rollback()
            flash('更新失败', 'error')
    
    return render_template('settings.html', user=user)