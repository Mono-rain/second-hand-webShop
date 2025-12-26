# testFolder/test_UserFavorites.py
import pytest
from models.user import User, Favorite
from models.item import Item, ItemImage
from models.database import db

class TestUserFavorites:
    """用户收藏功能集成测试 (自底向上)"""

    def test_favorite_workflow(self, app):
        """测试收藏功能的核心流程：创建用户、商品 -> 收藏 -> 验证关系 -> 价格快照 -> 取消收藏"""
        with app.app_context():
            # 1. 创建用户
            user1 = User(
                username='fav_test_user1',
                email='fav_user1@test.com',
                phone='13800138001'
            )
            user1.set_password('password')
            user2 = User(
                username='fav_test_user2',
                email='fav_user2@test.com',
                phone='13800138002'
            )
            user2.set_password('password')
            db.session.add_all([user1, user2])
            db.session.commit()

            # 2. 创建商品
            item1 = Item(
                title='索尼相机',
                description='专业级相机',
                price=3000.0,
                seller_id=user1.id
            )
            item2 = Item(
                title='佳能镜头',
                description='长焦镜头',
                price=2000.0,
                seller_id=user1.id
            )
            db.session.add_all([item1, item2])
            db.session.commit()

            # 3. 添加商品图片
            image1 = ItemImage(
                item_id=item1.id,
                image_url='/static/camera1.jpg',
                is_primary=True
            )
            image2 = ItemImage(
                item_id=item1.id,
                image_url='/static/camera2.jpg',
                is_primary=False
            )
            db.session.add_all([image1, image2])
            db.session.commit()

            # 4. 测试收藏
            # 用户2收藏两个商品
            favorite1 = Favorite(
                user_id=user2.id,
                item_id=item1.id,
                price_snapshot=item1.price # 记录当前价格
            )
            favorite2 = Favorite(
                user_id=user2.id,
                item_id=item2.id,
                price_snapshot=item2.price
            )
            db.session.add_all([favorite1, favorite2])
            db.session.commit()

            # 验证收藏关系是否正确建立
            assert user2.favorites.count() == 2
            assert item1.favorites.count() == 1
            assert favorite1.item_id == item1.id
            assert favorite1.user_id == user2.id

            # 5. 测试价格快照功能
            original_price_item1 = item1.price
            item1.price = 2800.0 # 卖家修改价格
            db.session.commit()

            # 重新加载 favorite1 以获取数据库最新状态
            db.session.refresh(favorite1)
            # 验证收藏中的价格快照保持不变
            assert favorite1.price_snapshot == original_price_item1
            assert favorite1.price_snapshot != item1.price # 当前价格已变

            # 6. 测试取消收藏
            db.session.delete(favorite1)
            db.session.commit()
            # 验证收藏数量减少
            assert user2.favorites.count() == 1
            # 验证 item1 的收藏数也减少
            db.session.refresh(item1)
            assert item1.favorites.count() == 0

            # 7. 测试商品图片关系和方法
            primary_image = item1.get_primary_image()
            assert primary_image is not None
            assert primary_image.image_url == '/static/camera1.jpg'

            display_url = item1.get_display_image_url()
            assert display_url == '/static/camera1.jpg' # 主图存在

            # 8. 测试商品序列化 (注意：原 Item.to_dict 方法不包含 favorite_count)
            # 如果需要 favorite_count，需要在 Item 模型中添加
            # 这里只测试现有方法
            item_dict = item1.to_dict()
            assert 'title' in item_dict
            assert 'price' in item_dict
            assert 'images' in item_dict
            assert len(item_dict['images']) == 2 # 包含两张图片

            print("✅ 收藏功能集成测试通过")

    def test_item_view_counter(self, app):
        """测试商品浏览计数功能 (自底向上)"""
        with app.app_context():
            # 创建用户和商品
            user = User(username='view_test_user', email='view@test.com')
            user.set_password('password')
            db.session.add(user)
            db.session.commit()

            item = Item(
                title='测试浏览计数商品',
                description='用于测试浏览次数',
                price=100.0,
                seller_id=user.id
            )
            db.session.add(item)
            db.session.commit()

            # 初始浏览数应为0
            assert item.view_count == 0

            # 模拟多次浏览
            item.increment_views()
            item.increment_views()
            item.increment_views()
            db.session.commit()
            # 刷新对象以获取数据库最新状态
            db.session.refresh(item)
            assert item.view_count == 3

            print("✅ 浏览计数集成测试通过")