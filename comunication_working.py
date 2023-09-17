import serial
import time

port = 'COM5' # port where is esp connecded 
baund_rate = 115200 # baund rate must be same in c++ code on esp!!!
esp = serial.Serial(port,baund_rate,timeout=1)

#sending data just one number 
def send_data(nunmber):
    command = f"{nunmber}\n" # change number to string 
    esp.write(command.encode('utf-8')) #write into serial using utf-8
    print('data send') #print data send to be sure it works 

#reciving data from serial 
def receve_data():
    line = 0 # line is zero becouse I wait for 69
    while True:
        line= esp.readline().decode('utf-8').strip() #reads line and saves it into variable line 
        print(line)
        if line == '69': #if its 69 breakes the loop
            break
        time.sleep(1)# delay so the while loop is not that fast 
    print('receved data')# print recived data to be sure it works 

send_data(2)
receve_data()
send_data(2)
receve_data()
send_data(2)
receve_data()
send_data(2)
receve_data()