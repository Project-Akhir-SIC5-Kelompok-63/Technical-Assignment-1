from flask import Flask, request, jsonify

app = Flask(__name__)
# Tempat menyimpan data untuk menyimpelkan codingan
data_storage = []

@app.route('/api/dht', methods=['POST'])
def handle_dht():
    data = request.get_json()
    if not data or 'temperature' not in data or 'humidity' not in data:
        return jsonify({'error': 'invalid data'}), 400
        
    # olah dan simpan data
    entry = {
        'temperature': data['temperature'],
        'humidity': data['humidity']
    }
    data_storage.append(entry)

    # response
    return jsonify({'Pesan': 'Data Diterima', 'data': entry}), 200
    
@app.route('/api/dht/all', methods=['GET'])
def get_all_data():
    return jsonify(data_storage), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)