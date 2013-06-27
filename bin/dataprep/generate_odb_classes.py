import re
import sys
import os	

def generate(cpp_path, transims_class_name, ref_flag=True, polaris_class_name=None):
	fields = []
	types = []
	accessors = []
	if polaris_class_name is None:
		polaris_class_name = transims_class_name
	#populate fields, types, accessors
	with open(cpp_path.replace("\\","/")) as fh:
		for line in fh:
			m = file_class_member_p.search(line)
			if m is not None:
				types.append(m.group(1))
				fields.append(m.group(3))
				accessors.append(m.group(2))
	nscn = "%s::%s"%(odb_namespace,polaris_class_name)		
	# create constructor
	#First part of the constructor
	constructor1 = "%s ( "%polaris_class_name
	#Second part of the constructor
	constructor2 = " \n\t: "
	#Adapter from transims type to polaris
	if ref_flag:
		adpater_method = "shared_ptr<%s> Adapter( %s_File &file, %s::%s& container) \n{\n\tshared_ptr<%s> result (new %s ());"%(nscn, transims_class_name, odb_namespace,container_type, nscn, nscn)
	else:
		adpater_method = "shared_ptr<%s> AdapterNoRef( %s_File &file, %s::%s& container) \n{\n\tshared_ptr<%s> result (new %s ());"%(nscn, transims_class_name, odb_namespace,container_type, nscn, nscn)
	members = ""
	odb_accessors = ""
	#Whether the table has a unique key is a class member
	# if not then a dummy id field will need to be added
	
	key_type = "unsigned long"
	key_field = "auto_id"
	auto_primary_key_member = "\n\t#pragma db id auto\n\t%s %s;"%(key_type, key_field)
	unique_fields = set()
	
	if transims_class_name in nested_records_init:
		nested_fields = []
		nested_field_types = []
		nested_field_accessors = []
		nested_records[transims_class_name] = [nested_records_init[transims_class_name][0], nested_fields, nested_field_types, nested_field_accessors]
	for i in range(len(fields)):
		field =  fields[i]
		if field in unique_fields:
			continue
		else:
			unique_fields.add(field)
		nested_flag = False
		mapping_info = None
		conversion_info = None
		
		type = types[i]
		accessor = accessors[i]
		if type in type_map:
			if type=="String":
				a = 5
			mapping_info = type_map[type]
			type = mapping_info[0]
		if (transims_class_name, field) in field_conversion:
			conversion_info = field_conversion[(transims_class_name, field)]
			type = conversion_info[0]
		original_type = ""
		if transims_class_name in nested_records_init and field in nested_records_init[transims_class_name][1]:
			# print "A nested field %s relation %s will be processed separately"%(field, transims_class_name)
			nested_flag = True
			
		ref_type = ""
		#check if this field is a foreign key
		for item in potential_ref_types:
			if item.lower() in field.lower() and (transims_class_name,field) not in false_foreign_keys:
				ref_type = item
		if (transims_class_name,field) in true_ref_fields_types:
			ref_type = true_ref_fields_types[(transims_class_name, field)]
		
		if not ref_flag:
			ref_type = ""
		
		#define type
		if ref_type!="" and field != transims_class_name.lower():
			original_type = type
			type = "shared_ptr<%s>"%ref_type
		if type == "string":
			type = "std::string"			
		#define converter
		converter = ""
		if original_type != "":			
			converter = "container.%ss[file.%s()]"%(ref_type, accessor)
		elif mapping_info is not None:
			converter = "file.%s().%s"%(accessor, mapping_info[2])
		elif conversion_info is not None:
			converter = "%s(%sfile.%s())"%(conversion_info[1],conversion_info[2], accessor)
		else: 
			converter = "file.%s ()"%(accessor)
			
		if nested_flag:
			nested_fields.append(field)
			nested_field_types.append(type)
			nested_field_accessors.append(converter)
			continue
		

		if type == "string":
			constructor1 += "const std::string&"
		else:
			constructor1 += type
		constructor1 += " %s_, "%field
		constructor2 += "%s (%s_), "%(field, field)
		if (transims_class_name.lower() in field.lower() and (transims_class_name, field) not in false_primary_keys) or (transims_class_name, field) in true_primary_keys: #this field is a primary key field
			key_type = type
			key_field = field
			auto_primary_key_member = ""
			members += "\t#pragma db id\n"
		#capitilized column name
		#members += "\t#pragma db column(\"%s\")\n"%field.upper()
		members += "\t%s %s;\n"%(type, field)
		
		odb_accessors += "\tconst %s& get%s () const {return %s;}\n"%(type, field.title(), field)
		odb_accessors += "\tvoid set%s (const %s& %s_){%s = %s_;}\n"%(field.title(),type, field,field, field)

		if original_type != "":			
			odb_accessors += "\tvoid set%s (const %s& %s_, %s& container){%s = container.%ss[%s_];}\n"%(field.title(),original_type, field, container_type,field,ref_type,field)
			#odb_accessors += "\tconst %s& get%s () const {return %s;}\n"%(type, field.title(), field)
			actual_ref_types.append(ref_type)
			if mapping_info is not None:
				adpater_method += "\n\tresult->set%s(file.%s ().%s, container); "%(field.title(), accessor, mapping_info[2])
			else:
				adpater_method += "\n\tresult->set%s(file.%s (), container); "%(field.title(), accessor)	
		elif mapping_info is not None:
			adpater_method += "\n\tresult->set%s(file.%s ().%s); "%(field.title(), accessor, mapping_info[2])
		elif conversion_info is not None:
			adpater_method += "\n\tresult->set%s(%s(%sfile.%s())); "%( field.title(), conversion_info[1],conversion_info[2], accessor)
		else: 
			adpater_method += "\n\tresult->set%s(file.%s ()); "%(field.title(), accessor)
	odb_accessors += "\tconst %s& get%s () const {return %s;}\n"%(key_type, "PrimaryKey", key_field)
	#define index for primary key
	if key_field!="auto_id":
		members += "\t#pragma db index member(%s)\n"%key_field
	type_primary_key_types[transims_class_name] = key_type
	if auto_primary_key_member!="":
		odb_accessors += "\tconst %s& get%s () const {return %s;}\n"%("unsigned long", "Auto_id", "auto_id")
	if transims_class_name in nested_records:
		odb_accessors += "\n\n\t//Vector that contains the associated nested records\n"
		odb_accessors += "\t std::vector<%s> nested_records;\n"%(nested_records[transims_class_name][0])
	
		

	odb_code = """
#pragma db object //table(\"%s\")
class %s
{
public:
	// Default Constructor
	%s () {}	
	//Contructor
	%s ) %s
	{
	}
	//Accessors
%s
//Data Fields
private:
	friend class odb::access;%s
%s
};
"""%(polaris_class_name.upper(), polaris_class_name, polaris_class_name, constructor1[:-2], constructor2[:-2],  odb_accessors, auto_primary_key_member, members)
	
	adapter_code = """//Converter for %s
%s
	return result;
}
"""%(transims_class_name, adpater_method[:-1])
	return (odb_code, adapter_code)					


