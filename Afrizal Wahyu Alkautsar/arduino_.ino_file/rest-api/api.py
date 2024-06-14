from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/api/ta', methods=['POST'])
def data():
    temperature = request.form.get('temperature')

    if temperature:
        response = {
            'status': 'success',
            'temperature': temperature
        }
        return jsonify(response), 200
    else:
        response = {
            'status': 'failed',
            'message': 'Invalid data'
        }
        return jsonify(response), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
