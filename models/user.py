from .database import db
from datetime import datetime
from werkzeug.security import generate_password_hash, check_password_hash

class User(db.Model):
    __tablename__ = 'users'
    
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(80), unique=True, nullable=False, index=True)
    email = db.Column(db.String(120), unique=True, nullable=False, index=True)
    password_hash = db.Column(db.String(200), nullable=False)
    phone = db.Column(db.String(20))
    credit_score = db.Column(db.Integer, default=100)
    created_at = db.Column(db.DateTime, default=datetime.utcnow)
    last_login = db.Column(db.DateTime)
    
    # 关系
    items = db.relationship('Item', backref='seller', lazy='dynamic', 
                           cascade='all, delete-orphan')
    # orders = db.relationship('Order', backref='buyer', lazy='dynamic',
    #                         foreign_keys='Order.buyer_id')
    favorites = db.relationship('Favorite', backref='user', lazy='dynamic',
                               cascade='all, delete-orphan')
    
    def set_password(self, password):
        self.password_hash = generate_password_hash(password)
    
    def check_password(self, password):
        return check_password_hash(self.password_hash, password)
    
    def update_login_time(self):
        self.last_login = datetime.utcnow()
    
    def to_dict(self):
        return {
            'id': self.id,
            'username': self.username,
            'email': self.email,
            'phone': self.phone,
            'credit_score': self.credit_score,
            'created_at': self.created_at.isoformat() if self.created_at else None
        }

class Admin(db.Model):
    __tablename__ = 'admins'
    
    id = db.Column(db.Integer, primary_key=True)
    user_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False, unique=True)
    user = db.relationship('User', backref=db.backref('admin', uselist=False))
    permissions = db.Column(db.String(500), default='basic')  # JSON string of permissions
    created_at = db.Column(db.DateTime, default=datetime.utcnow)

class Favorite(db.Model):
    __tablename__ = 'favorites'
    
    id = db.Column(db.Integer, primary_key=True)
    user_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False)
    item_id = db.Column(db.Integer, db.ForeignKey('items.id'), nullable=False)
    price_snapshot = db.Column(db.Float)
    created_at = db.Column(db.DateTime, default=datetime.utcnow)
    
    
    # 唯一约束：一个用户不能收藏同一个商品多次
    __table_args__ = (db.UniqueConstraint('user_id', 'item_id', name='unique_user_favorite'),)