#parse the command line arguments
help_msg = """
usage: generate_odb_classes.py syslib_include_path
usage: generate_odb_classes.py C:\\Users\\vsokolov\\usr\\transims\\sf-version5\\branches\\aecom\\1457\\SysLib\\Include\\
will generate a single cpp file with the classes defined for the types defined in File_Service.hpp

"""
if len(sys.argv) < 2:
	print help_msg
	sys.exit()
	

###################################################
#	         	Initialization 					  #
###################################################	
#this pattern is applied to *_File.hpp file to extract the fileds
file_class_member_p = re.compile("(char|bool|int|string|double|float|short|Dtime|String)\s*(\w+).*Get_\w+\s*\((\w+)\)")
#this template is applied to File_Service.hpp file to exract the names of *_File objects
p_tables = re.compile("#include\s*\"(\w+)_File\.hpp\"")	
odb_namespace = "pio"
container_type = "InputContainer"
types = []
type_primary_key_types = {}
potential_ref_types = []
actual_ref_types = []

###################################################
#	         	Custom Rules 					  #
###################################################
#(type, field)
false_primary_keys = [("Trip", "trip"), ("Vehicle", "vehicle"), ("Sign", "sign"), ("Timing", "timing"), ("Phasing", "phasing")]
#(type, field)
false_foreign_keys = [("Phasing","detectors"), ("Ridership", "schedule"),("Event", "schedule")]
#(type, field)
true_primary_keys = [("Veh_Type", "type")]
# (type, field) -> foreign key to relation
true_ref_fields_types = {("Trip","origin"):"Location", ("Trip","destination"):"Location",("Vehicle", "type"):"Veh_Type"}
#transims type -> (polaris type, transims type, transms type getter)
type_map = {"Dtime":("double", "Dtime", "Seconds()"), "String":("int", "String", "Integer()")}
# (type,field) -> (new type, conversion method, class to cast to)
field_conversion = {("Connect","lanes"):("string", "Static_Service::Lane_Range_Code", "")}
field_conversion[("Connect","to_lanes")] = ("string", "Static_Service::Lane_Range_Code","")
field_conversion[("Connect","type")] = ("string", "Static_Service::Connect_Code","(Connect_Type)")
field_conversion[("Link","type")] = ("string", "Static_Service::Facility_Code","(Facility_Type)")
field_conversion[("Signal","type")] = ("string", "Static_Service::Signal_Code","(Signal_Type)")
field_conversion[("Sign","sign")] = ("string", "Static_Service::Control_Code","(Control_Type)")
field_conversion[("Phasing","movement")] = ("string", "Static_Service::Movement_Code","(Movement_Type)")
field_conversion[("Pocket","type")] = ("string", "Static_Service::Pocket_Code","(Pocket_Type)")
field_conversion[("Phasing","protect")] = ("string", "Static_Service::Protection_Code","(Protection_Type)")

