import json
import os

nmf = {
	"program": {
		"x86-64": {"url": "noiz2sa_64.nexe"},
		"x86-32": {"url": "noiz2sa_32.nexe"}
	},
	"files": {
	}
}

def enumFile(dirname):
	for root,_,files in os.walk(dirname):
		for f in files:
			fname = dirname+os.sep+f
			nmf["files"][fname]={
				"x86-64": {"url": fname},
				"x86-32": {"url": fname}
			}

enumFile("boss")
enumFile("images")
enumFile("sounds")
enumFile("middle")
enumFile("ogg")

print(json.dumps(nmf, sort_keys=True, indent=4))

