import serial
from serial.tools.list_ports import comports

ports = list(comports())

for p in ports:
    if 'wchusbserial' in p.device:
        print(p.device, p.description, p.serial_number)