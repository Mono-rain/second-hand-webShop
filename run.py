from app import create_app

app = create_app()

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)

# user_routes my_orders.html, my_items.html, settings.html not completed!