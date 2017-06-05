import sys
import os
import math
import numpy
import struct
import argparse
import csv



######################################################################################
# Place code to work with Skims in this funcxtion
######################################################################################	
def Main(skims, highway_skim_file, transit_skim_file, write_bin, write_csv, write_tab, origin_list, dest_list):

	do_highway = GetHighwaySkims(highway_skim_file, skims)
	
	skims.print_header_info()
	
	do_transit = GetTransitSkims(transit_skim_file, do_highway, skims)
	
	if origin_list is not None: skims = ReduceTransitSkims(skims,origin_list)

	if do_highway and write_bin: WriteHighwaySkimsV1(highway_skim_file, skims)
	if do_transit and write_bin: WriteTransitSkimsV1(transit_skim_file, skims)
	
	if do_highway and write_csv: WriteHighwaySkimsV1_CSV(highway_skim_file, skims, origin_list, dest_list)
	if do_transit and write_csv: WriteTransitSkimsV1_CSV(transit_skim_file, skims, origin_list, dest_list)
	
	if do_highway and write_tab: WriteHighwaySkimsV1_TEXT(highway_skim_file, skims, origin_list, dest_list)
	#if do_transit and write_csv: WriteTransitSkimsV1_CSV(transit_skim_file, skims, origin_list, dest_list)

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

def GetTransitSkims(transit_skim_file, validate_against_highway, skims, zone_list=None):
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
		print "Number of zones = " + str(tzones)
		for i in range(tzones):
			id, index = struct.unpack("ii",infile.read(8))
			if validate_against_highway:
				if id not in skims.zone_id_to_index_map.keys():
					print "Error: zone id " + str(id) + " found in transit file that does not exist in highway file"
				else:
					if skims.zone_id_to_index_map[id] != index:
						print "Error: zone id has a different index in transit file from that found in highway file"
			else:
				if id not in skims.zone_id_to_index_map.keys():
					skims.zone_id_to_index_map[id] = index
					skims.zone_index_to_id_map[index] = id
				else:
					print "Error, zone id was found more than once in zone map."
		#tag = struct.unpack("<4s",infile.read(4))[0]
		Check_Tag(infile,"EZON",exit=True)
		
	tsize = tzones*tzones
	skims.num_tzones=tzones

	if version1: Check_Tag(infile, "BMAT",True)
	data = numpy.matrix(numpy.fromfile(infile, dtype='f',count = tsize))
	if data.size < tsize: print "Error: transit ttime matrix not read properly"
	else: skims.transit_ttime = data.reshape(tzones,tzones)
	if version1: Check_Tag(infile, "EMAT",True)

	if version1: Check_Tag(infile, "BMAT",True)
	data = numpy.matrix(numpy.fromfile(infile, dtype='f',count = tsize))
	if data.size < tsize: print "Error: transit walk time matrix not read properly"
	else: skims.transit_walk_access_time = data.reshape(tzones,tzones)
	if version1: Check_Tag(infile, "EMAT",True)
	
	if version1: Check_Tag(infile, "BMAT",True)
	data = numpy.matrix(numpy.fromfile(infile, dtype='f',count = tsize))
	if data.size < tsize: print "Error: auto distance matrix not read properly"
	else: skims.auto_distance = data.reshape(tzones,tzones)
	if version1: Check_Tag(infile, "EMAT",True)
	
	if version1: Check_Tag(infile, "BMAT",True)
	data = numpy.matrix(numpy.fromfile(infile, dtype='f',count = tsize))
	if data.size < tsize: print "Error: transit_wait_time matrix not read properly"
	else: skims.transit_wait_time = data.reshape(tzones,tzones)
	if version1: Check_Tag(infile, "EMAT",True)
	
	if version1: Check_Tag(infile, "BMAT",True)
	data = numpy.matrix(numpy.fromfile(infile, dtype='f',count = tsize))
	if data.size < tsize: print "Error: transit_fare matrix not read properly"
	else: skims.transit_fare = data.reshape(tzones,tzones)
	if version1: Check_Tag(infile, "EMAT",True)
	return True
	
