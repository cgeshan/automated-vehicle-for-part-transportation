import serial

ser = serial.Serial('COMM4', 9600, timeout = 1)

x = [1,2,3,4,5]
ser.write(x)