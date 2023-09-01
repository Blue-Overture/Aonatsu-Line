import glob
import os
import sys
from pathlib import Path

files = glob.glob(f"{sys.argv[1]}/*.txt")

os.makedirs("Repacked", exist_ok=True)

for i in range(len(files)):
	print(Path(files[i]).stem)
	file = open(files[i], "r", encoding="UTF-8")

	DUMP = []

	for line in file:
		args = line.rstrip().split("\t")
		if len(args) < 2:
			continue
		if args[0][0:1] == ";":
			continue
		if args[0][0:1] != "{":
			print("No offset data detected! Aborting...")
			print(args[0])
			sys.exit()
		entry = {}
		entry["OFFSET"] = args[0]
		entry["LINE"] = line.rstrip()
		DUMP.append(entry)
	
	file.close()

	DATA = []
	script = open(f"{sys.argv[2]}/{Path(files[i]).stem}.asm", "r", encoding="UTF-8")

	Itr = 0

	count = len(DUMP)

	if len(DUMP) > 0:
		for line in script:
			args = line.rstrip().split("\t")
			if (len(args) < 2) or (Itr == count):
				DATA.append(line.rstrip())
			elif args[0] == DUMP[Itr]["OFFSET"]:
				DATA.append(DUMP[Itr]["LINE"])
				Itr += 1
			else:
				DATA.append(line.rstrip())
	else:
		DATA.append(script.read())
	script.close()

	new_file = open(f"Repacked/{Path(files[i]).stem}.asm", "w", encoding="UTF-8")
	new_file.write("\n".join(DATA))
	new_file.close()

	

		