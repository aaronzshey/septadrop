import os

RES_DIR = "../res/"
RES_FILE = "../include/res.hpp"

resources = os.fsencode(RES_DIR)
res = ""

for resource in os.listdir(resources):
	encoded = f"const unsigned char {os.path.splitext(resource)[0].decode().upper()}[] = {'{'}"
	file = open(RES_DIR + resource.decode(), "rb") # read binary
	bytes_processed = 0
	for byte in file.read():
		if bytes_processed % 16 == 0:
			encoded += "\n\t"
		encoded += f"0x{byte.to_bytes(1, byteorder='little').hex()}, "
		bytes_processed += 1
	encoded = encoded[:-2] + "\n};"
	res += encoded + "\n"

res_file = open(RES_FILE, "w")
res_file.write(res)
res_file.close()