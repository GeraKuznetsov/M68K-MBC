import serial
import time 
import sys
import threading
import math

sectorSize = 1024 * 4;
delay = 5/1000

file = "OS.bin"
com = "COM3"
bytes_read = open(file, "rb").read()
print('opened file: '+ file)
print ('useing '+ com + ' with rate of 500000')
ser = serial.Serial(com, 500000)

Run = True

def read_funct():
	print("hi")
	while Run:
		print(ser.readline())
		time.sleep(1/1000)

time.sleep(0.01)
t1 = threading.Thread(target=read_funct, args=())
t1.start()

time.sleep(3)
ser.write(bytes('i\n', 'ascii'))
time.sleep(1)
#ser.write(bytes('c\n', 'ascii'))
time.sleep(1)
#for x in range(math.ceil(len(bytes_read)/sectorSize)):
	#ser.write(bytes('e ' + '{:04x}'.format(x * sectorSize) + '\n', 'ascii'))
	#time.sleep(delay)
ser.write(bytes('d 0\n', 'ascii'))

i = 0
first = True
for b in bytes_read:
	if i % sectorSize == 0:
		print(i);
	if i % 16 == 0:
		if not(first):
			ser.write(bytes('\n', 'ascii'))
			#time.sleep(0.3/1000)
			time.sleep(50/1000)
		first = False
		toWrite = 16
		left = len(bytes_read) - i
		if left < 16:
			toWrite = left
		ser.write(bytes("w " + '{:05x}'.format(i) + ' '  + '{:01x}'.format(toWrite) + ' ', 'ascii'))
	ser.write(bytes('{:02x}'.format(b), 'ascii'))
	i += 1
	
ser.write(bytes('\n', 'ascii'));

time.sleep(delay)
Run = False
#for x in range(525*64//16):
#ser.write(bytes("r " + '{:04x}'.format(x * 16) + ' ' + '{:02x}'.format(16) + '\n', 'ascii'))
#time.sleep(delay)