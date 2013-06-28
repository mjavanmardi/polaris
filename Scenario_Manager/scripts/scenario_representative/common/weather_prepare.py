import sys
sys.path.append('simplejson-3.0.7')

import os
import subprocess
import simplejson
import weather_model
from io import *

if len(sys.argv) < 3:
    print 'Usage: python weather.py original_directory work_place'
    quit()

origin_dir = sys.argv[1]
if not os.path.isdir(origin_dir):
    print 'Original directory ' + origin_dir + ' is an invalid directory.'
    quit()

work_place = sys.argv[2]
if not os.path.isdir(work_place):
    print 'Work place ' + work_place + ' is an invalid directory.'
    quit()

if len(os.listdir(work_place)) != 0:
    print 'Work place ' + work_place + ' is not empty. Clean up work place before preparing scenarios.'
    quit()

""" Load scenario configuration file """
scenario_parameters = simplejson.load(open(origin_dir + '/scenario.json', 'r'))

""" Load weather parameter file """
weather_parameters = simplejson.load(open('weather_scenarios.json', 'r'))
#starting_time = weather_parameters.get('starting_time_hh_mm')
#ending_time = weather_parameters.get('ending_time_hh_mm')
database_name = scenario_parameters.get('database_name')

def prepare_work_space(weather_factor_table):
    scenario_name = weather_factor_table.get('scenario name');
    target_dir = work_place + '/' + scenario_name
    subprocess.call(['mkdir', target_dir])
    #print 'cp -rsf ' + origin_dir + '/. ' + target_dir
    subprocess.call(['cp', '-rsf', origin_dir + '/.', target_dir])
    target_event_db = makePath(target_dir, "Network_Event")
    print target_event_db
    #print 'rm -f ' + target_event_db
    subprocess.call(['rm', '-f', target_event_db]);
    origin_event_db = makePath(origin_dir, "Network_Event")
    #print 'cp -f ' + origin_event_db
    subprocess.call(['cp', '-f', origin_event_db, target_dir])
    subprocess.call(['rm', '-f', target_dir + '/scenario.json']);
    subprocess.call(['cp', '-f', origin_dir + '/scenario.json', target_dir + '/scenario.json']);
    populate_weather_event(target_dir + "/" + database_name, weather_factor_table)
    print 'scenario ' + scenario_name + ' prepared\n\n'

def makePath(path, schema):
    return path + "/" + database_name + "-" + schema + ".sqlite"

def populate_weather_event(event_db, weather_factor_table):
    context = InputContext(event_db)
    context.initEvent("Weather")
    context.event = context.event._replace(reporter = "scenario manager")
    #context.event = context.event._replace(confidence_level = 5)
    #context.event = context.event._replace(time_detected = -1) #in seconds
    #context.event = context.event._replace(expected_duraition = 60*60*2) #in seconds
    context.event = context.event._replace(start_time = 1) #in seconds
    context.event = context.event._replace(end_time = 24*60*60) #in seconds
    context.event = context.event._replace(expired = 0) #0 = False, 1 = True
    context.event.links.append(-1)
    weather_type = weather_factor_table.get('type')
    context.event.values["type"] = weather_type
    if weather_type == "rain":
        context.event.values["precipm"] = weather_factor_table.get('rainfall rate')
    elif weather_type == "snow":
        context.event.values["snowdepthm"] = weather_factor_table.get('snowfall rate')
    elif weather_type == "temperature":
        context.event.values["temperature"] = weather_factor_table.get('temperature')
    elif weather_type == "wind":
        context.event.values["wind"] = weather_factor_table.get('wind speed')
    elif weather_type == "visibility":
        context.event.values["vism"] = weather_factor_table.get('visibility')
    elif weather_type != "clear_dry_pavement" and weather_type != "clear_wet_pavement":
        raise ValueError("weather_type " + weather_type + " not recognized.")
    context.persist_event();

def prepare_pbs(weather_factor_table):
    scenario_name = weather_factor_table.get('scenario name');
    target_dir = work_place + '/' + scenario_name
    pbs_name = target_dir + '/job.pbs'
    pbs_file = open(pbs_name, 'w')
    pbs_file.write('#PBS -l nodes=1:ppn=32\n')
    pbs_file.write('#\n')
    pbs_file.write('#PBS -l walltime=24:00:00\n')
    pbs_file.write('#\n')
    pbs_file.write('#PBS -N ' + scenario_name + '\n')
    pbs_file.write('#\n')
    pbs_file.write('#PBS -o ' + target_dir + '/job_stdout\n')
    pbs_file.write('#\n')
    pbs_file.write('#PBS -e ' + target_dir + '/job_stderr\n')
    pbs_file.write('#\n')
    pbs_file.write('cd ' + target_dir + '\n')
    pbs_file.write('#\n')
    pbs_file.write('./NetworkModel\n')
    pbs_file.close()
    """subprocess.call(['qsub', pbs_name])"""

for weather_factor_table in weather_parameters:
    prepare_work_space(weather_factor_table);
    prepare_pbs(weather_factor_table);
    
