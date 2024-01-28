from PyQt5 import QtCore, QtGui, QtWidgets, uic
import cv2 
import mediapipe as mp
from PyQt5.QtCore import QThread, pyqtSignal
from logicGesture import find_matching_gesture
import os
from dialogSaveGest import DialogSaveGestures
import requests
import json


class WorkerThread(QThread):
    finished = pyqtSignal()

    def __init__(self,gesture,url,  parent=None):
        super(WorkerThread, self).__init__(parent)
        self.gesture = gesture
        self.url = url

    def run(self):
        if(self.gesture):
            data={'gestures':self.gesture}
            response = requests.post(self.url, data=data)
            if response.status_code == 200:
                self.finished.emit()
            else:
                print("Ошибка при отправке данных на ESP32")
            


class MainGesture(QtWidgets.QMainWindow):
    def __init__(self, url):
        super(MainGesture, self).__init__()
        uic.loadUi("./UI/smarthouse.ui", self)

        window_icon = QtGui.QIcon("./icon/smarthouse.png")
        self.setWindowIcon(window_icon)

        self.cap = cv2.VideoCapture(2)

        self.hands = mp.solutions.hands.Hands(static_image_mode=False,
                                 max_num_hands=1,
                                 min_tracking_confidence = 0.5,
                                 min_detection_confidence= 0.5)
        self.mpDraw = mp.solutions.drawing_utils

        self.keypoints = []
        self.gestures = ''
        self.rgb_image = None

        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.update_frame)
        self.timer.start(30)
        
        self.pushButton.clicked.connect(self.btnClickedSave)

        self.count_image = 0

        self.dialog = None 

        self.url = url

    def btnClickedSave(self):
        if self.keypoints:
            path = os.getcwd()+'/img/{}.png'.format(self.count_image)
            ret, frame = self.cap.read()
            
            cv2.imwrite(path, frame)
            self.dialog = DialogSaveGestures(self.count_image)
            self.dialog.show()
            self.dialog.accepted.connect(self.onDialogAccepted)
            self.dialog.rejected.connect(self.onDialogRejected) 
            self.count_image += 1

            with open('data.json', 'w') as file:
                json.dump(self.keypoints, file)

    def CheckFile(self):
        count = 0
        base_name = self.dialog.name + str(count)
        directory = os.path.join(os.getcwd(), 'json')
        print(directory)
        while os.path.exists(os.path.join(directory, f'{base_name}.json')):
            print(os.path.join(directory, f'{base_name}.json'))
            count += 1
            base_name = self.dialog.name + str(count)

        return count

    def onDialogAccepted(self):
        old_name = os.getcwd()+'/img/{}.png'.format(self.count_image - 1)
        pathJSONold = os.getcwd()+'./data.json'

        
        count = self.CheckFile()
        
        print(count)
        new_name = os.getcwd()+'/img/{}.png'.format(self.dialog.name + str(count))
        pathJSONnew = os.getcwd()+'/json/{}.json'.format(self.dialog.name + str(count))

        os.rename(old_name, new_name)
        os.rename(pathJSONold, pathJSONnew)

    def onDialogRejected(self):
        path = os.getcwd()+'/img/{}.png'.format(self.count_image - 1)
        os.remove(path)

    def update_frame(self):
        ret, frame = self.cap.read()
        if ret:

            self.rgb_image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

            h, w, ch = self.rgb_image.shape
            bytes_per_line = ch * w
            result = self.hands.process(self.rgb_image)
            self.keypoints = []

            if result.multi_hand_landmarks:
                for id,hand_landmarks in enumerate(result.multi_hand_landmarks[0].landmark):
                    self.keypoints.append({
                        'id':id,
                        'x': hand_landmarks.x,
                        'y': hand_landmarks.y,
                        'z': hand_landmarks.z
                    })
                    cx,cy = int(hand_landmarks.x * w),int(hand_landmarks.y*h)
                    cv2.circle(self.rgb_image,(cx,cy),3,(255,0,255))
                    self.mpDraw.draw_landmarks(self.rgb_image,result.multi_hand_landmarks[0],mp.solutions.hands.HAND_CONNECTIONS)
                    
                self.gestures = find_matching_gesture(self.keypoints)

                self.worker_thread = WorkerThread(self.gestures, self.url)
                self.worker_thread.finished.connect(self.worker_thread_finished)
                self.worker_thread.start()

                self.updateText()    

            q_img = QtGui.QImage(self.rgb_image.data, w, h, bytes_per_line, QtGui.QImage.Format_RGB888)
            pixmap = QtGui.QPixmap.fromImage(q_img)
            self.label.setPixmap(pixmap)

    def worker_thread_finished(self):
        print("Данные успешно отправлены на ESP32")

    def updateText(self):
        if(self.gestures):
            self.label_2.setText(self.gestures)
