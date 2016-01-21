from application import app
from flask import Flask, jsonify, render_template, request


@app.route('/')
@app.route('/index')
def index():
    return render_template("index.html", name="apple") 

@app.route('/add')
def add_numbers():
    a = request.args.get('a', 0, type=int)
    b = request.args.get('b', 0, type=int)
    print jsonify(result = a + b)
    return jsonify(result = a + b)
