import smbus
import time
import MySQLdb as mdb

bus = smbus.SMBus(1)
address = 0x04

def readByteArray(lenght):
    bytes = bus.read_i2c_block_data(address,0,lenght)
    return bytes

def writeBytes(cmd, bytes):
    bus.write_i2c_block_data(address, cmd, bytes)
    return -1

def insertTempDB(temperature):
	con = mdb.connect('victorbarcelo.net', 'victorba_igni941', 'f(hP6(92S5', 'victorba_igni941')
	with con:
		cur = con.cursor()
		cur.execute("INSERT INTO temperatures (temperature,time) VALUES (%s,%s)", (temperature,int(time.time())))

def insertLightDB(light):
    con = mdb.connect('victorbarcelo.net', 'victorba_igni941', 'f(hP6(92S5', 'victorba_igni941')
    with con:
        cur = con.cursor()
        cur.execute("INSERT INTO lights (light,time) VALUES (%s,%s)", (light,int(time.time())))

while True:
    bytes = readByteArray(4) # Arduino -> Wire.onRequest(sendData);
    temperature = bytes[0] + (bytes[1] << 8)
    light = bytes[2] + (bytes[3] << 8)
    insertTempDB(temperature)
    insertLightDB(1024 - light)
    print "Arduino -> RPI [Temp]: ", temperature
    print "Arduino -> RPI [Light]: ", 1024 - light
    print
    time.sleep(15)