def ReduceTransitSkims(skim, zone_list):
	new_skim = Skim_Results()
	
	index = 0
	for zoneid in zone_list:
		if zoneid in skim.zone_id_to_index_map:
			new_skim.zone_id_to_index_map[zoneid] = index
			index += 1
		else: print "ERROR: " + str(zoneid) + " from reduced zone list is not in original list of zones."
	
	new_skim.num_tzones = len(zone_list)
	new_skim.resize_arrays(len(zone_list))
			
	for ozoneid in zone_list:
		for dzoneid in zone_list:
			if ozoneid not in skim.zone_id_to_index_map or dzoneid not in skim.zone_id_to_index_map or ozoneid not in new_skim.zone_id_to_index_map or dzoneid not in new_skim.zone_id_to_index_map:
				print "ERROR: " + str(zoneid) + " from reduced zone list is not in original list of zones."
				sys.exit()
				
			o_idx_orig = skim.zone_id_to_index_map[ozoneid]
			d_idx_orig = skim.zone_id_to_index_map[dzoneid]
			o_idx = new_skim.zone_id_to_index_map[ozoneid]
			d_idx = new_skim.zone_id_to_index_map[dzoneid]
			
			new_skim.transit_ttime[o_idx,d_idx] = skim.transit_ttime[o_idx_orig,d_idx_orig]
			new_skim.transit_walk_access_time[o_idx,d_idx] = skim.transit_walk_access_time[o_idx_orig,d_idx_orig]
			new_skim.transit_wait_time[o_idx,d_idx] = skim.transit_wait_time[o_idx_orig,d_idx_orig]
			new_skim.transit_fare[o_idx,d_idx] = skim.transit_fare[o_idx_orig,d_idx_orig]
			new_skim.auto_distance[o_idx,d_idx] = skim.auto_distance[o_idx_orig,d_idx_orig]
			
	return new_skim

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
			print "Warning: tag '" + check_val + " was missing. Read as: " + tag
			return False
	else:
		return True
		
def WriteHighwaySkimsV1(highway_skim_file, skims):
	outfile = open(highway_skim_file, 'wb')
	
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
	outfile = open(transit_skim_file, 'wb')
	
	vtag = struct.pack("<8s","SKIM:V01")
	outfile.write(vtag)
	
	# Write zone identification info if available, othwerise write old-style zone info
	if len(skims.zone_id_to_index_map) > 0:
		outfile.write(struct.pack("<4s","BZON"))
		outfile.write(struct.pack("i",skims.num_tzones))
		print "Transit zones = " + str(skims.num_tzones)
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

def WriteHighwaySkimsV1_CSV(highway_skim_file, skims, origin_list=None, dest_list=None):
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
		
def WriteHighwaySkimsV1_TEXT(highway_skim_file, skims, origin_list=None, dest_list=None):
	with open(highway_skim_file + '.csv', 'w') as outfile:	
	
		# Update the origin and destination lists
		if origin_list is None: origin_list = sorted(skims.zone_id_to_index_map.keys())
		if dest_list is None: dest_list = sorted(skims.zone_id_to_index_map.keys())
		
		# Write intervals
		outfile.write('O,D,')
		for interval in sorted(skims.auto_skims.keys()):
			outfile.write(str(interval) + ',')	
		# Write skim matrix for each interval
		outfile.write('\n')
			
			
		for i in origin_list:
			for j in dest_list:
				i_id = skims.zone_id_to_index_map[i]
				j_id = skims.zone_id_to_index_map[j]
				outfile.write(str(i) + ',' + str(j) + ',')
				for interval in sorted(skims.auto_skims.keys()):
					 outfile.write(str(skims.auto_skims[interval][i_id,j_id]) + ',')
				outfile.write('\n')



