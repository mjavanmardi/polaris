import json
import sys
import os
import csv
if len(sys.argv) < 2:
	print "Usage: %s master_scenario_file study_parameters_file  [out_folder]"%sys.argv[0]
	sys.exit()
	
study_parameters_file = sys.argv[1]


out_folder = '.'



s_doc = json.load(open(study_parameters_file))
study_id= study_parameters_file.split('.')[0]
outfile = open(study_id+"_analysis.csv","w")


# first, build list of common study params
params = []
scenario_id = 1
for scenario in s_doc['scenarios']:
	for parameter_key in scenario:
		if parameter_key not in params: params.append(parameter_key)
	
# write header:
for key in params:
	print key
	outfile.write(key+",")
outfile.write("time,loaded,departed,arrived,in_network,switched,VMT,VHT,avg_travel_time,delay_inducted_switches,realtime_inducted_switches,its_inducted_switches,wallclock_time,simulated_time,physical_memory_usage,physical_memory_percentage\n")

# Next, read all scenario results
scenario_id = 1
for scenario in s_doc['scenarios']:
	scenario_name = study_id + "_" + str(scenario_id)
	
	# write param values for scenario
	for parameter_key in params:
		if parameter_key in scenario:
			outfile.write(str(scenario[parameter_key])+",")
		else:
			outfile.write(",")
	
	# write results
	with open(scenario_name + "\\summary.csv") as sumamry_file:
		reader = csv.reader(sumamry_file,delimiter=',')
		last_row=[]
		for row in reader:
			last_row=row
			pass
		for value in last_row:
			outfile.write(str(value) + ",")
		outfile.write("\n")
		
	scenario_id+=1	
	
	
		
	
