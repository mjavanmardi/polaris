import sys
sys.path.append('/mnt/lustre/home/bxu/lib/simplejson-3.0.7')

import os
import subprocess
import simplejson
import weather_model

if len(sys.argv) < 2:
    print 'Usage: python weather_submit.py work_place'
    quit()

work_place = sys.argv[1]
if not os.path.isdir(work_place):
    print 'Work place not valid'
    quit()

weather_parameters = simplejson.load(open('weather_adjustments.json', 'r'))
weather_jobs = open('weather_jobs', 'w')
for weather_factor_table in weather_parameters:
    weather_type = weather_factor_table.get('weather type');
    target_dir = work_place + '/' + weather_type
    pbs_name = target_dir + '/job.pbs'
    p = subprocess.Popen(['qsub', pbs_name], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = p.communicate()
    print 'job for weather type ' + weather_type + ' submitted with id ' + out
    weather_jobs.write(weather_type + ' ' + out)
weather_jobs.close()
