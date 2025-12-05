from .database import db
from datetime import datetime

class Order(db.Model):
    __tablename__ = 'orders'
    
    id = db.Column(db.Integer, primary_key=True)
    buyer_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False, index=True)
    item_id = db.Column(db.Integer, db.ForeignKey('items.id'), nullable=False, index=True)
    status = db.Column(db.String(20), default='pending', index=True)  # pending, paid, shipped, completed, cancelled, refunded
    total_price = db.Column(db.Float, nullable=False)
    quantity = db.Column(db.Integer, default=1)
    shipping_address = db.Column(db.Text)
    notes = db.Column(db.Text)  # 买家留言
    created_at = db.Column(db.DateTime, default=datetime.utcnow, index=True)
    updated_at = db.Column(db.DateTime, default=datetime.utcnow, onupdate=datetime.utcnow)
    paid_at = db.Column(db.DateTime)
    completed_at = db.Column(db.DateTime)
    
    # 关系
    logistics = db.relationship('Logistics', backref='order', uselist=False, 
                               cascade='all, delete-orphan')
    arbitration = db.relationship('Arbitration', backref='order', uselist=False,
                                 cascade='all, delete-orphan')
    
    def to_dict(self):
        return {
            'id': self.id,
            'buyer_id': self.buyer_id,
            'item_id': self.item_id,
            'status': self.status,
            'total_price': self.total_price,
            'quantity': self.quantity,
            'created_at': self.created_at.isoformat() if self.created_at else None,
            'item': self.item.to_dict() if self.item else None
        }

class Logistics(db.Model):
    __tablename__ = 'logistics'
    
    id = db.Column(db.Integer, primary_key=True)
    order_id = db.Column(db.Integer, db.ForeignKey('orders.id'), nullable=False, unique=True)
    tracking_id = db.Column(db.String(100), unique=True, index=True)
    carrier = db.Column(db.String(100))  # 快递公司
    status = db.Column(db.String(50), default='preparing')  # preparing, shipped, in_transit, delivered
    shipping_address = db.Column(db.Text)
    estimated_delivery = db.Column(db.DateTime)
    created_at = db.Column(db.DateTime, default=datetime.utcnow)
    updated_at = db.Column(db.DateTime, default=datetime.utcnow, onupdate=datetime.utcnow)

class Arbitration(db.Model):
    __tablename__ = 'arbitrations'
    
    id = db.Column(db.Integer, primary_key=True)
    order_id = db.Column(db.Integer, db.ForeignKey('orders.id'), nullable=False, unique=True)
    reason = db.Column(db.Text, nullable=False)
    status = db.Column(db.String(20), default='pending', index=True)  # pending, investigating, resolved
    resolution = db.Column(db.Text)  # 仲裁结果
    admin_id = db.Column(db.Integer, db.ForeignKey('users.id'))  # 处理仲裁的管理员
    created_at = db.Column(db.DateTime, default=datetime.utcnow)
    resolved_at = db.Column(db.DateTime)