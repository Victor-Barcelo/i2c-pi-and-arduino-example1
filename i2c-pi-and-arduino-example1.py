import smbus
import time
import struct

bus = smbus.SMBus(1)
address = 0x04

def readBytes(lenght):
    bytes = bus.read_i2c_block_data(address,0,lenght)
    return bytes

def writeBytes(cmd, bytes):
    bus.write_i2c_block_data(address, cmd, bytes)
    return -1

def setDatetimeArduino():
    actualTime = int(time.time())
    bytesTuple = struct.unpack("4B", struct.pack("I", actualTime))
    bytes = []
    for item in bytesTuple:
    	bytes.append(item)
    cmd = 0x01 # our command to set datetime in Arduino
    writeBytes(cmd,bytes)

def readFromArduino():
    bytes = readBytes(4)
    temperature = bytes[0] + (bytes[1] << 8)
    light = bytes[2] + (bytes[3] << 8)
    print "Arduino -> RPI [Temp]: ", temperature
    print "Arduino -> RPI [Light]: ", light
    print

setDatetimeArduino()
while True:
    time.sleep(2)
    readFromArduino()