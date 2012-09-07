import os
import array

FileImage = []
def enumFile(dirname):
	for root,_,files in os.walk(dirname):
		for fn in files:
			fname = dirname+os.sep+fn
			FileImage.append(fname)

enumFile("boss")
enumFile("images")
enumFile("sounds")
enumFile("middle")
enumFile("zako")
enumFile("ogg")

total_size = 0;
for fname in FileImage:
	total_size += os.path.getsize(fname)

print("#include \"file_data.h\"")

print("static const char ImageData[{0}] = ".format(total_size)+"{")
for fname in FileImage:
	with open(fname,"rb") as f:
		size = os.path.getsize(fname)
		dat = array.array('b')
		dat.fromfile(f,size)
		dat = dat.tolist()
		print(repr(dat).replace('[','').replace(']','')+",")
print("};")

print("const size_t gNumberOfFileImages={0};".format(len(FileImage)))
print("const struct FileEntry gFileImages[{0}] = ".format(len(FileImage))+"{")

offset = 0;
for fname in FileImage:
	with open(fname,"rb") as f:
		print("\t{");
		print("\t.filename=\""+fname+"\",")
		size = os.path.getsize(fname)
		print("\t.size={0},".format(size))
		print("\t.data=&ImageData[{0}],".format(offset))
		offset += size
		print("\t},");
		pass

print("""};
""")

