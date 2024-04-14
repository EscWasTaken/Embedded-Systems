# Library Imports
from PyQt5 import QtWidgets
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QColor
from PyQt5.QtWidgets import *
import RPi.GPIO as GPIO
import sys

GPIO.setwarnings(False) # Ignore warning for now
GPIO.setmode(GPIO.BOARD) # Use physical pin numbering
GPIO.setup(11, GPIO.OUT, initial=GPIO.LOW) # Blue
GPIO.setup(13, GPIO.OUT, initial=GPIO.LOW) # Green
GPIO.setup(15, GPIO.OUT, initial=GPIO.LOW) # Red

# Basic Window
class LEDControlApp(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('LED Control')
        self.setGeometry(100, 100, 500, 200)  # Set window position and size

        self.layout = QVBoxLayout()
        self.createLEDRows()

        self.setLayout(self.layout)

        # Set default checked state for "Off" buttons
        self.radio_red_off.setChecked(True)
        self.radio_green_off.setChecked(True)
        self.radio_blue_off.setChecked(True)

    def createLEDRows(self):
        # Red LEDs
        red_layout = QHBoxLayout()
        self.radio_red_on = QRadioButton('Red LED On')
        self.radio_red_off = QRadioButton('Red LED Off')
        
        self.button_group_red = QButtonGroup()
        self.button_group_red.addButton(self.radio_red_on, id=1)
        self.button_group_red.addButton(self.radio_red_off, id=2)
        
        red_layout.addWidget(self.radio_red_on)
        red_layout.addWidget(self.radio_red_off)
        self.layout.addLayout(red_layout)

        # Green LEDs
        green_layout = QHBoxLayout()
        self.radio_green_on = QRadioButton('Green LED On')
        self.radio_green_off = QRadioButton('Green LED Off')
        
        self.button_group_green = QButtonGroup()
        self.button_group_green.addButton(self.radio_green_on, id=3)
        self.button_group_green.addButton(self.radio_green_off, id=4)
        
        green_layout.addWidget(self.radio_green_on)
        green_layout.addWidget(self.radio_green_off)
        self.layout.addLayout(green_layout)

        # Blue LEDs
        blue_layout = QHBoxLayout()
        self.radio_blue_on = QRadioButton('Blue LED On')
        self.radio_blue_off = QRadioButton('Blue LED Off')
        
        self.button_group_blue = QButtonGroup()
        self.button_group_blue.addButton(self.radio_blue_on, id=5)
        self.button_group_blue.addButton(self.radio_blue_off, id=6)
        
        blue_layout.addWidget(self.radio_blue_on)
        blue_layout.addWidget(self.radio_blue_off)
        self.layout.addLayout(blue_layout)

        # Connect click events
        self.button_group_red.buttonClicked.connect(self.updateLEDState)
        self.button_group_green.buttonClicked.connect(self.updateLEDState)
        self.button_group_blue.buttonClicked.connect(self.updateLEDState)

    def updateLEDState(self, button):
        button_id = button.group().checkedId()
        
        if button_id == 1:  # Red LED On
            # Code to turn Red LED On
            print('Red LED On')
            self.button_group_red.button(2).setChecked(False)  # Uncheck Red LED Off
            GPIO.output(15, GPIO.HIGH)
            
        elif button_id == 2:  # Red LED Off
            # Code to turn Red LED Off
            print('Red LED Off')
            self.button_group_red.button(1).setChecked(False)  # Uncheck Red LED On
            GPIO.output(15, GPIO.LOW)
            
        elif button_id == 3:  # Green LED On
            # Code to turn Green LED On
            print('Green LED On')
            self.button_group_green.button(4).setChecked(False)  # Uncheck Green LED Off
            GPIO.output(13, GPIO.HIGH)
            
        elif button_id == 4:  # Green LED Off
            # Code to turn Green LED Off
            print('Green LED Off')
            self.button_group_green.button(3).setChecked(False)  # Uncheck Green LED On
            GPIO.output(13, GPIO.LOW)
            
        elif button_id == 5:  # Blue LED On
            # Code to turn Blue LED On
            print('Blue LED On')
            self.button_group_blue.button(6).setChecked(False)  # Uncheck Blue LED Off
            GPIO.output(11, GPIO.HIGH)
            
        elif button_id == 6:  # Blue LED Off
            # Code to turn Blue LED Off
            print('Blue LED Off')
            self.button_group_blue.button(5).setChecked(False)  # Uncheck Blue LED On
            GPIO.output(11, GPIO.LOW)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = LEDControlApp()
    window.show()
    sys.exit(app.exec_())
