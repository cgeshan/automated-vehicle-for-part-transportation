import serial

ser = serial.Serial('COM5', 115200, timeout = 1)

x = [1,2,3,4,5]

while len(x) >= 0:
  ser.write(bytes(x[0], 'utf-8'))
  time.sleep(1)  
  status = ser.readline(1024)
  if !status:
      time.sleep(1)
  else: 
    print(status)
    x.pop(0)
      
