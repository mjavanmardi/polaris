import sys
import os
import math
import numpy
import struct
import argparse



######################################################################################
# Place code to work with Skims in this funcxtion
######################################################################################	
def Main(skims, highway_skim_file, transit_skim_file, write_bin, write_csv, origin_list, dest_list):

	do_highway = GetHighwaySkims(highway_skim_file, skims)
	do_transit = GetTransitSkims(transit_skim_file, skims)
	
	#skims.print_header_info()
	#skims.print_skims()

	if do_highway and write_bin: WriteHighwaySkimsV1(highway_skim_file, skims)
	if do_transit and write_bin: WriteTransitSkimsV1(transit_skim_file, skims)
	
	if do_highway and write_csv: WriteHighwaySkimsV1_CSV(highway_skim_file, skims, origin_list, dest_list)
	if do_transit and write_csv: WriteTransitSkimsV1_CSV(transit_skim_file, skims, origin_list, dest_list)

######################################################################################
# Skim Functions - do not modify anything below this section
######################################################################################
def GetHighwaySkims(highway_skim_file, skims):

	if highway_skim_file == '':
		return False
		
	infile = open(highway_skim_file, 'rb')
	
	# check version tag
	if not Check_Tag(infile,"SKIM:V01"):
		version1 = False
		infile.seek(-8,1)
	else:
		version1 = True
	
	# check to see if <BZON> tag is in the file, if not read old version header information (modes,num_zones)
	if not Check_Tag(infile,"BZON"):
		infile.seek(-4,1)
		# get zones
		modes, zones = struct.unpack("ii",infile.read(8))
	# if zone tag is there, read in numzones, then the zone id-index pairs
	else:
		zones = struct.unpack("i",infile.read(4))[0]
		for i in range(zones):
			id, index = struct.unpack("ii",infile.read(8))
			if id not in skims.zone_id_to_index_map.keys():
				skims.zone_id_to_index_map[id] = index
				skims.zone_index_to_id_map[index] = id
			else:
				print "Error, zone id was found more than once in zone map."
		Check_Tag(infile,"EZON",exit=True)
		
	# read intervals
	if version1:
		Check_Tag(infile,"BINT",True)
		num_intervals = struct.unpack("i",infile.read(4))[0]
		for i in range(num_intervals):
			skims.intervals.append(struct.unpack("i",infile.read(4))[0])
		Check_Tag(infile,"EINT",True)
	else:
		increment = struct.unpack("i",infile.read(4))[0] / 60
		interval = increment
		num_intervals = 0
		while interval < 1441:
			skims.intervals.append(interval)
			interval = interval + increment
			num_intervals = num_intervals + 1
	size = zones*zones
	skims.num_zones=zones

	# for each interval, check tags and read in matrix
	for i in range(num_intervals):
		if version1: Check_Tag(infile,"BMAT",True)
		data = numpy.matrix(numpy.fromfile(infile, dtype='f',count = size))
		if data.size < size:
			print "Error: matrix not read properly for interval " + str(i) + ", copying from previous"
			skims.auto_skims[skims.intervals[i]] = skims.auto_skims[skims.intervals[i-1]]
		else:
			skims.auto_skims[skims.intervals[i]] = data.reshape(zones,zones)
		if version1: Check_Tag(infile,"EMAT",True)
			
	return True

def GetTransitSkims(transit_skim_file, skims):

	version1 = True
	if transit_skim_file == '':
		return False
	
	infile = open(transit_skim_file, 'rb')
		
	# get tag
	if not Check_Tag(infile,"SKIM:V01",False):
		version1 = False
		infile.seek(-8,1)
	else:
		version1 = True

	# check to see if <BZON> tag is in the file, if not read old version header information (modes,num_zones)
	if not Check_Tag(infile,"BZON"):
		infile.seek(-4,1)
		# get zones
		tzones = struct.unpack("i",infile.read(4))[0]
	# if zone tag is there, read in numzones, then the zone id-index pairs
	else:
		tzones = struct.unpack("i",infile.read(4))[0]
		for i in range(zones):
			id, index = struct.unpack("ii",infile.read(8))
			if id not in skims.zone_id_to_index_map.keys():
				print "Error: zone id found in transit file that does not exist in highway file"
			else:
				if skims.zone_id_to_index_map[id] != index:
					print "Error: zone id has a different index in transit file from that found in highway file"
		tag = struct.unpack("<4s",infile.read(4))[0]
		Check_Tag(infile,"EZON",exit=True)
		
	tsize = tzones*tzones
	skims.num_tzones=tzones

	if version1: Check_Tag(infile, "BMAT")
	skims.transit_ttime = numpy.matrix(numpy.fromfile(infile, dtype='f',count = tsize))
	skims.transit_ttime = skims.transit_ttime.reshape(tzones,tzones)
	if version1: Check_Tag(infile, "EMAT")

	if version1: Check_Tag(infile, "BMAT")
	skims.transit_walk_access_time = numpy.matrix(numpy.fromfile(infile, dtype='f',count = tsize))
	skims.transit_walk_access_time = skims.transit_walk_access_time.reshape(tzones,tzones)
	if version1: Check_Tag(infile, "EMAT")
	
	if version1: Check_Tag(infile, "BMAT")
	skims.auto_distance = numpy.matrix(numpy.fromfile(infile, dtype='f',count = tsize))
	skims.auto_distance = skims.auto_distance.reshape(tzones,tzones)
	if version1: Check_Tag(infile, "EMAT")
	
	if version1: Check_Tag(infile, "BMAT")
	skims.transit_wait_time = numpy.matrix(numpy.fromfile(infile, dtype='f',count = tsize))
	skims.transit_wait_time = skims.transit_wait_time.reshape(tzones,tzones)
	if version1: Check_Tag(infile, "EMAT")
	
	if version1: Check_Tag(infile, "BMAT")
	skims.transit_fare = numpy.matrix(numpy.fromfile(infile, dtype='f',count = tsize))
	skims.transit_fare = skims.transit_fare.reshape(tzones,tzones)
	if version1: Check_Tag(infile, "EMAT")
	return True
	
