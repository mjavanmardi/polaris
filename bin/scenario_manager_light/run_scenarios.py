import json
import sys
import os

if len(sys.argv) < 4:
	print "Usage: %s master_scenario_file study_parameters_file output_path"%sys.argv[0]
	sys.exit()
	
master_scenario_file  = sys.argv[1]
study_parameters_file = sys.argv[2]
output_path           = sys.argv[3]
m_doc = json.load(open(master_scenario_file))
s_doc = json.load(open(study_parameters_file))

scenario_id = s_doc['initial_scenairo_id']
for scenario in s_doc['scnarios']:
	for parmeter_key in scenario:
		# print m_doc
		m_doc[parmeter_key] = scenario[parmeter_key]
	out_folder = output_path + '/' + str(scenario_id)
	if not os.path.exists(out_folder):
		os.makedirs(out_folder)		
	# print json.dumps(m_doc, sort_keys=True, indent=4, separators=(',',': '))
	m_doc['output_dir_name'] = out_folder
	m_doc['master_scenario_file'] = master_scenario_file
	scenario_id += 1 
	# print json.dumps(m_doc, sort_keys=True, indent=4, separators=(',',': '))
	with open(out_folder + '/scenario.json','w') as fh:
		# print m_doc['output_dir_name'] + '/scenario.json'
		# print m_doc
		fh.write(json.dumps(m_doc, sort_keys=True, indent=4, separators=(',',': ')))
		
		
		
# d = {}
# d['initial_scenairo_id'] = 10
# d['scnarios'] = []

# d['scnarios'].append({'a':10, 'b':20})
# d['scnarios'].append({'a':15, 'b':25})

# json.dumps(d, sort_keys=True, indent=4, separators=(',', ': '))
	
	
		
	
