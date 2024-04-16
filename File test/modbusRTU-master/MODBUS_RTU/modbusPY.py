import serial
from pymodbus.client.sync import ModbusSerialClient as ModbusClient
serialCom = ModbusClient(method='rtu', port='COM1', timeout=1, stopbits=1, bytesize=8, parity='N', baudrate=9600)

while serialCom.connect():
    try:
        res = serialCom.read_holding_registers(address=19, count=1, unit=1)

        if not res.isError():
            rawData = int(res.registers[0])
            print rawData
        else:
            continue
    except ValueError:
        print("no or odd data received\n")
        continue