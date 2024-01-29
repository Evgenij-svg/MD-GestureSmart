from flask import Flask, request, jsonify,render_template, url_for
import requests
import ipaddress
import json
import socket
import threading
from flask import Flask, request, jsonify, render_template
from flask_socketio import SocketIO
import time
 




app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")  # Разрешить соединения со всех источников

urls = {
    "clients":{
        "socket":[],
        "tcp":[],
        "post":[],
        "get":[],
    },
    "paj":[]
}

urls_clients = urls['clients']
# esp_ip_list = []

thread_finished = False

import ipaddress
import requests

def validate_ip(IpAdress):
    result = IpAdress.split(':')

    if len(result) >= 3:
        return False
    elif len(result) == 2:
        Ip, Port = result
        if not (Port.isdigit() and len(Port) == 4):
            return False
    elif len(result) == 1:
        Ip = result[0]

    try:
        ipaddress.ip_address(Ip)
    except ValueError:
        return False

    url = f'http://{IpAdress}/Gesture_data'
    data = {'gestures': 'Tests'}

    try:
        response = requests.post(url, data=data)
        response.raise_for_status()  # Проверка на ошибку в HTTP-ответе
        print("Data successfully sent to Server")
        return True
    except (requests.exceptions.RequestException, ipaddress.AddressValueError):
        return False

def send_to_servers(command_ges):
    for elem in urls_clients['post']:
        for command in elem['commands']:
            if(command == command_ges):
                urls_send = 'http://' + elem['nameUrl'] + '/Gesture_data'
                requests.post(urls_send, data={'gestures': command_ges})

def find_esp_ip_gesture():
    global thread_finished
    # Замените на диапазон IP-адресов вашей локальной сети
    ip_range = "192.168.0."

    # Замените на порт, который ESP слушает
    udp_port = 1234

    # Попробуем подключиться к каждому устройству в диапазоне
    for i in range(1, 255):
        target_ip = ip_range + str(i)
        try:
            # Создаем сокет и устанавливаем таймаут для ожидания ответа
            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            sock.settimeout(1)
            sock.sendto(b"ESP_REQUEST", (target_ip, udp_port))

            # Ждем ответа от ESP
            response, addr = sock.recvfrom(1024)
            print(target_ip)

            # Проверяем, что ответ содержит ожидаемую строку
            if "ESP_OK" in response.decode("utf-8"):
                print(f"Found ESP at IP: {target_ip}")
                
                milliseconds = int(round(time.time() * 1000))
                # print(milliseconds)

                dataPaj={
                    'ipPaj':target_ip,
                    'id':milliseconds
                }

                urls['paj'].append(dataPaj)
                socketio.emit('esp_ip_update', json.dumps(dataPaj))


        except socket.error as e:
            # Обработка ошибки подключения
            pass
    thread_finished = True

@app.route('/Get_gesture_GestureSmart', methods=['POST'])
def Get_gesture_GestureSmart():
    data = request.form
    if 'gestures' not in data:
        return jsonify({'error': 'Command not provided'}), 400
    command = data['gestures']
    send_to_servers(command)
    return jsonify({'success': True})

@app.route('/add_urls', methods=['POST'])
def add_urls():
    global urls
    data = request.form['url']
    id_url = request.form['id_url']
    _request = request.form['_request']
    print(data)
    if(validate_ip(data)):
        # url = 'http://' + data + '/Gesture_data'
        url_command = {
            'nameUrl':data,
            'commands':[],
            'id':int(id_url)
        }
        if not any(d['nameUrl'] == url_command['nameUrl'] for d in urls_clients[_request]):
            urls_clients[_request].append(url_command)
            return "True"

        return "False"
    return "False"

@app.route('/get_urls',methods=['POST'])
def getdata():
    global urls
    return json.dumps(urls)

@app.route('/remove_url',methods=['POST'])
def remove_url():
    data = request.form['id_url']
    _request = request.form['_request']
    for elem in urls_clients[_request]:
        if(elem['id'] == int(data)):
            urls_clients[_request].pop(urls_clients[_request].index(elem))
    return "Ok"

@app.route('/remove_url_paj',methods=['POST'])
def remove_url_paj():
    id_paj = request.form['id_paj']
    for elem in urls['paj']:
        if(elem['id'] == int(id_paj)):
            urls['paj'].pop(urls['paj'].index(elem))
    return "Ok"

@app.route('/Search_MD_GESTURE_PAJ', methods=['POST'])
def Search_MD_GESTURE_PAJ():
    # urls['paj'] = [] 
    find_esp_ip_gesture()
    return json.dumps(urls['paj'])

@app.route('/DataGesPAJ', methods=['POST'])
def DataGesPAJ():
    data = request.form
    if 'gesture' not in data:
        return jsonify({'error': 'Command not provided'}), 400
    command = data['gesture']
    send_to_servers(command)
    return jsonify({'success': True})

@app.route('/add_command', methods=['POST'])
def add_command():
    data = request.form
    id_url = data['id_url']
    command = data['command']
    _request = data['_request']
    # print(name + " " + command)
    for elem in urls_clients[_request]:
        if(elem['id'] == int(id_url)):
            elem['commands'].append(command)

    return jsonify({'success': True})

@app.route('/')
def start():
    return render_template("index.html")




if __name__ == '__main__':

    host = '192.168.0.14'  # или любой другой адрес
    # host = '0.0.0.0'
    port = 5000  # или любой другой порт
    socketio.run(app, host=host, port=port)







# import asyncio
# import websockets

# ESP_IP_ADDRESSES = ["192.168.1.100", "192.168.1.101", "192.168.1.102"]  # Замените этим своим списком IP-адресов ESP

# async def connect_to_esp(ip):
#     uri = f"ws://{ip}/your_websocket_path"  # Замените "your_websocket_path" на реальный путь WebSocket на вашем ESP

#     try:
#         async with websockets.connect(uri) as websocket:
#             print(f"Connected to ESP at {ip}")
#             # Вы можете здесь отправлять и принимать сообщения по WebSocket
#             # Например: await websocket.send("Hello, ESP!")
#             #         response = await websocket.recv()
#             #         print(f"Received from ESP: {response}")
#     except websockets.exceptions.ConnectionError:
#         print(f"Failed to connect to ESP at {ip}")

# async def main():
#     tasks = [connect_to_esp(ip) for ip in ESP_IP_ADDRESSES]
#     await asyncio.gather(*tasks)

# if __name__ == "__main__":
#     asyncio.run(main())