# type -> [nested_type, (field1, field2,...), (TypeOffield1, TypeOffield2,...), (field1Getter, field2Getter,...)]
# example: shape -> (shape_geometry, (x,y,x))
nested_records = {}
nested_records_init = {}
nested_records_init["Shape"]   = ["shape_geometry", ("x","y","z")]#,("double", "double", "double"),("file.X()","file.Y()","file.Z()"))}
nested_records_init["Signal"]  = ["signal_time",    ("start", "end", "timing", "phasing", "notes")]#,("double", "double", "int", "int", "std::string"),("file.Start().Seconds()", "file.End().Seconds()", "file.Timing()", "file.Phasing()", "file.Notes()"))
nested_records_init["Phasing"] = ["phase_movement", ("link", "dir", "to_link", "protect", "movement")]#,("shared_ptr<Link>", "int", "shared_ptr<Link>", "int", "std::string"),("container.Links[file.Link()]", "file.dir()", "container.Links[file.To_Link()]", "file.Protection()", "Static_Service::Movement_Code((Movement_Type)file.Movement())"))
nested_records_init["Timing"] = ["timing_phase", ("phase", "barrier", "ring", "position", "minimum", "maximum","extend","yellow","red")]


if len(sys.argv)!=2:
	print "The path to the SysLib Include folder was not provided. Exiting... "
	sys.exit()
syslib_include_path = sys.argv[1]
temp =  os.path.join(syslib_include_path, "File_Service.hpp")
if not os.path.exists(temp):
	print "The %s path does not exist, exiting..."%temp
	sys.exit()
#populate the types list
with open(temp, 'r') as fh:
	for line in fh:
		m = p_tables.match(line)
		if m is not None:
			types.append(m.group(1))
			
#initialize strings and file handlers
forward_declarations = ""
nested_classes_code = ""
odb_code=""
adapter_code=""
nested_adapter_code = ""
input_container = "class %s \n{\npublic:\n"%container_type
odb_fh =  open("out\\InputContext.h", 'w')
adapter_fh =  open("out\\adapter_methods.h", 'w') 
no_ref_types = ["Trip"]

#populate potential_ref_types and forward_declarations
for item in types:
	if item.lower() not in ["type", "use"] and item not in zip(*false_primary_keys)[0]:			
		potential_ref_types.append(item)	
	forward_declarations  += "class %s;\n"%item 	
forward_declarations += "class %s;\n"%container_type

#generate c++ code for new classes as well as adapter methods from transims to the new classes
for item in types:
	print "Processing %s"%item
	(o,a) = generate(os.path.join(syslib_include_path,"%s_File.hpp"%item), item)
	odb_code+=o
	adapter_code+=a

for item in no_ref_types:
	print "Processing %s"%item
	(o,a) = generate(os.path.join(syslib_include_path,"%s_File.hpp"%item), item, False, item+"NoRef")
	odb_code+=o
	adapter_code+=a
#generate input container class
for i in range(len(types)):
	item = types[i]
	if item not in actual_ref_types:
		continue
	input_container += "\tstd::map<%s,shared_ptr<%s>> %ss;\n"%(type_primary_key_types[item],item, item)
# print nested_records
for transims_type in nested_records:
	data = nested_records[transims_type]
	nscn = "%s::%s"%(odb_namespace,data[0])
	nested_adapter_code += "%s AdapterNested(%s_File &file, pio::InputContainer& container)\n{\n\t%s nested_record;\n"%(nscn, transims_type, nscn)
	nested_classes_code += "#pragma db value\nclass %s\n{\npublic:\n"%data[0]
	for i in range(len(data[1])):
		nested_classes_code += "\t%s %s;\n"%(data[2][i],data[1][i])
		nested_adapter_code += "\tnested_record.%s = %s;\n"%(data[1][i], data[3][i])
	nested_classes_code += "};\n"
	nested_adapter_code += "\treturn nested_record;\n};\n"
#write content to the files
odb_fh.write("""
#ifndef InputContext
#define InputContext
#pragma warning(disable:4068)

#include <map>
#include <vector>
#include <string>

#include <odb/core.hxx>

// Include TR1 <memory> header in a compiler-specific fashion. Fall back
// on the Boost implementation if the compiler does not support TR1.
//
#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;
""")
odb_fh.write("namespace %s\n{\n"%odb_namespace)	
odb_fh.write("//Forward declarations.\n//\n"+forward_declarations)
odb_fh.write(nested_classes_code)	
odb_fh.write("//Input Container.\n//\n"+input_container+"};\n")
odb_fh.write(odb_code)
odb_fh.write("\n}//end of namespace\n") #close namespace bracket
odb_fh.write("#endif // InputContext")
	
adapter_fh.write("""
#pragma once
#include "InputContext.h"
#include "File_Service.hpp"
""")
adapter_fh.write(adapter_code)
adapter_fh.write(nested_adapter_code)
	
	
		
