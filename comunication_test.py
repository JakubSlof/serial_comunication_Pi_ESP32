#program na ozkouseni seriove komunikace mezi raspberry pi a esp 32
# import knihoven  
import serial
import time 

port = 'COM3' #nazev portu kde je esp32 pripojeno, porte se musi zmenit pri zmene portu 
baund_rate = 115200 # rychlost seriove komunikace 
ser = serial.Serial(port, baund_rate,timeout=1) # nastaveni parametru komunikace 
time.sleep(3)#cas nez se esp zapne a zacne fungovat 
ser.reset_input_buffer()# smaze se vse co mohlo prijit drive 
print("serial ok") # pokud vse fungujevipise serial ok 
ser.close # uzavreni seriove komunikace 
