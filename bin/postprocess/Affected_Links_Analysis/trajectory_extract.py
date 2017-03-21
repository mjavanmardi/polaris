
import pprint
import sys
import urllib2
import urllib
import json
import csv
import time
import random
import math

input_trajectory_file_name =	"vehicle_trajectory_test.csv"
GENERATE_TRACKING_LIST = False  # set to true if outputing trajectories based on traffic incident impact, otherwise false if using a tracking list
USE_TRACKING_LIST = False
USE_INCIDENTS = False
SAMPLING_RATE = 1.0
MINIMUM_SPEED = 6.0				# minimum travel speed threshold in mph
MAX_DECELERATION = 11.2			# max acceleration/deceleration in ft/s^2

# if GENERATE TRACKING LIST or USE INCIDENTS: Specify filenames here
incident_file_name =			"incidents.txt"
tracking_file_name =			"tracked_vehicles.txt"

incident_array = []
tracked_person_list = {}
person_list = {}
affected_person_ids = {}
affected_persons = {}
trip_count = 1
output_trajectory_file_name =	input_trajectory_file_name[:-4]+"_output.csv"


if GENERATE_TRACKING_LIST: USE_INCIDENTS=True


class Incident:
	def __init__(self,data_row):
		self.id = int(data_row[0])
		self.link_id = int(data_row[1])
		self.start_time = int(data_row[2])
		self.end_time = int(data_row[3])
		self.notes = data_row[4]
	def affects_trajectory(self,trajectory):
		for tu in trajectory.trajectory_units:
			if tu is None:
				print "Error - trajectory unit uninitialized"
			else:
				if tu.link_id == self.link_id and tu.enter_time > self.start_time - 15*60 and tu.enter_time < self.end_time:
					tu.affected_by_incident = True
					return True
		return False
		
class Trajectory_Unit:
	def __init__(self,data_row):
		#try:
			self.id = int(data_row[1])
			self.link_id = int(data_row[2])
			self.link_dir = int(data_row[3])
			t=time.strptime(data_row[4], "%H:%M:%S")
			self.enter_time = t.tm_hour*3600 + t.tm_min*60 + t.tm_sec
			self.travel_time = int(data_row[5])
			self.start_pos = float(data_row[6])
			self.length = float(data_row[7])
			self.actual_speed = float(data_row[8])
			self.free_flow_speed = float(data_row[9])
			self.stopped_time = float(data_row[10])
			self.stop_pos = float(data_row[11])
			self.exit_time = self.enter_time + int(data_row[5])
			self.affected_by_incident = False
			# check for actual-flow speed realism, and alter the stop-time/actual speed as needed
			self.Correct_Speed()
		#except:
		#	print "Error parsing data row."
		#	print data_row
		#	sys.exit()

	
	def Correct_Speed(self):
		global MINIMUM_SPEED
		global MAX_DECELERATION

		# IF(AND(actual_speed>free_flow_speed+2,stopped_time>0),INT(travel_time-length/5280/free_flow_speed*3600),stopped_time)
		if self.actual_speed > self.free_flow_speed and self.stopped_time > 0:
			tmp_stop = int(self.travel_time-self.length/5280.0/self.free_flow_speed*3600.0)
			if tmp_stop < 0: tmp_stop = 0
			self.stopped_time = tmp_stop
			self.actual_speed = (self.length/5280.0)/ float(self.travel_time - self.stopped_time)*3600.0

		# correct the trajectory unit speed if less than minimum by adding stopped time for Autonomie compatibility
		if self.actual_speed < MINIMUM_SPEED:
			speed = MINIMUM_SPEED * 5280.0 / 3600.0
			new_ttime = self.length / speed
			new_stop_time = float(self.travel_time) - new_ttime
			self.stopped_time = new_stop_time
			self.actual_speed = MINIMUM_SPEED

		# correct the speed for maximum acceleration/deceleration rate for short links
		if self.stopped_time > 0:
			speed = self.actual_speed * 5280.0 / 3600.0
			decel = (speed * speed)/(2.0 * self.length)
			if decel > MAX_DECELERATION:
				new_speed = math.sqrt(MAX_DECELERATION * (2.0 * self.length))
				self.actual_speed = new_speed * 3600.0 / 5280.0

	def To_String(self):
		s = ""
		s += str(self.id)+ ","
		s += str(self.link_id)+ ","
		s += str(self.link_dir)+ ","
		s += str(self.enter_time)+ ","
		s += str(self.travel_time)+ ","
		s += str(self.start_pos)+ ","
		s += str(self.length)+ ","
		s += str(self.actual_speed)+ ","
		s += str(self.free_flow_speed)+ ","
		s += str(self.stopped_time)+ ","
		s += str(self.stop_pos)
		return s
		
