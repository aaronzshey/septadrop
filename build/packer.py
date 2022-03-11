import os, shutil

RES_DIR = "../res/"
PACKED_DIR = "../include/packed/"

resources = os.fsencode(RES_DIR)
os.makedirs(PACKED_DIR, exist_ok=True)
packed_dir = os.fsencode(PACKED_DIR)

for filename in os.listdir(packed_dir):
	file_path = os.path.join(packed_dir, filename)
	if os.path.isfile(file_path) or os.path.islink(file_path):
		os.unlink(file_path)
	elif os.path.isdir(file_path):
		shutil.rmtree(file_path)

for folder in os.listdir(resources):
	folder_path = os.path.join(resources, folder)
	if os.path.isfile(folder_path):
		continue
	packed_path = os.path.join(packed_dir, folder)
	os.makedirs(packed_path, exist_ok=True)
	for resource in os.listdir(folder_path):
		resource_path = os.path.join(folder_path, resource)

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
		file = open(resource_path, "rb") # read binary
		bytes_processed = 0
		for byte in file.read():
			if bytes_processed % 16 == 0:
				encoded += "\n\t"
			encoded += f"0x{byte.to_bytes(1, byteorder='little').hex()}, "
			bytes_processed += 1
		encoded = encoded[:-2] + "\n};"
		res += encoded + "\n"
		with open(os.path.join(packed_path.decode(), f"{name}.hpp"), "w") as f:
			f.write(res)