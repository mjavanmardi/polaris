
import pprint
import sys
import urllib2
import urllib
import json
import csv
import time

incident_array = []
person_list = {}
affected_persons = []

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
		self.id = int(data_row[1])
		self.link_id = int(data_row[2])
		self.link_dir = int(data_row[3])
		t=time.strptime(data_row[4], "%H:%M:%S")
		self.enter_time = t.tm_hour*3600 + t.tm_min*60 + t.tm_sec
		self.exit_time = self.enter_time + int(data_row[5])
		self.affected_by_incident = False
		
class Trajectory:
	def __init__(self,data_row):
		self.trajectory_units = []
		self.tu_ids = []
		t = Trajectory_Unit(data_row)
		self.trajectory_units.append(t)
		self.tu_ids.append(t.id)
		self.affected_by_incident = False
		
		
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
	# check each person for being affected by incident			
	for person_id, person in person_list.iteritems():
		for t in person.trajectories:
			for i in incident_array:
				if i.affects_trajectory(t):
					if person_id not in affected_persons: affected_persons.append(person_id)
					t.affected_by_incident = True
					break
			if t.affected_by_incident:
				break
	person_list.clear()

def Write_Affected_Person_IDs():
	# Print trajectories
	with open("affected_vehicles.txt", 'a') as fw:
		for person in affected_persons:
			fw.write(str(person) + '\n')
	affected_persons[:] = []
			#print "Person ID: " + str(person.id)
			#t_count =1
			#for t in person.trajectories:
			#	if t.affected_by_incident:
			#		print "Trajectory " + str(t_count) + ":"
			#		for tu in t.trajectory_units:
			#			print tu.id, ":", tu.link_id, tu.enter_time, tu.exit_time, tu.affected_by_incident
			#		t_count += 1
		

# Read incidents
with open("incidents.txt") as fh:
	reader = csv.reader(fh,delimiter='\t')
	header = reader.next()
	for sline in reader:
		i =Incident(sline)
		incident_array.append(i)
		
# Read trajectories
person_count = 0
with open("vehicle_trajectory_all_trips.csv") as fh:
	reader = csv.reader(fh,delimiter=',')
	header = reader.next()
	for sline in reader:
		veh_id = sline[0]	
		if veh_id not in person_list:
			p = Person(veh_id,sline)
			person_count += 1
			
			if person_count % 10000 == 0:
				Check_Affected_Trajectories()
				print "Finished analyzing " + str(person_count) + " travelers."
				
			person_list[veh_id] = p
			
		else:
			person_list[veh_id].add_trajectory_data(sline)

	Check_Affected_Trajectories()
	Write_Affected_Person_IDs()
	print "Finished analyzing " + str(person_count) + " travelers."

		
# Print incidents
#for i in incident_array:
#	print i.id, i.link_id, i.notes
	
	