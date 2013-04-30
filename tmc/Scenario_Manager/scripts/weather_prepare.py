import sys
sys.path.append('/mnt/lustre/home/bxu/lib/simplejson-3.0.7')

import os
import subprocess
import simplejson
import weather_model

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

""" Open link file """
link_file = open(origin_dir+'/link', 'r')
""" Skip the head line """
head_line = link_file.readline()
links = list()
for link_line in link_file:
    links.append(link_line.split())

""" Load weather parameter file """
weather_parameters = simplejson.load(open('weather_adjustments.json', 'r'))

def link_to_str(link):
    str = ''
    link_len = len(link)
    for i in range(link_len - 1):
        str += link[i] 
        str += '\t'
    str += link[link_len - 1]
    return str

def create_link_file(weather_factor_table):
    links_copy = list(links)
    modeler = weather_model.Weather_Modeler(links_copy, weather_factor_table)
    modeler.adjust()
    weather_type = weather_factor_table.get('weather type');
    file_name = work_place+'/link_'+weather_type;
    adjusted_link_file = open(file_name, 'w')
    adjusted_link_file.write(head_line)
    links_len = len(links_copy)
    for i in range(links_len - 1):
        adjusted_link_file.write(link_to_str(links_copy[i]))
        adjusted_link_file.write('\n') 
    adjusted_link_file.write(link_to_str(links_copy[links_len - 1]))

def prepare_work_space(weather_factor_table):
    weather_type = weather_factor_table.get('weather type');
    file_name = work_place+'/link_'+weather_type
    target_dir = work_place + '/' + weather_type
    subprocess.call(['mkdir', target_dir])
    #print 'cp -rsf ' + origin_dir + '/. ' + target_dir
    subprocess.call(['cp', '-rsf', origin_dir + '/.', target_dir])
    #print 'mv -f ' + file_name + ' ' + target_dir + '/link'
    subprocess.call(['mv', '-f', file_name, target_dir + '/link'])
    print 'scenario ' + weather_type + ' prepared\n\n'

def prepare_pbs(weather_factor_table):
    weather_type = weather_factor_table.get('weather type');
    target_dir = work_place + '/' + weather_type
    pbs_name = target_dir + '/job.pbs'
    pbs_file = open(pbs_name, 'w')
    pbs_file.write('#PBS -l nodes=1:ppn=8\n')
    pbs_file.write('#\n')
    pbs_file.write('#PBS -l walltime=01:00:00\n')
    pbs_file.write('#\n')
    pbs_file.write('#PBS -N ' + weather_type + '\n')
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
    create_link_file(weather_factor_table);
    prepare_work_space(weather_factor_table);
    prepare_pbs(weather_factor_table);
    
