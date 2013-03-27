import os, sys
from stat import *
import re

props = dict()
files = []
with open("db-inventory.txt") as fh:
    for line in fh:
        files.append(line.strip("\n"))

props["revision"] = 0
for item in files:
    props[item] =  0


with open("schema_revision.txt") as fh:
    props["revision"] = int(fh.readline().split("=")[1])
    for line in fh:
        value = int(line.split("=")[1])
        name = line.split("=")[0]        
        props[name] =  value
        
for item in files:
    # print item
    # print int(os.stat(item+".h")[ST_MTIME])
    # print props[item]
    if int(os.stat(item+".h")[ST_MTIME]) > props[item]: 
        props[item] = int(os.stat(item+".h")[ST_MTIME])
        props["revision"]= props["revision"]+1
        break;
        
for item in files:
    props[item] = int(os.stat(item+".h")[ST_MTIME])       
with open("schema_revision.txt", 'w') as fh:
    fh.write("revision="+str(props["revision"])+"\n")
    for item in files:
        fh.write(item+"="+str(props[item])+"\n")
        

p = re.compile("(#define\s+SCHEMA_REVISION\s+)(\"\d+\")")

with open("database.h") as fh:
    content = fh.read()  
new_content = p.sub("\g<1>"+"\""+str(props["revision"])+"\"", content)

with open("database.h", 'w') as fh:
    fh.write(new_content)