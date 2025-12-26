from .database import db
from datetime import datetime


class Item(db.Model):
    __tablename__ = 'items'
    
    id = db.Column(db.Integer, primary_key=True)
    title = db.Column(db.String(200), nullable=False, index=True)
    description = db.Column(db.Text)
    price = db.Column(db.Float, nullable=False)
    original_price = db.Column(db.Float)  # 原价（用于显示折扣）
    category = db.Column(db.String(100), index=True)
    condition = db.Column(db.String(50), default='used')  # new, used, refurbished
    status = db.Column(db.String(20), default='available', index=True)  # available, sold, reserved, hidden
    seller_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False, index=True)
    view_count = db.Column(db.Integer, default=0)
    created_at = db.Column(db.DateTime, default=datetime.utcnow, index=True)
    updated_at = db.Column(db.DateTime, default=datetime.utcnow, onupdate=datetime.utcnow)
    
    # 关系
    images = db.relationship('ItemImage', backref='item', lazy='dynamic', 
                            cascade='all, delete-orphan')
    # orders = db.relationship('Order', backref='item', lazy='dynamic')
    favorites = db.relationship('Favorite', backref='item', lazy='dynamic')
    
    def increment_views(self):
        self.view_count += 1
        db.session.commit()
    
    def to_dict(self):
        return {
            'id': self.id,
            'title': self.title,
            'description': self.description,
            'price': self.price,
            'original_price': self.original_price,
            'category': self.category,
            'condition': self.condition,
            'status': self.status,
            'seller_id': self.seller_id,
            'view_count': self.view_count,
            'created_at': self.created_at.isoformat() if self.created_at else None,
            'images': [img.to_dict() for img in self.images.all()]
        }
    
     # 添加获取主图的方法
    def get_primary_image(self):
        return self.images.filter_by(is_primary=True).first()
    
    def get_display_image_url(self):
        """获取展示图片的URL，用于模板显示"""
        # 先尝试获取主图
        primary_image = self.images.filter_by(is_primary=True).first()
        if primary_image:
            return primary_image.image_url
        
        # 如果没有主图，获取第一张图片
        first_image = self.images.first()
        if first_image:
            return first_image.image_url
        
        # 如果没有任何图片，返回默认图片
        return '/static/images/placeholder.jpg'
    
    def get_first_image_url(self):
        """获取展示图片URL（兼容性方法）"""
        return self.get_display_image_url()
    
    def has_images(self):
        """检查商品是否有图片"""
        return self.images.first() is not None

class ItemImage(db.Model):
    __tablename__ = 'item_images'
    
    id = db.Column(db.Integer, primary_key=True)
    item_id = db.Column(db.Integer, db.ForeignKey('items.id'), nullable=False)
    image_url = db.Column(db.String(500), nullable=False)
    is_primary = db.Column(db.Boolean, default=False)
    created_at = db.Column(db.DateTime, default=datetime.utcnow)
    
    def to_dict(self):
        return {
            'id': self.id,
            'image_url': self.image_url,
            'is_primary': self.is_primary
        }