def Check_Tag(file, check_val, exit=False):
	size = len(check_val)
	read_val = file.read(size)
	
	# check if at end of file
	if len(read_val) != size: return False
	
	# if not, read in tag
	tag = struct.unpack("<" + str(size) + "s",read_val)[0]
	
	# check against expected value and exit if requested
	if tag != check_val:
		if exit:
			print "Error: " + check_val + " tag missing. Read as: " + tag
			sys.exit()
		else:
			return False
	else:
		return True
		
def WriteHighwaySkimsV1(highway_skim_file, skims):
	outfile = open(highway_skim_file + 'new.bin', 'wb')
	
	# Write version info
	outfile.write(struct.pack("<8s","SKIM:V01"))
	
	# Write zone identification info if available, othwerise write old-style zone info
	if len(skims.zone_id_to_index_map) > 0:
		outfile.write(struct.pack("<4s","BZON"))
		outfile.write(struct.pack("i",skims.num_zones))
		for id, index in skims.zone_id_to_index_map.items():
			outfile.write(struct.pack("ii",id, index))
		outfile.write(struct.pack("<4s","EZON"))
	else:
		outfile.write(struct.pack("ii",1, skims.num_zones))
	
	# Write intervals
	outfile.write(struct.pack("<4s","BINT"))
	outfile.write(struct.pack("i",len(skims.auto_skims)))
	for interval in sorted(skims.auto_skims.keys()):
		outfile.write(struct.pack("i",interval))	
	outfile.write(struct.pack("<4s","EINT"))
	
	# Write skim matrix for each interval
	for interval in sorted(skims.auto_skims.keys()):
		outfile.write(struct.pack("<4s","BMAT"))
		skims.auto_skims[interval].tofile(outfile)
		outfile.write(struct.pack("<4s","EMAT"))
	
def WriteTransitSkimsV1(transit_skim_file, skims):
	outfile = open(transit_skim_file + 'new.bin', 'wb')
	
	vtag = struct.pack("<8s","SKIM:V01")
	outfile.write(vtag)
	
	# Write zone identification info if available, othwerise write old-style zone info
	if len(skims.zone_id_to_index_map) > 0:
		outfile.write(struct.pack("<4s","BZON"))
		outfile.write(struct.pack("i",skims.num_tzones))
		for id, index in skims.zone_id_to_index_map.items():
			outfile.write(struct.pack("ii",id, index))
		outfile.write(struct.pack("<4s","EZON"))
	else:
		outfile.write(struct.pack("ii",1, skims.num_tzones))
	
	btag = struct.pack("<4s","BMAT")
	etag = struct.pack("<4s","EMAT")
	
	outfile.write(btag)
	skims.transit_ttime.tofile(outfile)
	outfile.write(etag)
	
	outfile.write(btag)
	skims.transit_walk_access_time.tofile(outfile)
	outfile.write(etag)
	
	outfile.write(btag)
	skims.auto_distance.tofile(outfile)
	outfile.write(etag)
	
	outfile.write(btag)
	skims.transit_wait_time.tofile(outfile)
	outfile.write(etag)
	
	outfile.write(btag)
	skims.transit_fare.tofile(outfile)
	outfile.write(etag)

