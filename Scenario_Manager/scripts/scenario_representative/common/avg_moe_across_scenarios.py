from os import listdir
import sys
sys.path.append('simplejson-3.0.7')
import simplejson

output_file = open("../results/combined_moe_network.csv", "w")
target_dir = "../scenarios"
scenarios = listdir(target_dir)
scenario_lines = dict()

weather_scenarios = simplejson.load(open('weather_scenarios.json', 'r'))
scenario_probabilities = dict()
for scenario in scenarios:
    for weather_factor_table in weather_scenarios:
        type=weather_factor_table.get('scenario name')
        if type == scenario:
            scenario_probabilities[scenario] = float(weather_factor_table.get('probability'))

for scenario in scenarios:
    scenario_json = simplejson.load(open(target_dir+ "/" + scenario + "/scenario.json", "r"))
    output_dir_name = scenario_json.get('output_dir_name')
    file = open(target_dir+ "/" + scenario + "/" + output_dir_name + "/moe_network.csv", "r")
    scenario_lines[scenario] = []
    start_flag = True;
    for line in file:
        if start_flag == True:
            start_flag = False
        else: 
            scenario_lines[scenario].append(line.rstrip('\n').rstrip(',').split(","))

time_stamps = []
for scenario in scenarios:
    num_lines = len(scenario_lines[scenario])
    start_flag = True
    for line in scenario_lines[scenario]:
        if start_flag == True:
            num_moes = len(line) - 2
            start_flag = False
        time_stamps.append(line[0] + "," + line[1])
    break
   
for line_index in range(0, num_lines):
    weighted_moes = [0.0] * num_moes
    for scenario in scenarios:
        line = scenario_lines[scenario][line_index]
        for moe_index in range(0, num_moes):
            weighted_moes[moe_index] += float(line[moe_index + 2]) * scenario_probabilities[scenario]
    output_line = time_stamps[line_index]
    for moe_index in range(0, num_moes):
        output_line += ","
        output_line += str(weighted_moes[moe_index])
    #print "line " + str(line_index+1) + " processed"
    output_file.write(output_line + "\n")    

output_file.close()
