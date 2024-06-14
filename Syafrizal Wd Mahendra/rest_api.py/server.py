from flask import Flask, request, jsonify

app = Flask(__name__)

# Variabel global untuk menyimpan data terakhir
last_data = {}

@app.route('/data', methods=['POST'])
def receive_data():
    global last_data
    if request.is_json:
        data = request.get_json()
        if 'temperature' in data and 'humidity' in data:
            temperature = data['temperature']
            humidity = data['humidity']
            last_data = {
                "temperature": temperature,
                "humidity": humidity
            }
            print(f"Received temperature: {temperature} C, humidity: {humidity} %")
            return jsonify({
                "status": "success", 
                "message": "Data received"
            }), 200
        else:
            return jsonify({
                "status": "fail", 
                "message": "Invalid data"
            }), 400
    else:
        return jsonify({
            "status": "fail", 
            "message": "Content-Type must be application/json"
        }), 415

@app.route('/data', methods=['GET'])
def get_data():
    global last_data
    if last_data:
        return jsonify({
            "status": "success",
            "data": last_data
        }), 200
    else:
        return jsonify({
            "status": "fail",
            "message": "No data available"
        }), 404

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
