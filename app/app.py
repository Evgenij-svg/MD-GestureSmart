from flask import Flask, request, jsonify,render_template, url_for
import requests
import ipaddress
import json
import socket
import threading
from flask import Flask, request, jsonify, render_template
from flask_socketio import SocketIO

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")  # Разрешить соединения со всех источников


urls = []
esp_ip_list = []

thread_finished = False

def validate_ip(IpAdress):
    result = IpAdress.split(':')
    Ip = ''
    Port = ''
    if(len(result) >= 3):
        return False
    elif(len(result) == 2):
        Ip = result[0]
        Port = result[1]
        if((not Port.isdigit()) or (not (len(Port) == 4))):
            return False
    elif(len(result) == 1):
        Ip = result[0]
        
    try:
        print(Ip)
        ipaddress.ip_address(Ip)
    except ValueError:
        return False

    url = 'http://' + IpAdress + '/Gesture_data'
    data = {'gestures': 'Tests'}
    try:
        response = requests.post(url, data=data)
    except requests.exceptions.ConnectionError:
        return False

    if response.status_code == 200:
        print("Data successfully sent to Serve")
    else:
        return False

    return True

def send_to_servers(command_ges):
    for elem in urls:
        for command in elem['commands']:
            if(command == command_ges):
                requests.post(elem['nameUrl'], data={'gestures': command_ges})

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
                esp_ip_list.append(target_ip)
                socketio.emit('esp_ip_update', target_ip)


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
    data = request.form['input_data']
    if(validate_ip(data)):
        url = 'http://' + data + '/Gesture_data'
        url_command = {
            'nameUrl':url,
            'commands':[]
        }
        if not any(d['nameUrl'] == url_command['nameUrl'] for d in urls):
            urls.append(url_command)
            return "True"

        return "False"
    return "False"

@app.route('/get_urls',methods=['POST'])
def getdata():
    global urls
    return json.dumps(urls)

@app.route('/remove_url',methods=['POST'])
def remove_url():
    data = request.form['name_url']
    for elem in urls:
        if(elem['nameUrl'].split('/')[2] == data):
            urls.pop(urls.index(elem))
    return "Ok"

@app.route('/Search_MD_GESTURE_PAJ', methods=['POST'])
def Search_MD_GESTURE_PAJ():
    # thread = threading.Thread(target=find_esp_ip_gesture)
    # thread.start()
    # thread.join()  # Wait for the thread to finish
    esp_ip_list = []  # Создаем новый список для каждого запроса
    find_esp_ip_gesture()
    return json.dumps(esp_ip_list)

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
    name = data['name_url']
    command = data['command']
    # print(name + " " + command)
    for elem in urls:
        if(elem['nameUrl'].split('/')[2] == name):
            elem['commands'].append(command)

    print(json.dumps(urls))
    return jsonify({'success': True})

@app.route('/')
def start():
    return render_template("index.html")




if __name__ == '__main__':

    host = '192.168.0.14'  # или любой другой адрес
    # host = '0.0.0.0'
    port = 5000  # или любой другой порт
    socketio.run(app, host=host, port=port)


