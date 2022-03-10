from mainWindowUI import QtCore, QtGui, QtWidgets, Ui_MainWindow

class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self, *args, **kwargs):
        QtWidgets.QMainWindow.__init__(self, *args, **kwargs)
        self.setupUi(self)
        
    def linkButtons(self):
        self.pushButtonHomeBase.dialogHomeBase.connect(self.dialogHomeBase)
        self.pushButtonStation1.clicked.connect(self.dialogStation1)
        self.pushButtonStation2.clicked.connect(self.dialogStation2)
        self.pushButtonStation3.clicked.connect(self.dialogStation3)
        self.pushButtonStation4.clicked.connect(self.dialogStation4)
        self.pushButtonStation5.clicked.connect(self.clicked)
        self.pushButtonStation6.clicked.connect(self.dialogStation6)
    
    def dialogHomeBase(self):
        self.labelTest.setText("HOPEFUL")
        
    def dialogStation1(self):
        print('Station 1')
        
    def dialogStation2(self):
        print('Station 2')
        
    def dialogStation3(self):
        print('Station 3')
        
    def dialogStation4(self):
        print('Station 4')
        
    def dialogStation5(self):
        print('Station 5')
        
    def dialogStation6(self):
        print('Station 6')
        
        
if __name__ == "__main__":
    app = QtWidgets.QApplication([])
    window = MainWindow()    
    window.show()
    app.exec_()  