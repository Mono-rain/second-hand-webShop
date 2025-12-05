from flask import Blueprint, request, render_template, session, jsonify
from services.search_service import SearchEngine
from models.user import Favorite

search_bp = Blueprint('search', __name__)
search_engine = SearchEngine()

@search_bp.route('/')
def search():
    """搜索页面"""
    query = request.args.get('q', '')
    category = request.args.get('category', 'all')
    min_price = request.args.get('min_price', '')
    max_price = request.args.get('max_price', '')
    condition = request.args.get('condition', 'all')
    page = request.args.get('page', 1, type=int)
    
    # 获取自动补全建议
    suggestions = []
    if query:
        suggestions = search_engine.auto_complete(query)
    
    # 执行搜索
    filters = {
        'category': category if category != 'all' else None,
        'min_price': min_price if min_price else None,
        'max_price': max_price if max_price else None,
        'condition': condition if condition != 'all' else None
    }
    
    pagination = search_engine.search(query, filters, page=page)
    
    # 获取用户收藏
    user_favorites = []
    if 'user_id' in session:
        user_favorites = [f.item_id for f in Favorite.query.filter_by(
            user_id=session['user_id']
        ).all()]
    
    # 获取分类和热门搜索
    categories = search_engine.get_categories()
    popular_searches = search_engine.get_popular_searches()
    
    return render_template('search.html', 
                          pagination=pagination,
                          items=pagination.items,
                          query=query,
                          category=category,
                          min_price=min_price,
                          max_price=max_price,
                          condition=condition,
                          suggestions=suggestions,
                          user_favorites=user_favorites,
                          categories=categories,
                          popular_searches=popular_searches)

@search_bp.route('/autocomplete')
def autocomplete():
    """搜索自动补全API"""
    query = request.args.get('q', '')
    suggestions = search_engine.auto_complete(query)
    
    return jsonify({
        'query': query,
        'suggestions': suggestions
    })

@search_bp.route('/categories')
def categories():
    """分类页面"""
    from services.item_service import SearchService
    
    categories = SearchService.get_categories()
    category_items = {}
    
    for category in categories:
        items = SearchService.search_items(category=category, per_page=8)
        category_items[category] = items.items
    
    return render_template('categories.html',
                         categories=categories,
                         category_items=category_items)