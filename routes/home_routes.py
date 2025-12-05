from flask import Blueprint, render_template, session
from services.item_service import ItemService
from services.item_service import SearchService

home_bp = Blueprint('home', __name__)

@home_bp.route('/')
def homepage():
    """主页"""
    from models.item import Item
    from models.user import Favorite
    
    # 获取最新商品
    latest_items = Item.query.filter(
        Item.status == 'available'
    ).order_by(Item.created_at.desc()).limit(12).all()
    
    # 获取热门商品（按浏览量）
    popular_items = Item.query.filter(
        Item.status == 'available'
    ).order_by(Item.view_count.desc()).limit(6).all()
    
    # 获取用户收藏
    user_favorites = []
    if 'user_id' in session:
        user_favorites = [f.item_id for f in Favorite.query.filter_by(
            user_id=session['user_id']
        ).all()]
    
    # 获取分类
    categories = SearchService.get_categories()
    
    return render_template('homepage.html', 
                          latest_items=latest_items,
                          popular_items=popular_items,
                          user_favorites=user_favorites,
                          categories=categories)

@home_bp.route('/about')
def about():
    """关于页面"""
    return render_template('about.html')

@home_bp.route('/contact')
def contact():
    """联系页面"""
    return render_template('contact.html')