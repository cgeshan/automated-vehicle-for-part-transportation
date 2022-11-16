import sys, serial, time
from mainWindowUI import QtCore, QtGui, QtWidgets, Ui_MainWindow
from PyQt5 import QtCore, QtWidgets
from PyQt5.QtWidgets import QMainWindow, QLabel, QGridLayout, QWidget
from PyQt5.QtWidgets import QPushButton
from PyQt5.QtCore import QSize  


try:
    ser = serial.Serial('COM7', 9600, timeout = 1)


    class MainWindow(QMainWindow, Ui_MainWindow):
        def __init__(self):
            QMainWindow.__init__(self)
            self.setupUi(self)
            self.pushButtonStation1 = QtWidgets.QPushButton(self.centralwidget)
            self.pushButtonStation1.setGeometry(QtCore.QRect(250, 550, 151, 61))
            font = QtGui.QFont()
            font.setPointSize(24)
            font.setBold(False)
            font.setWeight(50)
            self.pushButtonStation1.setFont(font)
            self.pushButtonStation1.setObjectName("pushButtonStation1")
            self.pushButtonStation1.setText("Station 1")
            self.pushButtonStation2 = QtWidgets.QPushButton(self.centralwidget)
            self.pushButtonStation2.setGeometry(QtCore.QRect(250, 362, 151, 61))
            font = QtGui.QFont()
            font.setPointSize(24)
            font.setBold(False)
            font.setWeight(50)
            self.pushButtonStation2.setFont(font)
            self.pushButtonStation2.setObjectName("pushButtonStation2")
            self.pushButtonStation2.setText("Station 2")
            self.pushButtonStation3 = QtWidgets.QPushButton(self.centralwidget)
            self.pushButtonStation3.setGeometry(QtCore.QRect(250, 175, 151, 61))
            font = QtGui.QFont()
            font.setPointSize(24)
            font.setBold(False)
            font.setWeight(50)
            self.pushButtonStation3.setFont(font)
            self.pushButtonStation3.setObjectName("pushButtonStation3")
            self.pushButtonStation3.setText("Station 3")
            self.pushButtonStation6 = QtWidgets.QPushButton(self.centralwidget)
            self.pushButtonStation6.setGeometry(QtCore.QRect(690, 550, 151, 61))
            font = QtGui.QFont()
            font.setPointSize(24)
            font.setBold(False)
            font.setWeight(50)
            self.pushButtonStation6.setFont(font)
            self.pushButtonStation6.setObjectName("pushButtonStation6")
            self.pushButtonStation6.setText("Station 6")
            self.pushButtonStation4 = QtWidgets.QPushButton(self.centralwidget)
            self.pushButtonStation4.setGeometry(QtCore.QRect(690, 175, 151, 61))
            font = QtGui.QFont()
            font.setPointSize(24)
            font.setBold(False)
            font.setWeight(50)
            self.pushButtonStation4.setFont(font)
            self.pushButtonStation4.setObjectName("pushButtonStation4")
            self.pushButtonStation4.setText("Station 4")
            self.pushButtonStation5 = QtWidgets.QPushButton(self.centralwidget)
            self.pushButtonStation5.setGeometry(QtCore.QRect(690, 362, 151, 61))
            font = QtGui.QFont()
            font.setPointSize(24)
            font.setBold(False)
            font.setWeight(50)
            self.pushButtonStation5.setFont(font)
            self.pushButtonStation5.setObjectName("pushButtonStation5")
            self.pushButtonStation5.setText("Station 5")
            self.pushButtonHomeBase = QtWidgets.QPushButton(self.centralwidget)
            self.pushButtonHomeBase.setGeometry(QtCore.QRect(440, 1010, 191, 61))
            font = QtGui.QFont()
            font.setPointSize(24)
            font.setBold(False)
            font.setWeight(50)
            self.pushButtonHomeBase.setFont(font)
            self.pushButtonHomeBase.setObjectName("pushButtonHomeBase")
            self.pushButtonHomeBase.setText("Home Base")
            self.labelDesiredDeliveries = QtWidgets.QLabel(self.centralwidget)
            self.labelDesiredDeliveries.setGeometry(QtCore.QRect(1090, 30, 371, 71))
            font = QtGui.QFont()
            font.setPointSize(24)
            self.labelDesiredDeliveries.setFont(font)
            self.labelDesiredDeliveries.setObjectName("labelDesiredDeliveries")
            self.pushButtonStopDeliveries = QtWidgets.QPushButton(self.centralwidget)
            self.pushButtonStopDeliveries.setGeometry(QtCore.QRect(1090, 1010, 391, 71))
            font = QtGui.QFont()
            font.setPointSize(24)
            font.setBold(False)
            font.setWeight(50)
            self.pushButtonStopDeliveries.setFont(font)
            self.pushButtonStopDeliveries.setObjectName("pushButtonStopDeliveries")
            self.pushButtonStopDeliveries.setText("Stop!")
            self.pushButtonStartDeliveries = QtWidgets.QPushButton(self.centralwidget)
            self.pushButtonStartDeliveries.setGeometry(QtCore.QRect(1090, 900, 391, 71))
            font = QtGui.QFont()
            font.setPointSize(24)
            font.setBold(False)
            font.setWeight(50)
            self.pushButtonStartDeliveries.setFont(font)
            self.pushButtonStartDeliveries.setObjectName("pushButtonStartDeliveries")
            self.pushButtonStartDeliveries.setText("Start Deliveries")
            self.labelLiveUpdates = QtWidgets.QLabel(self.centralwidget)
            self.labelLiveUpdates.setGeometry(QtCore.QRect(1090, 380, 371, 71))
            font = QtGui.QFont()
            font.setPointSize(24)
            self.labelLiveUpdates.setFont(font)
            self.labelLiveUpdates.setObjectName("labelLiveUpdates")
            self.listDesiredDeliveries = QtWidgets.QListWidget(self.centralwidget)
            self.listDesiredDeliveries.setGeometry(QtCore.QRect(1090, 90, 401, 261))
            self.listDesiredDeliveries.setObjectName("listDesiredDeliveries")
            self.listLiveUpdates = QtWidgets.QListWidget(self.centralwidget)
            self.listLiveUpdates.setGeometry(QtCore.QRect(1090, 460, 401, 421))
            self.listLiveUpdates.setObjectName("listLiveUpdates")
            self.labelPlantLayout = QtWidgets.QLabel(self.centralwidget)
            self.labelPlantLayout.setGeometry(QtCore.QRect(10, 10, 1071, 1171))
            self.labelPlantLayout.setText("")
            self.labelPlantLayout.setPixmap(QtGui.QPixmap(":/newPrefix/plantLayout.png"))
            self.labelPlantLayout.setObjectName("labelPlantLayout")
            self.pushButtonRemoveStop.setText("Remove Stop")
            self.pushButtonClearStops.setText("Clear Stops")
            self.labelPlantLayout.raise_()
            self.pushButtonStation1.raise_()
            self.pushButtonStation2.raise_()
            self.pushButtonStation3.raise_()
            self.pushButtonStation6.raise_()
            self.pushButtonStation4.raise_()
            self.pushButtonStation5.raise_()
            self.pushButtonHomeBase.raise_()
            self.labelDesiredDeliveries.raise_()
            self.pushButtonStopDeliveries.raise_()
            self.pushButtonStartDeliveries.raise_()
            self.labelLiveUpdates.raise_()
            self.listDesiredDeliveries.raise_()
            self.listLiveUpdates.raise_()
            self.pushButtonClearStops.raise_()
            self.pushButtonRemoveStop.raise_()
    # Linking all buttons
            self.pushButtonHomeBase.clicked.connect(self.dialogHomeBase)
            self.pushButtonStation1.clicked.connect(self.dialogStation1)
            self.pushButtonStation2.clicked.connect(self.dialogStation2)
            self.pushButtonStation3.clicked.connect(self.dialogStation3)
            self.pushButtonStation4.clicked.connect(self.dialogStation4)
            self.pushButtonStation5.clicked.connect(self.dialogStation5)
            self.pushButtonStation6.clicked.connect(self.dialogStation6)
            self.pushButtonStartDeliveries.clicked.connect(self.dialogStartDeliveries)
            self.pushButtonStopDeliveries.clicked.connect(self.dialogStopDeliveries)
            self.pushButtonClearStops.clicked.connect(self.clearStops)
            self.pushButtonRemoveStop.clicked.connect(self.removeStop)
            
            
    #Setting up the text values of all pushbuttons
        def clearStops(self):
            #print("All Stops Cleared!")
            self.listDesiredDeliveries.clear()
        
        def removeStop(self):
            listItems=self.listDesiredDeliveries.selectedItems()
            if not listItems: return        
            for item in listItems:
                self.listDesiredDeliveries.takeItem(self.listDesiredDeliveries.row(item))
            #print(item.text() + " Removed!")
            
        def dialogHomeBase(self):
        # print("Home Base")
            self.listDesiredDeliveries.addItem("Home Base")
            self.listDesiredDeliveries.sortItems()
            
        def dialogStation1(self):
        # print('Station 1')
            self.listDesiredDeliveries.addItem("Station 1")
            self.listDesiredDeliveries.sortItems()
            #self.pushButtonStation1.setStyleSheet('background-color:#90EE90;')
            
        def dialogStation2(self):
        # print('Station 2')
            self.listDesiredDeliveries.addItem("Station 2")
            self.listDesiredDeliveries.sortItems()
            #self.pushButtonStation2.setStyleSheet('background-color:#90EE90;')
            
        def dialogStation3(self):
        # print('Station 3')
            self.listDesiredDeliveries.addItem("Station 3")
            self.listDesiredDeliveries.sortItems()
        # self.pushButtonStation3.setStyleSheet('background-color:#90EE90;')
            
        def dialogStation4(self):
            #print('Station 4')
            self.listDesiredDeliveries.addItem("Station 4")
            self.listDesiredDeliveries.sortItems()
            #self.pushButtonStation4.setStyleSheet('background-color:#90EE90;')
            
        def dialogStation5(self):
        # print('Station 5')
            self.listDesiredDeliveries.addItem("Station 5")
            self.listDesiredDeliveries.sortItems()
            #self.pushButtonStation5.setStyleSheet('background-color:#90EE90;')
            
        def dialogStation6(self):
        #  print('Station 6')
            self.listDesiredDeliveries.addItem("Station 6")
            self.listDesiredDeliveries.sortItems()
            #self.pushButtonStation6.setStyleSheet('background-color:#90EE90;')
        
        def dialogStartDeliveries(self):
        # print("Start Deliveries")
            self.sendJob()
            
        def dialogStopDeliveries(self):
            print("Stop Now, RETURN HOME!")

    #Create sorted list of jobs
        def sendJob(self):
            self.listLiveUpdates.clear() 
            self.listDesiredDeliveries.addItem('Hub')
            print("Added Home Station To Deliveries")
            QtWidgets.QApplication.processEvents()
            entries = range(self.listDesiredDeliveries.count())

            for i in entries:
                QtWidgets.QApplication.processEvents()
                station = self.listDesiredDeliveries.item(i).text()
                values = station.split()
                stationNum = values[-1]
                if station == 'Hub':
                    stationNum = '0'
                
                ser.write(bytes(str(stationNum), 'utf-8'))
                print('sent station: ', stationNum)
                status = ser.readline().decode('ascii')

                while status == '':
                    status = ser.readline().decode('ascii')

                if status == 'Delivered':
                    statusUpdate = 'Completed Delivery to Station ' + stationNum
                    self.listLiveUpdates.addItem(statusUpdate)
                    QtWidgets.QApplication.processEvents()
                    print("Read Delivered from HC05")
                    time.sleep(0.1)
                        
                    i = i+1  

                elif status == 'Hub':
                    statusUpdate = 'Vehicle has returned to the Hub '
                    self.listLiveUpdates.addItem(statusUpdate)
                    QtWidgets.QApplication.processEvents()
                    
                    print("Read Hub from HC05")
                    time.sleep(0.1)
                    break

            self.listDesiredDeliveries.clear()
              

    if __name__ == "__main__":
        app = QtWidgets.QApplication([])
        window = MainWindow()    
        window.show()
        app.exec_()  

except:
    print("*******Error Encountered Starting Interfeace*******\n")
    print("No Bluetooth Device Found...")
    print("   Please ensure the vehicle is turned on, and Bluetooth Module is blinking red!")
    print("   Once powered on, please restart interface application.\n")



 
