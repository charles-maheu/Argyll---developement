# For Saving and Receiving/Sending Data
import json
import serial
import time

# For Uart Connection
import sys
import dbus, dbus.mainloop.glib
from gi.repository import GLib
from example_advertisement import Advertisement
from example_advertisement import register_ad_cb, register_ad_error_cb
from example_gatt_server import Service, Characteristic
from example_gatt_server import register_app_cb, register_app_error_cb
 
BLUEZ_SERVICE_NAME =           'org.bluez'
DBUS_OM_IFACE =                'org.freedesktop.DBus.ObjectManager'
LE_ADVERTISING_MANAGER_IFACE = 'org.bluez.LEAdvertisingManager1'
GATT_MANAGER_IFACE =           'org.bluez.GattManager1'
GATT_CHRC_IFACE =              'org.bluez.GattCharacteristic1'
UART_SERVICE_UUID =            '6e400001-b5a3-f393-e0a9-e50e24dcca9e'
UART_RX_CHARACTERISTIC_UUID =  '6e400002-b5a3-f393-e0a9-e50e24dcca9e'
UART_TX_CHARACTERISTIC_UUID =  '6e400003-b5a3-f393-e0a9-e50e24dcca9e'
LOCAL_NAME =                   'Robot Arm'
mainloop = None

# Joint position data file name
jointPositionsFile = "/home/pi/PROJET_ARGYLL/ble-uart-peripheral/data.json"

# Serial setup
ser = serial.Serial('/dev/ttyACM0', baudrate = 9600 , timeout=1)
# ser.flush()

def SendingToOpenCr(self,data):
    Data_encode = data.encode('utf-8')
    ser.write(Data_encode)
    ser.flush()
    time.sleep(.01)

    while (ser.in_waiting > 0):

        line = ser.readline().decode('utf-8').rstrip()

        if (line[0] == "<"):        
            print("From OpenCr: "+line)
            send.send_tx(line)
            time.sleep(0.01)                                                        

        if (line == "["):
            print("Receiving and saving joint positions")
            data = {} # Init data dict
            data['jointPositions'] = []
            while True:
                line = ser.readline().decode('utf-8').rstrip() # Read serial input until '\n'
                if (line == "]") or (line.find("{") == -1):
                    break
                lineArray = line.strip("{}").split(':') # Removes brackets and splits on ':'
                data['jointPositions'].append({'positionID' : lineArray[0],'positionName' : lineArray[1],'joints' : lineArray[2].split(',')})
                with open(jointPositionsFile, 'w') as outfile:
                    json.dump(data, outfile, indent=3)
            print("End of saving joint positions")
            outfile.close()

        if (line == "ReadyToSetup"):
            print("Sending all data from the JSON file to the Arduino")
            with open(jointPositionsFile) as infile:
                data = json.load(infile)
                print("[")
                Data = "["
                ser.write(Data.encode('utf-8'))
                for jPos in data["jointPositions"]:
                    Data = "{"+jPos["positionID"] + ":" + jPos["positionName"] + ":" + str(jPos["joints"]).replace("'","").strip("[]").replace(" ","")+",}"+"\n"
                    Data_encode = Data.encode('utf-8')
                    ser.write(Data_encode)
                    print("{"+jPos["positionID"] + ":" + jPos["positionName"] + ":" + str(jPos["joints"]).replace("'","").strip("[]").replace(" ","")+",}")
                print("]")
                Data = "]"
                ser.write(Data.encode('utf-8'))
                while (ser.in_waiting > 0):
                    line = ser.readline().decode('utf-8').rstrip()
                    print(line)
                    time.sleep(0.01)
            infile.close()       
    print("")
            
    


 
class TxCharacteristic(Characteristic):

    def __init__(self, bus, index, service):
        Characteristic.__init__(self, bus, index, UART_TX_CHARACTERISTIC_UUID,
                                ['notify'], service)
        self.notifying = False
        global send
        send = self
        GLib.io_add_watch(sys.stdin, GLib.IO_IN, self.on_console_input)

 
    def on_console_input(self, fd, condition):
        s = fd.readline()
        if s.isspace():
            pass
        else:
            SendingToOpenCr(self,s)
            self.send_tx(s)
            return True
 
    def send_tx(self, s):
        if not self.notifying:
            return
        value = []
        for c in s:
            value.append(dbus.Byte(c.encode()))
        self.PropertiesChanged(GATT_CHRC_IFACE, {'Value': value}, [])
                 
 
    def StartNotify(self):
        if self.notifying:
            return
        self.notifying = True
 
    def StopNotify(self):
        if not self.notifying:
            return
        self.notifying = False



    def send(obj):
        s = obj
        global send
        value=[]
        for c in s:
            value.append(dbus.Byte(c.encode()))
        self.PropertiesChanged(GATT_CHRC_IFACE, {'Value': value}, [])


 
