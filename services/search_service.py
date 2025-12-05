from models.database import db  # 添加数据库导入
from sqlalchemy import or_


class LLM_API:
    """模拟LLM API服务"""
    
    @staticmethod
    def auto_complete(query):
        """智能搜索补全"""
        if not query or len(query) < 2:
            return []
        
        # 模拟智能补全逻辑
        suggestions_map = {
            "phone": ["智能手机", "iPhone", "安卓手机", "手机配件"],
            "book": ["教科书", "小说", "漫画", "技术书籍"],
            "computer": ["笔记本电脑", "台式机", "平板电脑", "电脑配件"],
            "clothes": ["T恤", "牛仔裤", "外套", "运动服"],
            "sports": ["篮球", "足球", "跑步鞋", "健身器材"]
        }
        
        # 查找相关建议
        suggestions = []
        query_lower = query.lower()
        
        for category, terms in suggestions_map.items():
            if any(term in query_lower for term in [category] + terms):
                suggestions.extend(terms)
        
        # 去重并限制数量
        unique_suggestions = list(dict.fromkeys(suggestions))[:5]
        
        return unique_suggestions if unique_suggestions else [f"'{query}' 相关商品"]

class SearchEngine:
    """搜索引擎"""
    
    def __init__(self):
        self.llm_api = LLM_API()
    
    def search(self, query, filters=None, page=1, per_page=12):
        """执行搜索"""
        from services.item_service import SearchService
        
        if filters is None:
            filters = {}
        
        return SearchService.search_items(
            query=query,
            category=filters.get('category'),
            min_price=filters.get('min_price'),
            max_price=filters.get('max_price'),
            condition=filters.get('condition'),
            page=page,
            per_page=per_page
        )
    
    def auto_complete(self, query):
        """获取搜索建议"""
        return self.llm_api.auto_complete(query)
    
    def get_popular_searches(self, limit=5):
        """获取热门搜索（模拟数据）"""
        return ["智能手机", "笔记本电脑", "运动鞋", "教科书", "耳机"]
    
    def get_categories(self):
        """获取商品分类"""
        from services.item_service import SearchService
        return SearchService.get_categories()
    
    def get_category_stats(self):
        """获取分类统计信息"""
        from services.item_service import SearchService
        categories = self.get_categories()
        stats = {}
        
        for category in categories:
            count = self.search("", filters={"category": category}).total
            stats[category] = count
            
        return stats

