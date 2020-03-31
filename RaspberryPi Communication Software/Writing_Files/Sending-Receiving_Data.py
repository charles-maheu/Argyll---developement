#Envoi d'une donnée lu de la command window vers le moniteur de l'arduino et réception des données

import serial
import time
import csv


line = ""


if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', baudrate = 9600, timeout=1)
    ser.flush()
    while True:
        Data = input()
        Data = Data + "\n"
        Data_encode = Data.encode('utf-8')
        ser.write(Data_encode)
        ser.flush()
        time.sleep(.1)
        
        if (ser.in_waiting > 0):
            line = ser.readline().decode('utf-8').rstrip()
            
            if (line != '<'):
                print('Error')
                
            if (line == '<'):
                line = ser.readline().decode('utf-8').rstrip()
                
                if (line == '!'):
                    PointsFile = open('Points.csv', 'w')
                    PointsFile.write('!\n')
                    
                    while(line !='>'):
                        line = ser.readline().decode('utf-8').rstrip()
                        PointsFile.write(line+'\n')
                    PointsFile.close()
                    print("Writing Points Done")

                if (line == '&'):
                    ProgFile = open('Prog.csv', 'w')
                    ProgFile.write('&\n')
                    while(line !='>'):
                        line = ser.readline().decode('utf-8').rstrip()
                        ProgFile.write(line+'\n')
                    ProgFile.close()
                    print("Writing Prog Done")
            line = ""
            ser.flush();
    time.sleep(.1)

  
            
            