def WriteTransitSkimsV1_CSV(transit_skim_file, skims, origin_list=None, dest_list=None):
	outfile = open(transit_skim_file + '.csv', 'wb')
	
	# Update the origin and destination lists
	if origin_list is None: origin_list = sorted(skims.zone_id_to_index_map.keys())
	if dest_list is None: dest_list = sorted(skims.zone_id_to_index_map.keys())
	
	# Write TTime
	outfile.write("Transit TTime Skim\n")	
	outfile.write(',')
	for i in dest_list:
		outfile.write(str(i) + ',')			
	outfile.write('\n')
	for i in origin_list:
		outfile.write(str(i) + ',')
		for j in dest_list:
			i_id = skims.zone_id_to_index_map[i]
			j_id = skims.zone_id_to_index_map[j]
			outfile.write(str(skims.transit_ttime[i_id,j_id]) + ',')
		outfile.write('\n')
	outfile.write('\n')
	
	# Write walk time
	outfile.write("Transit Walk Time Skim\n")	
	outfile.write(',')
	for i in dest_list:
		outfile.write(str(i) + ',')			
	outfile.write('\n')
	for i in origin_list:
		outfile.write(str(i) + ',')
		for j in dest_list:
			i_id = skims.zone_id_to_index_map[i]
			j_id = skims.zone_id_to_index_map[j]
			outfile.write(str(skims.transit_walk_access_time[i_id,j_id]) + ',')
		outfile.write('\n')
	outfile.write('\n')
	
	# Write wait time
	outfile.write("Transit Wait Time Skim\n")	
	outfile.write(',')
	for i in dest_list:
		outfile.write(str(i) + ',')			
	outfile.write('\n')
	for i in origin_list:
		outfile.write(str(i) + ',')
		for j in dest_list:
			i_id = skims.zone_id_to_index_map[i]
			j_id = skims.zone_id_to_index_map[j]
			outfile.write(str(skims.transit_wait_time[i_id,j_id]) + ',')
		outfile.write('\n')
	outfile.write('\n')
	
	# Write Fare
	outfile.write("Transit Fare Skim\n")	
	outfile.write(',')
	for i in dest_list:
		outfile.write(str(i) + ',')			
	outfile.write('\n')
	for i in origin_list:
		outfile.write(str(i) + ',')
		for j in dest_list:
			i_id = skims.zone_id_to_index_map[i]
			j_id = skims.zone_id_to_index_map[j]
			outfile.write(str(skims.transit_fare[i_id,j_id]) + ',')
		outfile.write('\n')
	outfile.write('\n')
	
	# Write Distance
	outfile.write("Distance Skim\n")	
	outfile.write(',')
	for i in dest_list:
		outfile.write(str(i) + ',')			
	outfile.write('\n')
	for i in origin_list:
		outfile.write(str(i) + ',')
		for j in dest_list:
			i_id = skims.zone_id_to_index_map[i]
			j_id = skims.zone_id_to_index_map[j]
			outfile.write(str(skims.auto_distance[i_id,j_id]) + ',')
		outfile.write('\n')
	outfile.write('\n')

