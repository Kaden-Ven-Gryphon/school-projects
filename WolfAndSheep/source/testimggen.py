# create a simple text file
img_bytes = bytearray()

width = 256
hight = 256

img_bytes.extend(width.to_bytes(4, 'little'))
img_bytes.extend(hight.to_bytes(4, 'little'))


for x in range(0,256):
	for y in range(0,256):
		img_bytes.append(x)
		img_bytes.append(y)
		img_bytes.append(0)
		img_bytes.append(255)

immutable_bytes = bytes(img_bytes)

with open("testimg.bin", "wb") as binary_file:
	binary_file.write(immutable_bytes)
