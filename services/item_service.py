from models.item import Item, ItemImage
from models.user import Favorite
from models.database import db
from sqlalchemy import or_, and_

class ItemService:
    @staticmethod
    def create_item(title, description, price, category, seller_id, condition='used', original_price=None):
        """创建新商品"""
        item = Item(
            title=title,
            description=description,
            price=float(price),
            original_price=float(original_price) if original_price else None,
            category=category,
            condition=condition,
            seller_id=seller_id
        )
        
        try:
            db.session.add(item)
            db.session.commit()
            return item, "商品发布成功"
        except Exception as e:
            db.session.rollback()
            return None, f"发布失败: {str(e)}"
    
    @staticmethod
    def add_item_image(item_id, image_url, is_primary=False):
        """添加商品图片"""
        image = ItemImage(
            item_id=item_id,
            image_url=image_url,
            is_primary=is_primary
        )
        
        try:
            db.session.add(image)
            db.session.commit()
            return image, "图片添加成功"
        except Exception as e:
            db.session.rollback()
            return None, f"添加失败: {str(e)}"
    
    @staticmethod
    def toggle_favorite(user_id, item_id):
        """切换收藏状态"""
        favorite = Favorite.query.filter_by(user_id=user_id, item_id=item_id).first()
        
        if favorite:
            db.session.delete(favorite)
            db.session.commit()
            return False, "已取消收藏"
        else:
            # 获取当前价格作为快照
            item = Item.query.get(item_id)
            if not item:
                return None, "商品不存在"
            
            favorite = Favorite(
                user_id=user_id,
                item_id=item_id,
                price_snapshot=item.price
            )
            
            try:
                db.session.add(favorite)
                db.session.commit()
                return True, "已添加收藏"
            except Exception as e:
                db.session.rollback()
                return None, f"操作失败: {str(e)}"
    
    @staticmethod
    def get_user_favorites(user_id):
        """获取用户收藏列表"""
        return Favorite.query.filter_by(user_id=user_id).order_by(Favorite.created_at.desc()).all()
    
    @staticmethod
    def get_user_items(user_id, include_hidden=False):
        """获取用户发布的商品"""
        query = Item.query.filter_by(seller_id=user_id)
        if not include_hidden:
            query = query.filter(Item.status != 'hidden')
        return query.order_by(Item.created_at.desc()).all()
    
    @staticmethod
    def update_item_status(item_id, status):
        """更新商品状态"""
        item = Item.query.get(item_id)
        if not item:
            return False, "商品不存在"
        
        item.status = status
        db.session.commit()
        return True, "状态更新成功"

class SearchService:
    @staticmethod
    def search_items(query=None, category=None, min_price=None, max_price=None, 
                    condition=None, page=1, per_page=12):
        """搜索商品"""
        from models.item import Item
        
        # 基础查询 - 只显示可用的商品
        results = Item.query.filter(Item.status == 'available')
        
        # 关键词搜索（标题和描述）
        if query:
            results = results.filter(
                or_(
                    Item.title.ilike(f'%{query}%'),
                    Item.description.ilike(f'%{query}%')
                )
            )
        
        # 分类筛选
        if category and category != 'all':
            results = results.filter(Item.category == category)
        
        # 价格范围筛选
        if min_price:
            results = results.filter(Item.price >= float(min_price))
        if max_price:
            results = results.filter(Item.price <= float(max_price))
        
        # 商品状况筛选
        if condition and condition != 'all':
            results = results.filter(Item.condition == condition)
        
        # 排序和分页
        results = results.order_by(Item.created_at.desc())
        pagination = results.paginate(page=page, per_page=per_page, error_out=False)
        
        return pagination
    
    @staticmethod
    def get_categories():
        """获取所有分类"""
        from models.item import Item
        from sqlalchemy import distinct
        categories = db.session.query(distinct(Item.category)).filter(Item.category.isnot(None)).all()
        return [cat[0] for cat in categories if cat[0]]