class Trajectory:
	def __init__(self,data_row):
		self.trajectory_units = []
		self.tu_ids = []
		t = Trajectory_Unit(data_row)
		self.trajectory_units.append(t)
		self.tu_ids.append(t.id)
		self.affected_by_incident = False
	def Write(self, file, trip_id, per_id):
		for t in self.trajectory_units:
			file.write(str(trip_id) + "," + str(per_id) + "," + t.To_String() + '\n')		
		
class Person:
	def __init__(self,id,data_row):
		self.id = int(id)
		self.affected = False
		self.trajectories = []
		t = Trajectory(data_row)
		self.trajectories.append(t)
		
	def add_trajectory_data(self,data_row):
		t = self.trajectories[-1]
		tu = Trajectory_Unit(data_row)
		if tu.id in t.tu_ids:
			new_traj = Trajectory(data_row)
			self.trajectories.append(new_traj)
		else:
			t.trajectory_units.append(tu)
		
				
def Check_Affected_Trajectories():
	if USE_INCIDENTS:
		# check each person for being affected by incident			
		for person_id, person in person_list.iteritems():
			for t in person.trajectories:
				for i in incident_array:
					if i.affects_trajectory(t):
						if person_id not in affected_persons:
							if GENERATE_TRACKING_LIST: # when generating the tracking list, only store the person id
								affected_persons[person_id]=person_id
							else:
								affected_persons[person_id]=person
						t.affected_by_incident = True
						break
				if t.affected_by_incident:
					break
		person_list.clear()

	elif USE_TRACKING_LIST:
		# only print people in a previously generated tracking list
		for person_id, person in person_list.iteritems():
			if person_id in tracked_person_list:
				affected_persons[person_id]=person
		person_list.clear()

	else:
		# print randomly sampled trajectories
		for person_id, person in person_list.iteritems():
			if random.random() < SAMPLING_RATE:
				affected_persons[person_id]=person
		person_list.clear()

def Write_Affected_Person_IDs():
	with open(tracking_file_name, 'w') as fw:
		for id in affected_persons:
			fw.write(str(id) + '\n')
	affected_persons.clear()

def Write_Affected_Trajectories():
	global trip_count
	# Print trajectories
	with open(output_trajectory_file_name, 'a') as fw:
		for id, person in affected_persons.iteritems():
			for t in person.trajectories:
				t.Write(fw,trip_count,id)
				trip_count += 1
	affected_persons.clear()

random.seed()

# Define the incidents or individuals to check against the input trajectory file
if USE_INCIDENTS:
	# Read incidents
	with open(incident_file_name) as fh:
		reader = csv.reader(fh,delimiter='\t')
		header = reader.next()
		for sline in reader:
			i =Incident(sline)
			incident_array.append(i)

else:
	# Read tracked vehicle ids
	with open(tracking_file_name) as fh:
		reader = csv.reader(fh,delimiter='\t')
		for sline in reader:
			tracked_person_list[sline[0]] = sline[0]


# Prepare output file
if not GENERATE_TRACKING_LIST: 
	with open(output_trajectory_file_name, 'w') as fw:
		fw.write("TRIP,vehicle,link_number,link_id,link_dir,entering_time,travel_time,start_position,length,actual_speed,free_flow_speed,stopped_time,stop_position\n")
		

# Read trajectories
person_count = 0
with open(input_trajectory_file_name) as fh:
	reader = csv.reader(fh,delimiter=',')
	header = reader.next()
	try:
		for sline in reader:
			offset = 0
			if len(header)==12:
				veh_id = sline[0]		
			else:
				veh_id = sline[1]
				offset = 1

			if veh_id not in person_list:
				p = Person(veh_id,sline)
				person_count += 1
			
				if person_count % 10000 == 0:
					Check_Affected_Trajectories()
					if not GENERATE_TRACKING_LIST: Write_Affected_Trajectories()
					print "Finished analyzing " + str(person_count) + " travelers."
				
				person_list[veh_id] = p
			
			else:
				person_list[veh_id].add_trajectory_data(sline)
	except csv.Error, e:
		sys.exit('line %d: %s' % (reader.line_num, e))

	Check_Affected_Trajectories()
	if GENERATE_TRACKING_LIST:
		Write_Affected_Person_IDs()
	else:
		Write_Affected_Trajectories()
	print "Finished analyzing " + str(person_count) + " travelers."

		
	
	