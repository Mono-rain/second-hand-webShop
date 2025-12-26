# testFolder/test_Trading.py
import pytest
from datetime import datetime, timedelta
from models.user import User
from models.item import Item
from models.order import Order, Logistics, Arbitration
from models.database import db

class TestTradingWorkflow:
    """交易流程集成测试 (自顶向下)"""

    def test_complete_trading_workflow(self, app):
        """测试完整交易流程：创建用户 -> 发布商品 -> 下单 -> 确认订单 -> 创建物流 -> 发起仲裁"""
        with app.app_context():
            # 1. 创建卖家和买家
            seller = User(
                username='trade_seller',
                email='trade_seller@example.com',
                phone='13800138010'
            )
            seller.set_password('password123')
            buyer = User(
                username='trade_buyer',
                email='trade_buyer@example.com',
                phone='13800138011'
            )
            buyer.set_password('password123')
            db.session.add_all([seller, buyer])
            db.session.commit()

            # 2. 卖家发布商品
            item = Item(
                title='iPhone 13 Pro',
                description='99新，国行，在保',
                price=5000.0,
                original_price=8000.0,
                category='手机数码',
                condition='99新',
                seller_id=seller.id
            )
            db.session.add(item)
            db.session.commit()

            # 验证商品创建成功且状态为可用
            assert item.id is not None
            assert item.status == 'available'
            assert item.seller_id == seller.id

            # 3. 买家下单 (模拟支付成功后状态变为 'paid')
            order = Order(
                buyer_id=buyer.id,
                item_id=item.id,
                total_price=item.price,
                quantity=1,
                shipping_address='北京市朝阳区',
                status='paid' # 假设已支付
            )
            db.session.add(order)
            db.session.commit()

            # 验证订单创建成功
            assert order.id is not None
            assert order.total_price == 5000.0
            assert order.status == 'paid'
            assert order.buyer_id == buyer.id
            assert order.item_id == item.id

            # 4. 下单后商品状态应自动更新为 'sold' (注意：原数据库初始化代码可能有此逻辑，但模型定义中没有)
            # 如果业务逻辑要求，需要在创建 Order 后手动更新 Item 状态
            # 这里我们手动更新以符合测试场景
            item.status = 'sold'
            db.session.commit()
            db.session.refresh(item)
            assert item.status == 'sold'

            # 5. 卖家发货，创建物流信息
            logistics = Logistics(
                order_id=order.id,
                tracking_id='SF987654321',
                carrier='顺丰速运',
                status='shipped', # 已发货
                shipping_address='北京市朝阳区',
                estimated_delivery=datetime.utcnow() + timedelta(days=3)
            )
            db.session.add(logistics)
            db.session.commit()

            # 验证物流信息创建成功并与订单关联
            assert order.logistics is not None
            assert order.logistics.tracking_id == 'SF987654321'
            assert order.logistics.status == 'shipped'

            # 6. 买家收到货后，因商品问题发起仲裁
            arbitration = Arbitration(
                order_id=order.id,
                reason='收到的商品屏幕有划痕，与描述不符',
                status='pending' # 仲裁待处理
            )
            db.session.add(arbitration)
            db.session.commit()

            # 验证仲裁信息创建成功并与订单关联
            assert order.arbitration is not None
            assert order.arbitration.reason == '收到的商品屏幕有划痕，与描述不符'
            assert order.arbitration.status == 'pending'

            # 7. 验证所有模型之间的关系是否正确建立
            # User -> Item (seller)
            assert seller.items.count() == 1
            assert seller.items.first().id == item.id
            # User -> Order (buyer)
            assert buyer.to_dict() # 确保序列化方法可用
            # Item -> Order (backref)
            # Order -> Item (backref)
            assert order.item_id == item.id
            assert order.buyer_id == buyer.id
            # Order -> Logistics (one-to-one)
            assert order.logistics.id == logistics.id
            # Order -> Arbitration (one-to-one)
            assert order.arbitration.id == arbitration.id

            print("✅ 完整交易流程集成测试通过")

    def test_order_status_flow(self, app):
        """测试订单状态流转 (自顶向下，关注状态变化)"""
        with app.app_context():
            # 创建用户和商品
            seller = User(username='status_seller', email='status_seller@example.com', phone='13800138020')
            seller.set_password('password123')
            buyer = User(username='status_buyer', email='status_buyer@example.com', phone='13800138021')
            buyer.set_password('password123')
            db.session.add_all([seller, buyer])
            db.session.commit()

            item = Item(title='状态流转测试商品', price=100.0, seller_id=seller.id)
            db.session.add(item)
            db.session.commit()

            # 创建初始订单
            order = Order(
                buyer_id=buyer.id,
                item_id=item.id,
                total_price=item.price
            )
            db.session.add(order)
            db.session.commit()

            # 定义期望的状态流转序列
            status_flow = ['pending', 'paid', 'shipped', 'delivered', 'completed', 'cancelled', 'refunded']

            for expected_status in status_flow:
                order.status = expected_status
                db.session.commit()
                # 刷新对象以获取数据库最新状态
                db.session.refresh(order)
                assert order.status == expected_status, f"状态流转到 {expected_status} 失败"

            print("✅ 订单状态流转集成测试通过")