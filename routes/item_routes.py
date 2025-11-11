from flask import Blueprint, request, render_template, redirect, url_for, session, flash, jsonify
from services.item_service import ItemService
from utils.helpers import login_required  # 修复导入路径

item_bp = Blueprint('item', __name__)

@item_bp.route('/item/<int:item_id>')
def item_detail(item_id):
    from models.item import Item
    from models.user import Favorite
    
    item = Item.query.get_or_404(item_id)
    
    # 增加浏览量
    item.increment_views()
    
    # 检查用户是否已收藏
    user_favorited = False
    if 'user_id' in session:
        favorite = Favorite.query.filter_by(
            user_id=session['user_id'], 
            item_id=item_id
        ).first()
        user_favorited = favorite is not None
    
    return render_template('item_detail.html', 
                          item=item, 
                          user_favorited=user_favorited)

@item_bp.route('/upload_item', methods=['GET', 'POST'])
@login_required
def upload_item():
    from services.item_service import SearchService
    
    if request.method == 'POST':
        title = request.form.get('title')
        description = request.form.get('description')
        price = request.form.get('price')
        original_price = request.form.get('original_price')
        category = request.form.get('category')
        new_category = request.form.get('new_category')  # 新增：自主输入的分类
        condition = request.form.get('condition', 'used')
        
        # 优先使用自主输入的分类，如果没有则使用选择框的分类
        final_category = new_category.strip() if new_category and new_category.strip() else category

        # 验证分类不能为空
        if not final_category:
            flash('请选择或输入商品分类', 'error')
            return render_template('upload_item.html', categories=categories)
        
        item, message = ItemService.create_item(
            title=title,
            description=description,
            price=price,
            original_price=original_price,
            category=final_category,
            condition=condition,
            seller_id=session['user_id']
        )
        
        if item:
            # 处理图片上传（简化版）
            image_url = request.form.get('image_url')
            if image_url:
                ItemService.add_item_image(item.id, image_url, is_primary=True)
            
            flash('商品发布成功！', 'success')
            return redirect(url_for('item.item_detail', item_id=item.id))
        else:
            flash(message, 'error')
    
    categories = SearchService.get_categories()
    return render_template('upload_item.html', categories=categories)

@item_bp.route('/api/toggle_favorite/<int:item_id>', methods=['POST'])
@login_required
def toggle_favorite(item_id):
    result, message = ItemService.toggle_favorite(session['user_id'], item_id)
    
    if result is not None:
        return jsonify({
            'success': True, 
            'favorited': result, 
            'message': message
        })
    else:
        return jsonify({
            'success': False, 
            'message': message
        }), 400

@item_bp.route('/api/items/<int:item_id>/status', methods=['PUT'])
@login_required
def update_item_status(item_id):
    from models.item import Item
    
    item = Item.query.get_or_404(item_id)
    
    # 检查权限
    if item.seller_id != session['user_id']:
        return jsonify({'success': False, 'message': '无权操作'}), 403
    
    status = request.json.get('status')
    if status not in ['available', 'sold', 'hidden']:
        return jsonify({'success': False, 'message': '状态无效'}), 400
    
    success, message = ItemService.update_item_status(item_id, status)
    
    if success:
        return jsonify({'success': True, 'message': message})
    else:
        return jsonify({'success': False, 'message': message}), 400