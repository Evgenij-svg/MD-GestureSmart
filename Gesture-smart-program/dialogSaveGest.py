from PyQt5 import QtGui, QtWidgets, uic
import os

class DialogSaveGestures(QtWidgets.QDialog):
    def __init__(self, count_image):
        super(DialogSaveGestures, self).__init__()
        uic.loadUi("./UI/DialogSaveGestures.ui", self)

        # Set the window icon
        window_icon = QtGui.QIcon("./icon/smarthouse.png")
        self.setWindowIcon(window_icon)

        path = os.getcwd()+'/img/{}.png'.format(count_image)
        
        image = QtGui.QImage(path)

        pixmap = QtGui.QPixmap.fromImage(image)
        self.Image.setPixmap(pixmap)

        self.Cancel.clicked.connect(self.btnClickedCancel)
        self.OK.clicked.connect(self.btnClickedOK)

        self.name = ''

    def btnClickedOK(self):
        self.name = self.NameGest.text()
        self.accept()  # Call accept() to trigger the accepted signal

    def btnClickedCancel(self):
        self.reject()  # Call reject() to trigger the rejected signal
     # Call accept() to trigger the accepted signal