#note transit_walk_files and transit_wait_files are lists - as there may be multiple wait/walk input files which are aggregated to one value
def ReadTransitSkims_CSV(transit_ttime_file, transit_walk_files, transit_wait_files, transit_fare_file, auto_distance_file, zone_id_to_index, skims):
	# get the zone map
	with open(zone_id_to_index, 'r') as infile:
		cr = csv.reader(infile,delimiter =',')
		for row in cr:
			skims.zone_id_to_index_map[int(row[0])] = int(row[1])
			skims.zone_index_to_id_map[int(row[1])] = int(row[0])
			
	nzone = len(skims.zone_id_to_index_map)
	skims.num_tzones = nzone
	
	# resize the matrices according to number of zones
	skims.resize_arrays(nzone)
	
	ignored_zones = []
	
	# read in each individual matrix and store_const
	delim = ','
	if '.txt' in transit_ttime_file: delim = '\t'
	else: delim = ','
	
	with open(transit_ttime_file, 'r') as infile:
		cr = csv.reader(infile,delimiter =delim)
		try:
			for row in cr:
				if len(row) > 2:
					if int(row[0]) not in skims.zone_id_to_index_map:
						if row[0] not in ignored_zones: ignored_zones.append(row[0])
					elif int(row[1]) not in skims.zone_id_to_index_map:
						if row[1] not in ignored_zones: ignored_zones.append(row[1])
					else:
						o = skims.zone_id_to_index_map[int(row[0])]
						d = skims.zone_id_to_index_map[int(row[1])]
						skims.transit_ttime[o,d] = float(row[2])
		except ValueError:
			print row[2]
	
	for transit_walk_file in transit_walk_files:
		if '.txt' in transit_walk_file: delim = '\t'
		else: delim = ','
		with open(transit_walk_file, 'r') as infile:
			cr = csv.reader(infile,delimiter =delim)
			try:
				for row in cr:
					if len(row) > 2:
						if int(row[0]) not in skims.zone_id_to_index_map:
							if row[0] not in ignored_zones: ignored_zones.append(row[0])
						elif int(row[1]) not in skims.zone_id_to_index_map:
							if row[1] not in ignored_zones: ignored_zones.append(row[1])
						else:
							o = skims.zone_id_to_index_map[int(row[0])]
							d = skims.zone_id_to_index_map[int(row[1])]
							skims.transit_walk_access_time[o,d] += float(row[2])
			except ValueError:
					print row[2]
	
	for transit_wait_file in transit_wait_files:
		if '.txt' in transit_wait_file: delim = '\t'
		else: delim = ','
		with open(transit_wait_file, 'r') as infile:
			try:
				cr = csv.reader(infile,delimiter =delim)
				for row in cr:
					if len(row) > 2:
						if int(row[0]) not in skims.zone_id_to_index_map:
							if row[0] not in ignored_zones: ignored_zones.append(row[0])
						elif int(row[1]) not in skims.zone_id_to_index_map:
							if row[1] not in ignored_zones: ignored_zones.append(row[1])
						else:
							o = skims.zone_id_to_index_map[int(row[0])]
							d = skims.zone_id_to_index_map[int(row[1])]
							skims.transit_wait_time[o,d] += float(row[2])
			except ValueError:
				print row[2]
				
	if '.txt' in transit_wait_file: delim = '\t'
	else: delim = ','		
	with open(transit_fare_file, 'r') as infile:
		cr = csv.reader(infile,delimiter =delim)
		try:
			for row in cr:
				if len(row) > 2:
					if int(row[0]) not in skims.zone_id_to_index_map:
						if row[0] not in ignored_zones: ignored_zones.append(row[0])
					elif int(row[1]) not in skims.zone_id_to_index_map:
						if row[1] not in ignored_zones: ignored_zones.append(row[1])
					else:
						o = skims.zone_id_to_index_map[int(row[0])]
						d = skims.zone_id_to_index_map[int(row[1])]
						skims.transit_fare[o,d] = float(row[2])
		except ValueError:
				print row[2]
	
	if '.txt' in transit_wait_file: delim = '\t'
	else: delim = ','
	with open(auto_distance_file, 'r') as infile:
		cr = csv.reader(infile,delimiter =delim)
		try:
			for row in cr:
				if len(row) > 2:
					if int(row[0]) not in skims.zone_id_to_index_map:
						if row[0] not in ignored_zones: ignored_zones.append(row[0])
					elif int(row[1]) not in skims.zone_id_to_index_map:
						if row[1] not in ignored_zones: ignored_zones.append(row[1])
					else:
						o = skims.zone_id_to_index_map[int(row[0])]
						d = skims.zone_id_to_index_map[int(row[1])]
						skims.auto_distance[o,d] = float(row[2])
		except ValueError:
				print row[2]

	print "Warning, the following tazs from skim .csv input were ignored as they are not in the skim_id_to_index_file:"
	for id in ignored_zones: print str(id)
	
#note highway travel time files and intervals are lists
def ReadHighwaySkims_CSV(highway_ttime_files, intervals, zone_id_to_index, skims):
	print "Reading highway skims from csv files..."

	# get the zone map
	with open(zone_id_to_index, 'r') as infile:
		cr = csv.reader(infile,delimiter =',')
		for row in cr:
			skims.zone_id_to_index_map[int(row[0])] = int(row[1])
			skims.zone_index_to_id_map[int(row[1])] = int(row[0])
			
	nzone = len(skims.zone_id_to_index_map)
	skims.num_zones = nzone
	skims.num_tzones = nzone
	
	# get the intervals
	for interval in intervals: skims.intervals.append(interval)
	print "Time intervals: " + str(intervals)
	
	# resize the matrices according to number of zones and add for each interval
	print "Resizing matrice..."
	skims.resize_arrays(nzone)
	
	ignored_zones = []
	
	interval_count = 0
	for highway_ttime_file in highway_ttime_files:
		interval = skims.intervals[interval_count]
		read_count = 0
		print "Reading data for interval ending in " + str(interval)
		with open(highway_ttime_file, 'r') as infile:
			cr = csv.reader(infile,delimiter =',')
			try:
				for row in cr:
					if len(row) > 2:
						if int(row[0]) not in skims.zone_id_to_index_map:
							if row[0] not in ignored_zones: ignored_zones.append(row[0])
						elif int(row[1]) not in skims.zone_id_to_index_map:
							if row[1] not in ignored_zones: ignored_zones.append(row[1])
						else:
							o = skims.zone_id_to_index_map[int(row[0])]
							d = skims.zone_id_to_index_map[int(row[1])]
							skims.auto_skims[interval][o,d] += float(row[2])
						read_count = read_count + 1
					if read_count % (nzone*50) == 0:
						print str("Reading for interval {} is {:.2%} complete.").format(interval, float(read_count) / float(nzone) / float(nzone) )
			except ValueError:
					print row[2]
			interval_count = interval_count + 1
	print "Warning, the following tazs from skim .csv input were ignored as they are not in the skim_id_to_index_file:"
	for id in ignored_zones: print str(id)

