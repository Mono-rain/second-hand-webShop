from flask import Flask, render_template
from config import config
from models.database import init_db
from routes.auth_routes import auth_bp
from routes.item_routes import item_bp
from routes.search_routes import search_bp
from routes.user_routes import user_bp
from routes.home_routes import home_bp

def create_app(config_name='default'):
    app = Flask(__name__)
    app.config.from_object(config[config_name])
    
    # 初始化数据库
    init_db(app)
    
    # 注册蓝图
    app.register_blueprint(home_bp)
    app.register_blueprint(auth_bp, url_prefix='/auth')
    app.register_blueprint(item_bp, url_prefix='/item')
    app.register_blueprint(search_bp, url_prefix='/search')
    app.register_blueprint(user_bp, url_prefix='/user')
    
    # 错误处理
    @app.errorhandler(404)
    def not_found(error):
        return render_template('404.html'), 404
    
    @app.errorhandler(500)
    def internal_error(error):
        return render_template('500.html'), 500
    
    return app

if __name__ == '__main__':
    app = create_app()
    app.run(debug=True)