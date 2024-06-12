from flask import Flask, jsonify
from flask import request
import time

app = Flask('Flask_Testing')

airQuality = []
timestamp = []

@app.route('/', methods= ['GET'])
def hello_world():
    response = {
            "status": "success fetch",
            "message": "Data received",
            "data": [
                {
                    "air quality": airQuality,
                    "timestamp": timestamp
                }
            ]
        }
    return jsonify(response), 200

@app.route('/sensor/data',methods=['POST'])
def data():
    data = request.json
    gas = data.get('gas')
    times = time.time()
    
    airQuality.append(gas)
    timestamp.append(times)
    
    response = {
            "status": "success add",
            "message": "Data updated",
            "data": [
                {
                    "gas": gas,
                    "timestamp": times
                }
            ]
        }
    return jsonify(response), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0')