def WriteHighwaySkimsV1_CSV(highway_skim_file, skims, origin_list, dest_list):
	with open(highway_skim_file + '.csv', 'w') as outfile:	
	
		# Update the origin and destination lists
		if origin_list is None: origin_list = sorted(skims.zone_id_to_index_map.keys())
		if dest_list is None: dest_list = sorted(skims.zone_id_to_index_map.keys())
		
		# Write intervals
		for interval in sorted(skims.auto_skims.keys()):
			outfile.write("Skim for interval" + str(interval) + '\n')	
			# Write skim matrix for each interval
			outfile.write(',')
			
			# write destination zone headers (if in dest list)
			for i in dest_list:
				outfile.write(str(i) + ',')			
			outfile.write('\n')
			
			for i in origin_list:
				outfile.write(str(i) + ',')
				for j in dest_list:
					i_id = skims.zone_id_to_index_map[i]
					j_id = skims.zone_id_to_index_map[j]
					outfile.write(str(skims.auto_skims[interval][i_id,j_id]) + ',')
				outfile.write('\n')
			outfile.write('\n')
		outfile.write('\n')

def WriteTransitSkimsV1_CSV(transit_skim_file, skims, origin_list, dest_list):
	outfile = open(transit_skim_file + 'new.bin', 'wb')
	
	vtag = struct.pack("<8s","SKIM:V01")
	outfile.write(vtag)
	
	# Write zone identification info if available, othwerise write old-style zone info
	if len(skims.zone_map) > 0:
		outfile.write(struct.pack("<4s","BZON"))
		outfile.write(struct.pack("i",skims.num_tzones))
		for id, index in skims.zone_id_to_index_map.items():
			outfile.write(struct.pack("ii",id, index))
		outfile.write(struct.pack("<4s","EZON"))
	else:
		outfile.write(struct.pack("ii",1, skims.num_tzones))
	
	btag = struct.pack("<4s","BMAT")
	etag = struct.pack("<4s","EMAT")
	
	outfile.write(btag)
	skims.transit_ttime.tofile(outfile)
	outfile.write(etag)
	
	outfile.write(btag)
	skims.transit_walk_access_time.tofile(outfile)
	outfile.write(etag)
	
	outfile.write(btag)
	skims.auto_distance.tofile(outfile)
	outfile.write(etag)
	
	outfile.write(btag)
	skims.transit_wait_time.tofile(outfile)
	outfile.write(etag)
	
	outfile.write(btag)
	skims.transit_fare.tofile(outfile)
	outfile.write(etag)



class Skim_Results:
	def __init__(self):
		self.auto_skims = {}
		self.zone_id_to_index_map = {}
		self.zone_index_to_id_map = {}
		self.transit_ttime = numpy.matrix(0)
		self.transit_walk_access_time = numpy.matrix(0)
		self.auto_distance = numpy.matrix(0)
		self.transit_wait_time = numpy.matrix(0)
		self.transit_fare = numpy.matrix(0)
		self.num_zones=0
		self.num_tzones=0
		self.intervals=[]
	def print_header_info(self):
		print "Zone info (id, index):"
		for id, index in self.zone_id_to_index_map.items():
			print id, index
		print "\r\nInterval info:"
		print "Number of intervals = " + str(len(self.auto_skims.keys())) + ":"
		for interval in sorted(self.auto_skims.keys()):
			print interval
		print "\r\n"
	def print_skims(self):
		for interval in sorted(self.auto_skims.keys()):
			print 'Skim end=' + str(interval)
			if len(self.auto_skims[interval])>0: print self.auto_skims[interval]
			print ''
		
		print 'transit_ttime'
		if self.transit_ttime.size>0: print self.transit_ttime
		print ''
		print 'transit_walk_access_time'
		if self.transit_walk_access_time.size>0: print self.transit_walk_access_time
		print ''
		print 'auto_distance'
		if self.auto_distance.size>0: print self.auto_distance
		print ''
		print 'transit_wait_time'
		if self.transit_wait_time.size>0: print self.transit_wait_time
		print ''
		print 'transit_fare'
		if self.transit_fare.size>0: print self.transit_fare


			
skims = Skim_Results()

# parse the command line args
parser = argparse.ArgumentParser(description='Process the skim data')
parser.add_argument('-auto_skim_file', default='highway_skim_file.bin')
parser.add_argument('-transit_skim_file', default='')
parser.add_argument('-csv', action='store_const', const=1)
parser.add_argument('-bin', action='store_const', const=1)
parser.add_argument('-origin_list', type=int, nargs='*')
parser.add_argument('-destination_list', type=int, nargs='*')
args = parser.parse_args()

write_csv=False
if args.csv==1: write_csv=True
write_bin=False
if args.bin==1: write_bin=True
if write_csv==False: write_bin=True

Main(skims, args.auto_skim_file, args.transit_skim_file, write_bin, write_csv, args.origin_list, args.destination_list)

