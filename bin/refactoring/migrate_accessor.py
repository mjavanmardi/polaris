#!/usr/local/bin/python

# This script can do most of the more "mechanical" operations to move to the new protptype/implementation syntax
# It would look for "accessor", parse the name, then search for the corresponding name in implementation "m_data" macros and extract the data type. Then replace the accessor line with the non-templated accessor.
# Then it could find all instances of the function call and match the word template (if applicable) and open angle bracket to its closing angle bracket, deleting that portion of it.
# It could present each operation to the user to confirm manually.
# It wouldn't work for everything but would be much faster than the compile-fix-compile cycle.

import re
import sys

args = sys.argv
if (len(args) < 3):
	print "Usage: %s in_path file_prefix [out_path]\nFor example '%s .. Link' will parse ../Link_Implementation.h and ../Link_Prototype.h"%(args[0], args[0],)
	sys.exit()
prefix   = args[2]
in_path =  args[1]
if (len(args) > 3):
	out_path = args[3]
else:
	out_path = './'	
with open(in_path + prefix + '_Implementation.h') as fh:
	impl = fh.read()

with open(in_path + prefix + '_Prototype.h') as fh:
	prot = fh.read()

name_re = re.compile("accessor\s*\(\s*(\w+)\s*\,")
accessor_name = name_re.findall(prot)
n_names = len(accessor_name)
accessor_type = [None]*n_names
accessor_macro = [None]*n_names
new_prot_line = [None]*n_names
new_impl_line = [None]*n_names
for i in range(n_names):
	aname = accessor_name[i]
	type_re = "(m_data|m_prototype|m_container)\s*\(\s*(.*)(?=\,\s*%s)"%aname
	m =  re.search(type_re, impl)
	accessor_type = m.group(2)
	accessor_macro = m.group(1)
	if accessor_macro == 'm_data':
		new_prot_line[i] = "\taccess(%s, %s);\n"%(aname, accessor_type)
		new_impl_line[i] = "\tdata(%s, %s);\n"%(aname, accessor_type)
	if accessor_macro == 'm_container':
		# aname = activity_locations
		m = re.search("(\w+)\<typename\s*MasterType\:\:\s*(\w+)\*?\>", accessor_type)
		container_type = m.group(1) # vector
		element_type   = m.group(2) # activity_location_type
		element_type_l = element_type.split('_'); n_temp = len(element_type_l)
		element_type_l   = element_type_l[0:(n_temp - 1)] #truncate _type
		element_type_titled = '_'.join(map(lambda x: x.title(), element_type_l)) # Activity_Location
		new_prot_line[i] = "\taccess(%s<%s_Components::%s<%s>*>*, %s);\n"%(container_type, element_type_titled, element_type_titled, element_type, aname)
		new_impl_line[i] = "\ttypedef typename MasterType::%s %s;\n"%(element_type,element_type)
		new_impl_line[i] += "\tdata(%s<%s_Components::%s<%s>*>*, %s);\n"%(container_type, element_type_titled, element_type_titled, element_type, aname)
	if accessor_macro == 'm_prototype':
		# aname = network_reference
		# get rid of Prototypes::
		accessor_type = accessor_type.replace("Prototypes::","")
		temp  = accessor_type.split(',')
		accessor_type      = temp[0]  # Network_Components::Network
		typename_stm        = temp[1] # typename MasterType::network_type
		accessor_type_short = typename_stm.split('::')[1]  # network_type
		typename      = typename_stm.split('::')[1] # network_type
		new_prot_line[i] = "\taccess(%s<get(%s_type)>*, %s);\n"%(accessor_type,aname, aname)
		new_impl_line[i] = "\ttypedef typename MasterType::%s %s;\n"%(accessor_type_short,accessor_type_short)
		new_impl_line[i] += "\tdata(%s<%s>*, %s);\n"%(accessor_type,accessor_type_short, aname)

# for item in new_prot_line:
# 	print item

# for item in new_impl_line:
# 	print item

impl_fh = open(out_path + '/' + prefix + '_Implementation.h', 'w')
prot_fh = open(out_path + '/' + prefix + '_Prototype.h', 'w')

impl_fh.write("""
implementation struct %s_Implementation:public Polaris_Component<MasterType,INHERIT(%s_Implementation),Execution_Object>
{
	typedef typename Polaris_Component<MasterType,INHERIT(%s_Implementation),Execution_Object>::Component_Type ComponentType;
"""%(prefix, prefix,prefix,))

prot_fh.write("""
	#define get(X) typename ComponentType::X

prototype struct Link
{
	tag_as_prototype;
""")

for i in range(n_names):
	ans = raw_input("\nImplementation code for %s:\n%sApprove [Y/n]?: "%(accessor_name[i],new_impl_line[i])) 
	if (ans=="" or ans == "y" or ans == "Y"):
		impl_fh.write(new_impl_line[i])
	else:
		impl_fh.write("/* \nSCRIPT COVERTED INCORRECTLY NEED ATTENTION\n"+new_impl_line[i]+'*/\n')
	ans = raw_input("\nProtytpe code for %s:\n%sApprove [Y/n]?: "%(accessor_name[i],new_prot_line[i])) 
	if (ans=="" or ans == "y" or ans == "Y"):
		prot_fh.write(new_prot_line[i])
	else:
		prot_fh.write("/* \nSCRIPT COVERTED INCORRECTLY NEED ATTENTION\n"+new_prot_line[i]+'*/\n`')
	# prot_fh.write(new_prot_line[i])

impl_fh.write("};\n")
prot_fh.write("};\n")

impl_fh.close()
prot_fh.close()



