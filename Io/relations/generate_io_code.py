from xml.dom.minidom import parse, parseString, Document
import argparse
import sys
import glob 
from os import path

def GenerateSample():
    doc = Document()
    root = doc.createElement('root')
    type = doc.createElement('type')
    doc.appendChild(root)
    root.appendChild(type)
    type.setAttribute("name", "Population")
    type.setAttribute("pragma", "object")

    field = doc.createElement("field")
    type.appendChild(field)
    field.setAttribute("name", "person_id")
    field.setAttribute("key", "yes")

    field = doc.createElement("field")
    type.appendChild(field)
    field.setAttribute("name", "household")
   
    field.setAttribute("reference", "Household")

    field = doc.createElement("field")
    type.appendChild(field)
    field.setAttribute("name", "income")
    field.setAttribute("type", "float")
    field.setAttribute("index", "yes")
    field.setAttribute("unit", "USD")

    field = doc.createElement("field")
    type.appendChild(field)
    field.setAttribute("name", "name")
    field.setAttribute("type", "Name")  
    
    type = doc.createElement("type")
    type.setAttribute("name", "Name")
    type.setAttribute("pragma", "value")
    root.appendChild(type)
    field = doc.createElement("field")
    type.appendChild(field)
    field.setAttribute("name", "first_name")
    field.setAttribute("type", "string")
    
    field = doc.createElement("field")
    type.appendChild(field)
    field.setAttribute("name", "last_name")
    field.setAttribute("type", "string")

    
    xml_fh = open('population.xml','w')
    xml_fh.write(doc.toprettyxml(indent='\t',newl='\n'))    
    xml_fh.close()
    

    
def ParseFile(file_path):
    if not path.isfile(file_path):
        "File %s does not exist"%file_path
        return
    print "Parsing file: ", file_path
    (temp, file_name) = path.split(file_path)
    content = ""    
    header_file_name = file_name.split('.')[0].title()
    content+="""#ifndef %s
#define %s
#pragma warning(disable:4068)    
#include <vector>
#include <string>
#include <odb/core.hxx>
// Include TR1 <memory> header in a compiler-specific fashion. Fall back
// on the Boost implementation if the compiler does not support TR1.
//
#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

namespace %s {
//Forward declarations.
//
$ForwardDeclarations$
"""%(header_file_name, header_file_name, args.namespace)
    doc = parse(file_path)
    types = doc.getElementsByTagName("type")
    ForwardDeclarations = ""
    for type in types:
        t_name = type.getAttribute("name")
        ForwardDeclarations += "class %s;\n"%t_name
        pragma = type.getAttribute("pragma")
        fields = type.getElementsByTagName("field")
        content += """
#pragma db %s
class %s
{
public:
    // Default Constructor
    %s () {}        
"""%(pragma, t_name, t_name)
    
        constructor_1 = "\t%s ("%t_name
        constructor_2 = ": "
        accessors = "\t//Accessors\n"
        data_fields = "//Data Fields\nprivate:\n\tfriend class odb::access;\n"
 
        for field in fields:
            
            name = field.getAttribute("name")
            
            reference = field.getAttribute("reference")
            contanerSetter = field.getAttribute("contanerSetter")
            type = field.getAttribute("type")
            key = field.getAttribute("key")
            index = field.getAttribute("index")
            if reference!="":
                if contanerSetter!="no":
                    accessors += "\tvoid set%s (const %s& %s_, InputContainer& container) {%s = container.%ss[%s_];}\n"%(name.title(), type, name, name, reference, name)                
                type = "shared_ptr<%s>"%reference
            if key!="":
                if key=="yes":
                    data_fields += "\t#pragma db id\n"
                    accessors += "\tconst %s& getPrimaryKey () const {return %s;}\n"%(type, name) 
            if index!="":
                if index=="yes":
                    data_fields += "\t#pragma db index\n"
            constructor_1 += "%s %s_, "%(type, name)
            constructor_2 += "%s (%s_), "%(name,name)
            accessors += "\tconst %s& get%s () const {return %s;}\n"%(type,name.title(), name)
            accessors += "\tvoid set%s (const %s& %s_) {%s = %s_;}\n"%(name.title(), type, name, name, name)
            data_fields += "\t%s %s;\n"%(type,name)
        content += constructor_1[:-2] + ")\n\t" + constructor_2[:-2] + "\n\t{\n\t}\n"
        content += accessors
        content += data_fields
        content += "};\n"
    content += "#endif"
    content = content.replace("$ForwardDeclarations$", ForwardDeclarations)
    out_fn = args.output+"/"+header_file_name + ".h"
    print "Generated ", out_fn
    with open(out_fn,'w') as fh:
        fh.write(content)
            
    
    
    
    

app = argparse.ArgumentParser(description='Convert an xml file into a c++ code that is compatible with ODB')
app.add_argument("-s", "--sample", action="store_true", help="Generates a sample xml file")      
app.add_argument("-f", "--folder", default=".", help="Folder where xml files are located. Current folder by default")
app.add_argument("-o", "--output", default=".", help="Output directory")
app.add_argument("-n", "--namespace", default = "pio", help="Namespace for the classes")
app.add_argument("-i", "--input", nargs='*', help="Specific Fales to be parsed. If not specified, all of the xml files in the folder are processed")

args = app.parse_args()

if  args.sample:
    GenerateSample()
    print "A sample file %s was generated"%"population.xml"
    sys.exit()
if args.input is not None:
    for item in args.input:
        files.append(args.folder+"/item")
else:
    files = glob.glob('%s/*.xml'%str(args.folder))

for file in files:
    ParseFile(file)
    