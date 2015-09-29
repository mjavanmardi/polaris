import json
import sys
import os
import shutil
if len(sys.argv) < 3:
	print "Usage: %s master_scenario_file study_parameters_file  [out_folder]"%sys.argv[0]
	sys.exit()
	
master_scenario_file  = sys.argv[1]
study_parameters_file = sys.argv[2]


if len(sys.argv) > 3:
	out_folder           = sys.argv[3]
else:	
	out_folder = '.'

if not os.path.exists(out_folder):
	os.makedirs(out_folder)

if not os.path.exists(out_folder):
	os.makedirs(out_folder)	
print study_parameters_file
s_doc = json.load(open(study_parameters_file))

bath_id= study_parameters_file.split('.')[0]

study_folder  = out_folder + '/' + bath_id
if not os.path.exists(study_folder):
	os.makedirs(study_folder)	

polaris_exe = s_doc['polaris_exe']
polaris_exe_basename = os.path.basename(polaris_exe)
bin_folder = study_folder + '/bin'

prev_study_folder = ""
if 'previous_run' in s_doc:
	prev_study_folder = s_doc['previous_run']	
	if not os.path.exists(prev_study_folder):
		print "ERROR: previous study run directory not found."
		sys.exit()

if not os.path.exists(bin_folder):
	os.makedirs(bin_folder)	
shutil.copy(polaris_exe, bin_folder)
batch_fh = open(study_folder + '/run.bat','w')
scenario_id = 1
for scenario in s_doc['scenarios']:
	print scenario_id
	m_doc = json.load(open(master_scenario_file))
	for parmeter_key in scenario:
		m_doc[parmeter_key] = scenario[parmeter_key]
	m_doc['output_dir_name'] = bath_id+'_'+str(scenario_id)
	m_doc['master_scenario_file'] = master_scenario_file
	
	if prev_study_folder <> "":
		m_doc['input_highway_skim_file_path_name'] = prev_study_folder + '/' + bath_id+'_'+str(scenario_id) + '/' + m_doc['input_highway_skim_file_path_name']
	scenario_file_name = 'scenario_'+str(scenario_id)+'.json'
	with open(study_folder + '/' +  scenario_file_name,'w') as fh:
		fh.write(json.dumps(m_doc, sort_keys=True, indent=4, separators=(',',': ')))
	batch_fh.write(".\\bin\\%s %s 22\n"%(polaris_exe_basename, scenario_file_name))
	scenario_id+=1	
	
	
		
	
