import os

RES_DIR = "../res/"
PACKED_DIR = "../include/packed/"

resources = os.fsencode(RES_DIR)
os.makedirs(PACKED_DIR, exist_ok=True)
packed_dir = os.fsencode(PACKED_DIR)

for file in os.listdir(packed_dir):
	if os.path.splitext(file)[1] == b".hpp":
		os.unlink(os.path.join(packed_dir, file))

for resource in os.listdir(resources):
	res = ""

	splitext = os.path.splitext(resource)
	name = splitext[0].decode()
	ext = splitext[1].decode()
	
	if ext == ".png":
		name += "_texture"
	elif ext == ".wav":
		name += "_audio"
	name += "_data"

	encoded = f"const unsigned char {name.upper()}[] = {'{'}"
	file = open(os.path.join(resources, resource), "rb") # read binary
	bytes_processed = 0
	for byte in file.read():
		if bytes_processed % 16 == 0:
			encoded += "\n\t"
		encoded += f"0x{byte.to_bytes(1, byteorder='little').hex()}, "
		bytes_processed += 1
	encoded = encoded[:-2] + "\n};"
	res += encoded + "\n"
	with open(os.path.join(PACKED_DIR, f"{name}.hpp"), "w") as f:
		f.write(res)