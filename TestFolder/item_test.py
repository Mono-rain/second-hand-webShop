import pytest
from datetime import datetime, timedelta
from conftest import sample_item


# 从 conftest 导入
from conftest import sample_item

# 直接从 models 导入
from models.item import Item, ItemImage

class TestItemModel:
    """Item 模型测试类"""
    
    def test_item_creation(self, session):
        """测试商品创建"""
        item = Item(
            title='新商品',
            description='新商品描述',
            price=300.0,
            original_price=400.0,
            category='家居',
            condition='new', 
            seller_id=1
        )
        
        session.add(item)
        session.commit()
        
        assert item.id is not None
        assert item.title == '新商品'
        assert item.price == 300.0
        assert item.status == 'available'  # 默认值
    
    def test_item_to_dict(self, session):
        """测试商品字典转换"""
        item = Item.query.get(1)
        item_dict = item.to_dict()
        
        expected_keys = [
            'id', 'title', 'description', 'price', 'original_price',
            'category', 'condition', 'status', 'seller_id', 'view_count',
            'created_at', 'images'
        ]
        
        assert all(key in item_dict for key in expected_keys)
        assert item_dict['title'] == '测试商品1'
        assert item_dict['price'] == 100.0
        assert isinstance(item_dict['images'], list)
    
    def test_view_count_increment(self, session):
        """测试浏览计数增加"""
        item = Item.query.get(1)
        original_count = item.view_count
        
        item.increment_views()
        
        assert item.view_count == original_count + 1
        
        # 再次增加
        item.increment_views()
        assert item.view_count == original_count + 2
    
    def test_item_relationships(self, session):
        """测试商品关系"""
        item = Item.query.get(1)
        
        # 测试图片关系
        assert item.images.count() == 1
        
        # 测试卖家关系
        assert item.seller.username == 'test_user1'
        
        # 测试收藏关系
        assert hasattr(item, 'favorites')
    
    def test_get_display_image_url(self, session):
        """测试获取展示图片URL"""
        item = Item.query.get(1)
        
        # 测试获取主图
        image_url = item.get_display_image_url()
        assert image_url == '/static/images/test1.jpg'
        
        # 测试没有图片的情况
        item_without_images = Item.query.get(2)
        image_url = item_without_images.get_display_image_url()
        assert image_url == '/static/images/placeholder.jpg'
    
    def test_get_primary_image(self, session):
        """测试获取主图"""
        item = Item.query.get(1)
        primary_image = item.get_primary_image()
        
        assert primary_image is not None
        assert primary_image.is_primary is True
        assert primary_image.image_url == '/static/images/test1.jpg'
    
    def test_has_images_method(self, session):
        """测试检查是否有图片的方法"""
        item_with_images = Item.query.get(1)
        item_without_images = Item.query.get(2)
        
        assert item_with_images.has_images() is True
        assert item_without_images.has_images() is False
    
    def test_item_status_validation(self, session):
        """测试商品状态验证"""
        item = Item.query.get(1)
        
        # 测试有效状态
        valid_statuses = ['available', 'sold', 'reserved', 'hidden']
        
        for status in valid_statuses:
            item.status = status
            session.commit()
            
            updated_item = Item.query.get(1)
            assert updated_item.status == status
    
    def test_item_condition_validation(self, session):
        """测试商品状况验证"""
        item = Item.query.get(1)
        
        # 测试有效状况
        valid_conditions = ['new', 'used', 'refurbished', 'like new']
        
        for condition in valid_conditions:
            item.condition = condition
            session.commit()
            
            updated_item = Item.query.get(1)
            assert updated_item.condition == condition
    
    def test_price_validation(self, session):
        """测试价格验证"""
        item = Item.query.get(1)
        
        # 测试正数价格
        item.price = 500.0
        item.original_price = 600.0
        session.commit()
        
        updated_item = Item.query.get(1)
        assert updated_item.price == 500.0
        assert updated_item.original_price == 600.0
    
    def test_automatic_timestamps(self, session):
        """测试自动时间戳"""
        item = Item.query.get(1)
        original_updated_at = item.updated_at
        
        # 修改商品信息
        item.title = '修改后的标题'
        session.commit()
        
        updated_item = Item.query.get(1)
        assert updated_item.updated_at != original_updated_at
        assert updated_item.updated_at > original_updated_at
    
    def test_item_search_indexing(self, session):
        """测试搜索索引字段"""
        # 测试带索引的字段查询
        items_by_category = Item.query.filter_by(category='电子产品').all()
        assert len(items_by_category) >= 1
        
        items_by_status = Item.query.filter_by(status='available').all()
        assert len(items_by_status) >= 2

class TestItemImageModel:
    """ItemImage 模型测试类"""
    
    def test_image_creation(self, session):
        """测试图片创建"""
        image = ItemImage(
            item_id=2,
            image_url='/static/images/new.jpg',
            is_primary=True
        )
        
        session.add(image)
        session.commit()
        
        assert image.id is not None
        assert image.item_id == 2
        assert image.is_primary is True
    
    def test_image_to_dict(self, session):
        """测试图片字典转换"""
        image = ItemImage.query.get(1)
        image_dict = image.to_dict()
        
        expected_keys = ['id', 'image_url', 'is_primary']
        assert all(key in image_dict for key in expected_keys)
        assert image_dict['image_url'] == '/static/images/test1.jpg'
        assert image_dict['is_primary'] is True
    
    def test_image_item_relationship(self, session):
        """测试图片与商品的关系"""
        image = ItemImage.query.get(1)
        
        assert image.item.id == 1
        assert image.item.title == '测试商品1'
    
    def test_primary_image_constraint(self, session):
        """测试主图约束（业务逻辑）"""
        item = Item.query.get(1)
        
        # 获取当前主图
        primary_images = item.images.filter_by(is_primary=True).all()
        assert len(primary_images) == 1
        
        # 尝试设置多个主图（业务逻辑上应该避免）
        second_image = ItemImage(
            item_id=1,
            image_url='/static/images/another_primary.jpg',
            is_primary=True
        )
        
        session.add(second_image)
        session.commit()
        
        # 现在有两个主图，这可能需要业务逻辑来处理
        primary_images = item.images.filter_by(is_primary=True).all()
        assert len(primary_images) == 2