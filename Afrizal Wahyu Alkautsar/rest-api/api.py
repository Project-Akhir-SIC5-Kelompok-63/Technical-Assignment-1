from flask import Flask, request, jsonify

app = Flask(__name__)

temperature_data = []

@app.route('/api/ta', methods=['POST'])
def data():
    temperature = request.form.get('temperature')

    if temperature:
        temperature_data.append(temperature)
        response = {
            'status': 'success',
            'temperature': temperature,
            'all_temperatures': temperature_data
        }
        return jsonify(response), 200
    else:
        response = {
            'status': 'failed',
            'message': 'Invalid data'
        }
        return jsonify(response), 400

@app.route('/api/ta', methods=['GET'])
def get_data():
    response = {
        'status': 'success',
        'data': temperature_data
    }
    return jsonify(response), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
