#Envoi d'une donnée lu de la command window vers le moniteur de l'arduino et réception des données

import serial
import time
import csv


line = ""

point= ['0','0','0']
Points = []
pos = ['0','0','0','0','0','0','0','0','0']

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
            
            #FOR POINTS            
            if (line == '!'):
                with open('POINTS_FILE.csv','w') as point_file:
                    fieldnames = ['Number', 'Name' , 'Position']
                    points_writer = csv.DictWriter(point_file, fieldnames=fieldnames, delimiter = '|')
                    points_writer.writeheader()
                    while(line != '>'): 
                        for x in range(0,2):
                            line = ser.readline().decode('utf-8').rstrip()
                            point[x] = line
                        line = ser.readline().decode('utf-8').rstrip()
                        point[2] = line
                        line = ser.readline().decode('utf-8').rstrip()
                        i=0
                        while (line!='!'):
                            point[2] = point[2]+' '+line
                            line = ser.readline().decode('utf-8').rstrip() 
                            i=i+1
                            if (line=='>'):
                                break
                        points_writer.writerow({'Number':point[0],'Name':point[1],'Position':point[2]})
                        #print(*point)
                point_file.close();
                    
                        

  
            
            


