from xml.dom.minidom import parse, parseString, Document
import argparse
import sys
import glob 
from os import path
import re

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
    field.setAttribute("pragma", "id")

    field = doc.createElement("field")
    type.appendChild(field)
    field.setAttribute("name", "household")
   
    field.setAttribute("type", "shared_ptr<Household>")

    field = doc.createElement("field")
    type.appendChild(field)
    field.setAttribute("name", "income")
    field.setAttribute("type", "float")
    field.setAttribute("pragma", "index")

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
    field.setAttribute("type", "std::string")
    
    field = doc.createElement("field")
    type.appendChild(field)
    field.setAttribute("name", "last_name")
    field.setAttribute("type", "string")

    
    xml_fh = open('population.xml','w')
    xml_fh.write(doc.toprettyxml(indent='\t',newl='\n'))    
    xml_fh.close()
    

def my_singularize(word):
    """Return the singular form of a word
 
    &gt;&gt;&gt; singularize('rabbits')
    'rabbit'
    &gt;&gt;&gt; singularize('potatoes')
    'potato'
    &gt;&gt;&gt; singularize('leaves')
    'leaf'
    &gt;&gt;&gt; singularize('knives')
    'knife'
    &gt;&gt;&gt; singularize('spies')
    'spy'
    """
    sing_rules = [lambda w: w[-3:] == 'ies' and w[:-3] + 'y',
                  lambda w: w[-4:] == 'ives' and w[:-4] + 'ife',
                  lambda w: w[-3:] == 'ves' and w[:-3] + 'f',
                  lambda w: w[-2:] == 'es' and w[:-2],
                  lambda w: w[-1:] == 's' and w[:-1],
                  lambda w: w,
                  ]
    word = word.strip()
    singleword = [f(word) for f in sing_rules if f(word) is not False][0]
    return singleword

    
p_ptr1 = re.compile("\w+(\_ptr|\s*\*)") 
p_ptr = re.compile("\A\s*\w+(\_ptr|\s*\*)") 
# p_container = re.compile("\A\s*(std::)?(vector|set|map)\s*<")   
p_container = re.compile("\A\s*(std::)?(vector|set)\s*\<(\s*.+\s*)\>")   
 
 
 
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
using std::tr1::weak_ptr;

namespace %s {
//Forward declarations.
//
$ForwardDeclarations$
"""%(header_file_name, header_file_name, args.namespace)
    doc = parse(file_path)
    types = doc.getElementsByTagName("type")
    ForwardDeclarations = ""
    for obj_type in types:
        t_name = obj_type.getAttribute("name")
        ForwardDeclarations += "class %s;\n"%t_name
        pragma = obj_type.getAttribute("pragma")
        fields = obj_type.getElementsByTagName("field")
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
        constructor_3 = "\t%s ("%t_name
        constructor_4 = ": "   
        include_constractor_2 = False         
        accessors = "\t//Accessors\n"
        data_fields = "\t//Data Fields\nprivate:\n\tfriend class odb::access;\n"
        pragmas = ""
 
        for field in fields:            
            name = field.getAttribute("name")            
            type = field.getAttribute("type")
            pragma = field.getAttribute("pragma")   
            pointer_type = (p_ptr.search(type) is not None)
            has_pointer = (p_ptr1.search(type) is not None)
            container_type = (p_container.search(type) is not None)            
            if pointer_type:
                accessor_type = type
            else:
                accessor_type = type + "&"
            constructor_1 += "%s %s_, "%(type, name)
            constructor_2 += "%s (%s_), "%(name,name)
            if has_pointer:
                include_constractor_2 = True
            else:
                constructor_3 += "%s %s_, "%(type, name)
                constructor_4 += "%s (%s_), "%(name,name)            
            accessors += "\tconst %s get%s () const {return %s;}\n"%(accessor_type,name.title(), name)
            accessors += "\tvoid set%s (const %s %s_) {%s = %s_;}\n"%(name.title(), accessor_type, name, name, name)
            if container_type:
                m = p_container.search(type)
                accessors += "\tvoid set%s (const %s %s_) {%s.push_back(%s_);}\n"%(my_singularize(name).title(), m.group(3), name, name, name)
            if pragma!="":
                data_fields += "\t#pragma db %s\n"%(pragma)
            data_fields += "\t%s %s;\n"%(type,name)
        content += constructor_1[:-2] + ")\n\t" + constructor_2[:-2] + "\n\t{\n\t}\n"
        if include_constractor_2:
            content += constructor_3[:-2] + ")\n\t" + constructor_4[:-2] + "\n\t{\n\t}\n"
        content += accessors
        content += data_fields
        content += pragmas        
        content += "};\n"
    content += "}}\n#endif"
    content = content.replace("$ForwardDeclarations$", ForwardDeclarations)
    out_fn = args.output+"/"+header_file_name + ".h"
    print "Generated ", out_fn
    with open(out_fn,'w') as fh:
        fh.write(content)
            
    
    
    
    
if __name__ == "__main__":
    app = argparse.ArgumentParser(description='Convert an xml file into a c++ code that is compatible with ODB')
    app.add_argument("-s", "--sample", action="store_true", help="Generates a sample xml file")      
    app.add_argument("-f", "--folder", default=".", help="Folder where xml files are located. Current folder by default")
    app.add_argument("-o", "--output", default=".", help="Output directory")
    app.add_argument("-n", "--namespace", default = "polaris{ namespace io", help="Namespace for the classes")
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
    