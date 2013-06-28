import sys
sys.path.append('simplejson-3.0.7')
import simplejson

weather_type = sys.argv[1]
moe = float(sys.argv[2])
weighted_moe = float(sys.argv[3])
weather_parameters = simplejson.load(open('weather_scenarios.json', 'r'))
for weather_factor_table in weather_parameters:
    type=weather_factor_table.get('scenario name')
    if weather_type == type:
        probability = float(weather_factor_table.get('probability'))
weighted_moe = weighted_moe + (probability * moe);    
print weighted_moe
