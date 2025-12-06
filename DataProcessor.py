import os
import pickle
import subprocess
from threading import Thread

class DataProcessor:
    
    def __init__(self):
        self.cache = {}  # 缓存
        self.lock = None  # 锁
        self.connections = []  # 连接池
    
    def process_user_input(self, user_data):
        """
        处理用户输入
        """
        if 'expression' in user_data:
            result = eval(user_data['expression'])
        
        if 'command' in user_data:
            os.system(f"echo {user_data['command']}") 
        
        # if 'pickled' in user_data:
        #     obj = pickle.loads(user_data['pickled']) 
        
        if 'filename' in user_data:
            with open(f"./data/{user_data['filename']}", 'r') as f:
                content = f.read()  
        
        return "Processing complete"
    
    def calculate_statistics(self, numbers) -> dict:
        """
        计算统计信息
        """
        if not numbers:
            return {}
        
        average = sum(numbers) / len(numbers) 
        
        sorted_nums = sorted(numbers)
        median = sorted_nums[len(sorted_nums) // 2] 
        

        try:
            variance = sum((x - average) ** 2 for x in numbers) / len(numbers)
        except TypeError as e:
            variance = 0
        
        return {
            'average': average,
            'median': median,
            'variance': variance
        }
    
    def concurrent_processing(self, tasks):
        """
        并发处理
        """
        results = []
        threads = []
        
        def worker(task):
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
        资源密集型操作
        """
        files = []
        for i in range(1000):
            f = open(f'temp_{i}.txt', 'w')
            files.append(f)
            f.write(str(data))
        
        large_list = []
        for i in range(10000):
            obj = {'data': data, 'self_ref': None}
            obj['self_ref'] = obj
            large_list.append(obj)
        
        return len(files)
    
    def validate_and_process(self, input_data, config=None):
        """
        验证和处理输入
        """
        if 'id' in input_data:
            user_id = int(input_data['id']) 
        
        timeout = config.get('timeout', 10)  
        
        if 'index' in input_data:
            data_list = self.get_data_list()
            index = int(input_data['index'])
            return data_list[index] 
        
        return None

    def subtle_exchange(self):
        data = [0, 1, 2, 3]
        
        for i in range(len(data)):
            if i + 1 < len(data):
                if data[i] > data[i + 1]:
                    data[i], data[i + 1] = data[i + 1], data[i]
        
        total = 0.0
        for _ in range(10):
            total += 0.1
        
        if total == 1.0:
            print("Exactly 1.0")
        
        return data