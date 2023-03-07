from flask import Flask, make_response, render_template, request, url_for, redirect
from flask_sqlalchemy import SQLAlchemy
from matplotlib.figure import Figure
import base64
from io import BytesIO
import json
import time

from data_get import FULL_ARDUINO_IP, get_text_data, parse_response_text

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql://root:password@localhost:3306/FRblackbox'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

db = SQLAlchemy(app)

class test(db.Model):
	id = db.Column(db.Integer, primary_key=True)
	col1 = db.Column(db.Integer)

class maindata(db.Model):
	id = db.Column(db.Integer, primary_key=True)
	sos = db.Column(db.Boolean, nullable=False)
	fall = db.Column(db.Boolean, nullable=False)

@app.route('/')
def hello():
	info = test.query.first()
	response = make_response(str(info.col1), 200)
	response.mimetype = "text/plain"
	response.headers.add('Access-Control-Allow-Origin', '*')
	return response

@app.route('/receiveData')
def receiveData():
	while True:
		text_response = get_text_data(FULL_ARDUINO_IP, "/sensor/all")

		parsed_data = parse_response_text(text_response)
		if(not parsed_data.get('SOS')): print("yay")
		finalData = maindata(
			sos = parsed_data.get('SOS'),
			fall = False
		)
		db.session.add(finalData)
		db.session.commit()

		time.sleep(5)

@app.route('/graph', methods=['GET'])
def graph():
	

	fig = Figure()
	ax = fig.subplots()
	ax.plot([0.3,0.4,10],[0.5,0.7,7])
	buf = BytesIO()
	fig.savefig(buf, format="png")
	data = base64.b64encode(buf.getbuffer()).decode("ascii")
	html = f"<img src='data:image/png;base64,{data}'/>"

	response = make_response(html, 200)
	response.mimetype = "text/html"
	response.headers.add('Access-Control-Allow-Origin', '*')
	return response
