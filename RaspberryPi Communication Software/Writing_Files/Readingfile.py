#Envoi d'une donnée lu de la command window vers le moniteur de l'arduino et réception des données

import serial
import time
import csv


line = ""


if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', baudrate = 9600, timeout=1)
    ser.flush()
    while True:

        if (ser.in_waiting > 0):
            line = ser.readline().decode('utf-8').rstrip()

            if (line == "Ready to receive points"):
                with open('POINTS_FILE.csv') as Points:
                    points_reader = csv.reader(Points)

                    for row in points_reader:
                        row_encode = row.encode('utf-8')
                        ser.write(Data_encode)
                    ser.flush()
                    time.sleep(.1)
                Points.close()
                
            while(ser.in_waiting > 0):
                line = ser.readline().decode('utf-8').rstrip()
                print(line)            

    