class Skim_Results:
	def __init__(self):
		self.auto_skims = {}
		self.zone_id_to_index_map = {}
		self.zone_index_to_id_map = {}
		self.transit_ttime = numpy.matrix(999999,dtype='f')
		self.transit_walk_access_time = numpy.matrix(0,dtype='f')
		self.auto_distance = numpy.matrix(999999,dtype='f')
		self.transit_wait_time = numpy.matrix(0,dtype='f')
		self.transit_fare = numpy.matrix(999999,dtype='f')
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
		
	def resize_arrays(self, nzone):
		self.transit_ttime = numpy.resize(self.transit_ttime,(nzone,nzone))
		self.transit_walk_access_time = numpy.resize(self.transit_walk_access_time,(nzone,nzone))
		self.transit_wait_time = numpy.resize(self.transit_wait_time,(nzone,nzone))
		self.transit_fare = numpy.resize(self.transit_fare,(nzone,nzone))
		self.auto_distance = numpy.resize(self.auto_distance,(nzone,nzone))

		for interval in self.intervals:
			m = numpy.matrix(0,dtype='f')
			m = numpy.resize(m,(nzone,nzone))
			self.auto_skims[interval] = m

			
skims = Skim_Results()

# parse the command line args
parser = argparse.ArgumentParser(description='Process the skim data')
parser.add_argument('-auto_skim_file', default='')
parser.add_argument('-transit_skim_file', default='')
parser.add_argument('-csv', action='store_const', const=1)
parser.add_argument('-tab', action='store_const', const=1)
parser.add_argument('-bin', action='store_const', const=1)
parser.add_argument('-origin_list', type=int, nargs='*')
parser.add_argument('-destination_list', type=int, nargs='*')
parser.add_argument('-read_from_csv', action='store_true')
parser.add_argument('-i1_transit_ttime_file', default = '')
parser.add_argument('-i2_transit_walk_files', nargs='*')
parser.add_argument('-i3_transit_wait_files', nargs='*')
parser.add_argument('-i4_transit_fare_file', default = '')
parser.add_argument('-i5_auto_distance_file',default='')
parser.add_argument('-zone_id_to_index_file', default = '')
parser.add_argument('-i6_highway_ttime_files',nargs='*')
parser.add_argument('-i7_highway_intervals',nargs='*',type=int)

args = parser.parse_args()

write_csv=False
if args.csv==1: write_csv=True
write_bin=False
if args.bin==1: write_bin=True
write_tab=False
if args.tab==1: write_tab=True
if write_csv==False and write_tab==False: write_bin=True

if (args.read_from_csv):
	write_bin = True
	write_csv = True
	if args.zone_id_to_index_file == '': raise NameError("Error: missing zone_id_to_index file")

	if args.i6_highway_ttime_files is not None:
		ReadHighwaySkims_CSV(args.i6_highway_ttime_files,args.i7_highway_intervals,args.zone_id_to_index_file,skims)
		WriteHighwaySkimsV1(args.auto_skim_file,skims)
		WriteHighwaySkimsV1_CSV(args.auto_skim_file,skims)

	if args.i1_transit_ttime_file != '':
		ReadTransitSkims_CSV(args.i1_transit_ttime_file,args.i2_transit_walk_files,args.i3_transit_wait_files,args.i4_transit_fare_file,args.i5_auto_distance_file, args.zone_id_to_index_file,skims)
		WriteTransitSkimsV1(args.transit_skim_file,skims)
		WriteTransitSkimsV1_CSV(args.transit_skim_file,skims)
else:
	Main(skims, args.auto_skim_file, args.transit_skim_file, write_bin, write_csv, write_tab, args.origin_list, args.destination_list)

