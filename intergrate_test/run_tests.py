# testFolder/run_tests.py
import pytest
import sys
import os

# 设置项目根目录
current_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.dirname(current_dir)
sys.path.insert(0, project_root)

if __name__ == '__main__':
    # 运行所有测试
    pytest.main([
        '-v',
        '--tb=short',  # 简洁的错误信息
        '--cov=models',  # 覆盖models目录
        '--cov-report=term-missing',
        current_dir
    ])