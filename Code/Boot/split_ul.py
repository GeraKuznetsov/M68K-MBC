print("splitting file")
with open("OS.bin", mode='rb') as source: # b is important -> binary
    fileContent = source.read()

upper = open("OS_upper.bin", "wb")
lower = open("OS_lower.bin", "wb")

location = 0
for n in fileContent:
	if location == 0:
		upper.write(bytes([n]))
		location = 1
	else :
		lower.write(bytes([n]))
		location = 0
if location == 1:
	lower.write(bytes([0]))