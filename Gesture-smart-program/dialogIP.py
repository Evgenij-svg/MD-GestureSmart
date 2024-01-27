from PyQt5 import QtGui, QtWidgets, uic
from PyQt5.QtCore import QThread, pyqtSignal
import ipaddress
import requests
from smarthouse import MainGesture
import sys

class WorkerThread(QThread):
    finished = pyqtSignal()

    def __init__(self, IP_Address, error_label, parent=None):
        super(WorkerThread, self).__init__(parent)
        self.IPAddrres = IP_Address
        self.error_label = error_label
        self.url = ""

    def run(self):
        if self.validate_ip():
            self.finished.emit()

    def validate_ip(self):
        Ip = ''
        Port = ''
        result = self.IPAddrres.split(':')
        if(len(result) >= 3):
            self.error_label.setText('URL is not written correctly')
            return False
        elif(len(result) == 2):
            Ip = result[0]
            Port = result[1]
            if((not Port.isdigit()) or (not (len(Port) == 4))):
                self.error_label.setText('Port is not written correctly')
                return False
        elif(len(result) == 1):
            Ip = result[0]
        
        try:
            print(Ip)
            ipaddress.ip_address(Ip)
        except ValueError:
            self.error_label.setText('No valid IP')
            return False

        self.url = 'http://' + self.IPAddrres + '/Get_gesture_GestureSmart'
        data = {'gestures': ''}
        try:
            response = requests.post(self.url, data=data)
        except requests.exceptions.ConnectionError:
            self.error_label.setText('Error connection')
            return False

        if response.status_code == 200:
            print("Data successfully sent to Serve")
        else:
            self.error_label.setText('Error sending to Serve')
            return False

        return True

class dialogIP(QtWidgets.QDialog):
    def __init__(self):
        super(dialogIP, self).__init__()
        uic.loadUi("./UI/DialogIPesp.ui", self)

        window_icon = QtGui.QIcon("./icon/smarthouse.png")
        self.setWindowIcon(window_icon)

        self.Cancel.clicked.connect(self.btnClickedCancel)
        self.OK.clicked.connect(self.btnClickedOK)

        self.IPAddress = ''
        self.url = ''
    
    def btnClickedOK(self):
        self.IPAddress = self.IP.text()
        self.url = 'http://' + self.IPAddress + '/Get_gesture_GestureSmart'

        self.Error.setText('Wait')
        self.worker_thread = WorkerThread(self.IPAddress, self.Error)
        self.worker_thread.finished.connect(self.worker_thread_finished)
        self.worker_thread.start()

    def worker_thread_finished(self):
        self.CameraApp = MainGesture(self.url)
        self.CameraApp.show()
        self.accept()

    def btnClickedCancel(self):
        self.reject()

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = dialogIP()
    window.show()
    sys.exit(app.exec())