class RxCharacteristic(Characteristic):
    def __init__(self, bus, index, service):
        Characteristic.__init__(self, bus, index, UART_RX_CHARACTERISTIC_UUID,
                                ['write'], service)



        

# Sending/Receiving Data To/From OpenCr and Android     
 
    def WriteValue(self, value, options):
        self.line = ""
        Data = format(bytearray(value).decode()) #+ "\n"
        print("From Android: "+Data)
        SendingToOpenCr(self,Data)
        

 
class UartService(Service):
    def __init__(self, bus, index):
        Service.__init__(self, bus, index, UART_SERVICE_UUID, True)
        self.add_characteristic(TxCharacteristic(bus, 0, self))
        self.add_characteristic(RxCharacteristic(bus, 1, self))
        
 
class Application(dbus.service.Object):
    def __init__(self, bus):
        self.path = '/'
        self.services = []
        dbus.service.Object.__init__(self, bus, self.path)
 
    def get_path(self):
        return dbus.ObjectPath(self.path)
 
    def add_service(self, service):
        self.services.append(service)
 
    @dbus.service.method(DBUS_OM_IFACE, out_signature='a{oa{sa{sv}}}')
    def GetManagedObjects(self):
        response = {}
        for service in self.services:
            response[service.get_path()] = service.get_properties()
            chrcs = service.get_characteristics()
            for chrc in chrcs:
                response[chrc.get_path()] = chrc.get_properties()
        return response
 
class UartApplication(Application):
    def __init__(self, bus):
        Application.__init__(self, bus)
        self.add_service(UartService(bus, 0))
 
class UartAdvertisement(Advertisement):
    def __init__(self, bus, index):
        Advertisement.__init__(self, bus, index, 'peripheral')
        self.add_service_uuid(UART_SERVICE_UUID)
        self.add_local_name(LOCAL_NAME)
        self.include_tx_power = True
 
def find_adapter(bus):
    remote_om = dbus.Interface(bus.get_object(BLUEZ_SERVICE_NAME, '/'),
                               DBUS_OM_IFACE)
    objects = remote_om.GetManagedObjects()
    for o, props in objects.items():
        if LE_ADVERTISING_MANAGER_IFACE in props and GATT_MANAGER_IFACE in props:
            return o
        print('Skip adapter:', o)
    return




# FOR SAVING ON THE START

def StartupSaving():

    line = ""

    #Data = "<ReadyToReset>"
    #Data_encode = Data.encode('utf-8')
    #ser.write(Data_encode)
    #ser.flush()
    #time.sleep(.01)


    #while (line!="ReadyToSetup"):
    line = ser.readline().decode('utf-8').rstrip()
    
    if (line == "ReadyToSetup"):
            print("Sending all data from the JSON file to the Arduino")
            with open(jointPositionsFile) as infile:
                data = json.load(infile)
                print("[")
                Data = "["
                ser.write(Data.encode('utf-8'))
                for jPos in data["jointPositions"]:
                    Data = "{"+jPos["positionID"] + ":" + jPos["positionName"] + ":" + str(jPos["joints"]).replace("'","").strip("[]").replace(" ","")+",}"+"\n"
                    Data_encode = Data.encode('utf-8')
                    ser.write(Data_encode)
                    print("{"+jPos["positionID"] + ":" + jPos["positionName"] + ":" + str(jPos["joints"]).replace("'","").strip("[]").replace(" ","")+",}")
                print("]")
                Data = "]"
                ser.write(Data.encode('utf-8'))
                while (ser.in_waiting > 0):
                    line = ser.readline().decode('utf-8').rstrip()
                    print(line)
                    time.sleep(0.01)
            infile.close()



    
    

    
def main():

    StartupSaving()
    #Data = "<ReadyToUse>"
    #ser.write(Data.encode('utf-8')) 
    
    global mainloop
    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
    bus = dbus.SystemBus()
    adapter = find_adapter(bus)
    if not adapter:
        print('BLE adapter not found')
        return
 
    service_manager = dbus.Interface(
                                bus.get_object(BLUEZ_SERVICE_NAME, adapter),
                                GATT_MANAGER_IFACE)
    ad_manager = dbus.Interface(bus.get_object(BLUEZ_SERVICE_NAME, adapter),
                                LE_ADVERTISING_MANAGER_IFACE)
 
    app = UartApplication(bus)
    adv = UartAdvertisement(bus, 0)
 
    mainloop = GLib.MainLoop()
 
    service_manager.RegisterApplication(app.get_path(), {},
                                        reply_handler=register_app_cb,
                                        error_handler=register_app_error_cb)
    ad_manager.RegisterAdvertisement(adv.get_path(), {},
                                     reply_handler=register_ad_cb,
                                     error_handler=register_ad_error_cb)

    try:
        mainloop.run()
    except KeyboardInterrupt:
        adv.Release()

    
 
if __name__ == '__main__':
    main()
