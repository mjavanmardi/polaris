REPORT_FILE				z:/POLARIS/interchange/Chicago.log

#######z:/POLARIS/interchange/Run4_1##############
NODE_FILE		        z:/POLARIS/interchange/Run4_1/network/Node
LINK_FILE		        z:/POLARIS/interchange/Run4_1/network/Link
LOCATION_FILE		    z:/POLARIS/interchange/Run4_1/network/Location
CONNECTION_FILE	    	z:/POLARIS/interchange/Run4_1/network/Connection
PARKING_FILE	    	z:/POLARIS/interchange/Run4_1/network/Parking
ZONE_FILE		        z:/POLARIS/interchange/Run4_1/network/Zone
POCKET_FILE		        z:/POLARIS/interchange/Run4_1/network/Pocket
SHAPE_FILE		        z:/POLARIS/interchange/Run4_1/network/Shape
DETECTOR_FILE			z:/POLARIS/interchange/Run4_1/network/Detector
LANE_USE_FILE			z:/POLARIS/interchange/Run4_1/network/Lane_Use

#######DEMAND##############
TRIP_FILE		        z:/POLARIS/interchange/Run4_1/demand/regional_trips
VEHICLE_TYPE_FILE   	z:/POLARIS/interchange/Run4_1/inputs/vehicle_type_V5
VEHICLE_FILE		    z:/POLARIS/interchange/Run4_1/demand/regional_vehicle



#######CONTROL##############
SIGN_FILE		        z:/POLARIS/interchange/Run4_1/intcontrol/Sign
SIGNAL_FILE		        z:/POLARIS/interchange/Run4_1/intcontrol/Signal
TIMING_PLAN_FILE    	z:/POLARIS/interchange/Run4_1/intcontrol/timing_plan
PHASING_PLAN_FILE   	z:/POLARIS/interchange/Run4_1/intcontrol/phasing_plan

#######OUTPUT##############
NEW_SQLITE_DATABASE     z:/POLARIS/interchange/with_zone_data/chicago


#######MISC##############
MESSAGE 					Added Lane_Use and Detector tables per Hubert's request
ADD_GEOMETRY				TRUE
SRID						26916
