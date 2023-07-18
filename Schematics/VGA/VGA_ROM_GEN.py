import struct

file = "out.bin"
output = open(file, "wb")

HSYNC = 1 #NEG
VSYNC = 2 #NEG
RESX = 4  #NEG
RESY = 8  #NEG
INCY = 16 
VIS = 32  #NEG

NEGATE = HSYNC | VSYNC | VIS | RESX | RESY

H_VIS = 640 / 16
H_FP = 16 / 16
H_SYNC = 96 / 16
H_BP = 48 / 16 

Y_VIS = 480
Y_FP = 10
Y_SYNC = 2
Y_BP = 33

for y in range(1024):
	for x in range(64):
		b = 0
		if x < H_VIS and y < Y_VIS:
			b |= VIS
			
		if x >= H_VIS + H_FP and x < H_VIS + H_FP + H_SYNC:
			b |= HSYNC
		if x == H_VIS + H_FP + H_SYNC + H_BP - 2:
			b |= INCY
		if x >= H_VIS + H_FP + H_SYNC + H_BP - 1:
			b |= RESX
			if y == Y_VIS + Y_FP + Y_SYNC + Y_BP - 1:
				b |= RESY
			else:
				b |= INCY
			
		if y >= Y_VIS + Y_FP and y < Y_VIS + Y_FP + Y_SYNC:
			b |= VSYNC
			
		b ^= NEGATE
		output.write(struct.pack('1B', b))
		output.write(struct.pack('1B', b))
		
H_VIS = (640 / 16) - 8
H_FP = (16 / 16) + 4
H_SYNC = (96 / 16)
H_BP = (48 / 16) + 4
		
Y_VIS = 480 / 2
		
for y in range(1024):
	for x in range(64):
		b = 0
		if x < H_VIS and y < Y_VIS:
			b |= VIS
			
		if x >= H_VIS + H_FP and x < H_VIS + H_FP + H_SYNC:
			b |= HSYNC
			
		if x == H_VIS + H_FP + H_SYNC + H_BP - 2 and y >= Y_VIS:
			b |= INCY
			
		if x >= H_VIS + H_FP + H_SYNC + H_BP - 1:
			b |= RESX
			if y == Y_VIS + Y_FP + Y_SYNC + Y_BP - 1:
				b |= RESY
			else:
				b |= INCY
			
		if ((y == Y_VIS + Y_FP - 1 and x >= 50-4) or (y == Y_VIS + Y_FP) or (y == Y_VIS + Y_FP + 1 and x < 4)):
			b |= VSYNC
			
		b ^= NEGATE
		output.write(struct.pack('1B', b))
		output.write(struct.pack('1B', b))
		
print("done")