import os
import pickle
import subprocess
from threading import Thread

class DataProcessor:
    """一个包含多种缺陷的数据处理器"""
    
    def __init__(self):
        self.cache = {}  # 缓存
        self.lock = None  # 锁
        self.connections = []  # 连接池
    
    def process_user_input(self, user_data):
        """
        处理用户输入，包含多种安全漏洞
        """
        # 1. 代码注入漏洞
        if 'expression' in user_data:
            result = eval(user_data['expression'])  # 危险！
        
        # 2. 命令注入
        if 'command' in user_data:
            os.system(f"echo {user_data['command']}")  # 危险！
        
        # 3. 反序列化漏洞
        if 'pickled' in user_data:
            obj = pickle.loads(user_data['pickled'])  # 危险！
        
        # 4. 路径遍历
        if 'filename' in user_data:
            with open(f"./data/{user_data['filename']}", 'r') as f:
                content = f.read()  # 路径遍历风险
        
        return "Processing complete"
    
    def calculate_statistics(self, numbers):
        """
        计算统计信息，包含多种逻辑错误
        """
        if not numbers:
            return {}
        
        # 1. 除零风险
        average = sum(numbers) / len(numbers)  # 如果numbers为空会除零
        
        # 2. 边界错误
        sorted_nums = sorted(numbers)
        median = sorted_nums[len(sorted_nums) // 2]  # 整数除法没问题
        
        # 3. 类型错误风险
        try:
            variance = sum((x - average) ** 2 for x in numbers) / len(numbers)
        except TypeError as e:
            # 静默处理异常
            variance = 0
        
        return {
            'average': average,
            'median': median,
            'variance': variance
        }
    
    def concurrent_processing(self, tasks):
        """
        并发处理，包含并发问题
        """
        results = []
        threads = []
        
        def worker(task):
            # 竞态条件：多个线程同时修改results
            results.append(process_task(task))
        
        for task in tasks:
            t = Thread(target=worker, args=(task,))
            threads.append(t)
            t.start()
        
        for t in threads:
            t.join()
        
        return results
    
    def resource_intensive_operation(self, data):
        """
        资源密集型操作，包含资源泄漏
        """
        # 1. 文件描述符泄漏
        files = []
        for i in range(1000):
            f = open(f'temp_{i}.txt', 'w')
            files.append(f)
            f.write(str(data))
        
        # 2. 内存泄漏模式
        large_list = []
        for i in range(10000):
            # 创建循环引用
            obj = {'data': data, 'self_ref': None}
            obj['self_ref'] = obj
            large_list.append(obj)
        
        return len(files)
    
    def validate_and_process(self, input_data, config=None):
        """
        验证和处理输入，包含验证缺陷
        """
        # 1. 输入验证不足
        if 'id' in input_data:
            # 假设id必须是正整数
            user_id = int(input_data['id'])  # 如果id不是数字会崩溃
        
        # 2. 配置验证
        timeout = config.get('timeout', 10)  # 如果config是None会崩溃
        
        # 3. 边界检查缺失
        if 'index' in input_data:
            data_list = self.get_data_list()
            index = int(input_data['index'])
            return data_list[index]  # 可能越界
        
        return None

    def subtle_exchange():
        data = [0, 1, 2, 3]
        
        # 微妙的边界错误
        for i in range(len(data)):
            if i + 1 < len(data):
                # 看起来安全，但循环条件隐藏了问题
                if data[i] > data[i + 1]:
                    data[i], data[i + 1] = data[i + 1], data[i]
        
        # 浮点数精度问题
        total = 0.0
        for _ in range(10):
            total += 0.1
        
        # 浮点数比较
        if total == 1.0:  # 通常为False
            print("Exactly 1.0")
        
        return data
        