from flask import Flask, make_response, render_template, request, url_for, redirect
from flask_sqlalchemy import SQLAlchemy
from matplotlib.figure import Figure
from matplotlib.dates import date2num
import base64
from io import BytesIO
import json
import time
from datetime import datetime, timedelta

from data_get import FULL_ARDUINO_IP, get_text_data, parse_response_text, SOS_toggle
from gps_plot import  gen_gps_html

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql://root:password@localhost:3306/FRblackbox'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

db = SQLAlchemy(app)

class maindata(db.Model):
	id = db.Column(db.Integer, primary_key=True)
	sos = db.Column(db.Boolean, nullable=False)
	fall = db.Column(db.Boolean, nullable=False)
	humid = db.Column(db.Float, nullable=False)
	temp = db.Column(db.Float, nullable=False)
	pressure = db.Column(db.Integer, nullable=False)
	co2 = db.Column(db.Integer, nullable=False)
	latitude = db.Column(db.Float, nullable=False)
	longitude = db.Column(db.Float, nullable=False)
	timereceived = db.Column(db.DateTime, nullable=False)

@app.route('/receiveData')
def receiveData():
	db.session.query(maindata).filter(maindata.timereceived < datetime.now() - timedelta(hours=1)).delete()
	db.session.commit()

	while True:
		text_response = get_text_data(FULL_ARDUINO_IP, "/sensor/all")
		if(text_response != False):
			parsed_data = parse_response_text(text_response)
			finalData = maindata(
				sos = parsed_data.get('SOS'),
				fall = False,
				humid = parsed_data.get('humid'),
				temp = parsed_data.get('temp'),
				pressure = parsed_data.get('pressure'),
				co2 = parsed_data.get('CO2'),
				latitude = parsed_data.get('latitude'),
				longitude = parsed_data.get('longitude'),
				timereceived = datetime.now()
			)
			db.session.add(finalData)
			db.session.commit()

		time.sleep(0.5)

@app.route('/querySOS', methods=['GET'])
def querySOS():
	data = db.session.query(maindata.sos).filter(maindata.timereceived > datetime.now() - timedelta(hours=1)).order_by(maindata.id.desc()).first()

	res = "False"
	if data is not None:
		res = str(data.sos)

	response = make_response(res, 200)
	response.mimetype = "text/plain"
	response.headers.add("Access-Control-Allow-Origin", "*")
	return response

@app.route('/sendSOS')
def sendSOS():
	data = db.session.query(maindata.sos).filter(maindata.timereceived > datetime.now() - timedelta(hours=1)).order_by(maindata.id.desc()).first()
	toggle = "off"
	if(not data):
		toggle = "on"
	SOS_toggle(FULL_ARDUINO_IP, toggle)

	response = make_response("ok", 200)
	response.mimetype = "text/plain"
	response.headers.add("Access-Control-Allow-Origin", "*")
	return response

@app.route('/generateGPS')
def generateGPS():
	coord = db.session.query(maindata.latitude, maindata.longitude).order_by(maindata.id.desc()).first()

	if coord is not None:
		gen_gps_html(coord.latitude, coord.longitude)

	response = make_response("ok", 200)
	response.mimetype = "text/plain"
	response.headers.add("Access-Control-Allow-Origin", "*")
	return response

@app.route('/graph', methods=['GET'])
def graph():
	dataset = db.session.query(maindata).filter(maindata.timereceived > datetime.now() - timedelta(hours=1)).all()
	humidx = []
	tempx = []
	pressurex = []
	co2x = []
	timescale = []

	for data in dataset:
		humidx.append(data.humid)
		tempx.append(data.temp)
		pressurex.append(data.pressure)
		co2x.append(data.co2)
		timescale.append(data.timereceived)

	plotDatax = [humidx, tempx, pressurex, co2x]
	title = ["Humidity", "Temperature", "Pressure", "CO2"]
	ylim = [[0,100], [20, 30], [101000, 103000], [0, 1000]]
	ylabel = ["Percent (%)", "Degrees (C)", "Pascals (Pa)", "Concentration (PPM)"]
	html = []
	for n in range(0,4):
		fig = Figure(figsize=(10,6))
		plot = fig.subplots()
		plot.plot(date2num(timescale), plotDatax[n])
		plot.set_xlim([datetime.now() - timedelta(minutes=10), datetime.now()])
		plot.set_ylim(ylim[n])
		plot.set_title(title[n])
		plot.set_ylabel(ylabel[n])
		plot.set_xlabel("Time (dd mm:ss)")

		buf = BytesIO()
		fig.savefig(buf, format="png")
		pic = base64.b64encode(buf.getbuffer()).decode("ascii")
		html.append(f"<img src='data:image/png;base64,{pic}'/>")

	response = make_response(html, 200)
	response.mimetype = "application/json"
	response.headers.add('Access-Control-Allow-Origin', '*')
	return response
