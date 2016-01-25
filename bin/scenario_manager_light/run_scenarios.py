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

# Updated so that study folders are not overwritten, but a new folder created
study_folder  = out_folder + '/' + bath_id
tmp_study_folder = study_folder
batch_count = 1
while os.path.exists(study_folder):
	batch_count += 1
	study_folder = tmp_study_folder + '_' + str(batch_count)
os.makedirs(study_folder)	


# copy the executable to the runs directory
polaris_exe = s_doc['polaris_exe']
polaris_exe_basename = os.path.basename(polaris_exe)
bin_folder = study_folder + '/bin'
if not os.path.exists(bin_folder):
	os.makedirs(bin_folder)	
shutil.copy(polaris_exe, bin_folder)


# copy model input files
if 'input_file_directory' in s_doc:
	input_files = s_doc['input_file_directory']
	if not os.path.exists(input_files):
		print "Error: input file directory '",input_files, " not found."
		sys.exit()
	files = os.listdir(input_files)
	for file in files:
		file_path = os.path.join(input_files, file)
		if os.path.isfile(file_path):
			shutil.copy(file_path, study_folder)
	
# not sure how valuable this is, may be superseded by the addition of the 'previous' keyword logic below
prev_study_folder = ""
if 'previous_run' in s_doc:
	prev_study_folder = s_doc['previous_run']	
	if not os.path.exists(prev_study_folder):
		print "ERROR: previous study run directory not found."
		sys.exit()

# specify threads used in the batch file calls		
threads = 1
if 'threads' in s_doc:
	threads = s_doc['threads']
	
# copy the study parameters file to the study folder so scenarios can be identified easily
shutil.copy(study_parameters_file,study_folder)
batch_fh = open(study_folder + '/run.bat','w')
scenario_id = 1
for scenario in s_doc['scenarios']:
	print scenario_id
	m_doc = json.load(open(master_scenario_file))
	for parmeter_key in scenario:
		value = scenario[parmeter_key]
		
		# addition of logic to support compound keywords - in this case a previous keyword where any of the 'output...' keywords from a previous model run can be referenced as input
		# format: "input..." : "previous [output keyword] [scenario_id]"
		compound_key = str(scenario[parmeter_key]).split()
		if compound_key[0] == 'previous':		
			if len(compound_key) < 2:
				print "ERROR: parameter keys beginning with 'previous' should be followed by specification of the source key and source model run (if not using the previous run)"
			src_parmeter_key = compound_key[1]
			if len(compound_key) < 3:
				value = bath_id+'_'+str(scenario_id-1) + '/' + m_doc[src_parmeter_key]
			else:
				value = bath_id+'_'+compound_key[2] + '/' + m_doc[src_parmeter_key]
			
		m_doc[parmeter_key] = value
	m_doc['output_dir_name'] = bath_id+'_'+str(scenario_id)
	m_doc['master_scenario_file'] = master_scenario_file
	
	# don't remember why I added this - in this case a skim file from a previous study is used - differs slightly from the previous run logic above
	if prev_study_folder <> "":
		m_doc['input_highway_skim_file_path_name'] = prev_study_folder + '/' + bath_id+'_'+str(scenario_id) + '/' + m_doc['input_highway_skim_file_path_name']
	scenario_file_name = 'scenario_'+str(scenario_id)+'.json'
	with open(study_folder + '/' +  scenario_file_name,'w') as fh:
		fh.write(json.dumps(m_doc, sort_keys=True, indent=4, separators=(',',': ')))
	batch_fh.write(".\\bin\\%s %s %i\n"%(polaris_exe_basename, scenario_file_name, threads))
	scenario_id+=1	
	
	
		
	
