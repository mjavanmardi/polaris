#pragma once

#include "Activity_Simulator_Includes.h"


//---------------------------------------------------------
//	POP_PROPERTIES OBJECT CLASS (i.e. HH, PERSON, VEHICLE, FIRM, etc.)
//---------------------------------------------------------

namespace Person_Components
{
	namespace Types
	{
		// VARIOUS CENSUS ACS FILE CODES (NAME IN FILE given in comments)

		// SEX
		enum GENDER
		{
			MALE=1,
			FEMALE
		};
		// RACE1
		enum RACE
		{
			WHITE_ALONE=1,
			BLACK_ALONE,
			AMERICAN_INDIAN_ALONE,
			ALASKAN_NATIVE_ALONE,
			AMERICAN_INDIAN_OTHER,
			ASIAN_ALONE,
			PACIFIC_ISLANDER_ALONE,
			OTHER_RACE_ALONE,
			TWO_OR_MORE_RACES
		};
		// COW 
		enum CLASS_OF_WORKER
		{
			COW_NA,
			COW_EMPLOYEE,
			COW_EMPLOYEE_NONPROFIT,
			COW_EMPLOYEE_LOCAL_GOVERNMENT,
			COW_EMPLOYEE_STATE_GOVERNMENT,
			COW_EMPLOYEE_FEDERAL_GOVERNMENT,
			COW_SELF_EMPLOYED_UNINCORPORATED,
			COW_SELF_EMPLOYED_INCORPORATED,
			COW_WORK_FOR_FAMILY,
			COW_UNEMPLOYED
		};
		// ESR 
		enum EMPLOYMENT_STATUS
		{
			EMPLOYMENT_STATUS_NA,
			EMPLOYMENT_STATUS_CIVILIAN_AT_WORK,
			EMPLOYMENT_STATUS_CIVILIAN_NOT_AT_WORK,
			EMPLOYMENT_STATUS_UNEMPLOYED,
			EMPLOYMENT_STATUS_ARMED_FORCES_AT_WORK,
			EMPLOYMENT_STATUS_ARMED_FORCES_NOT_AT_WORK,
			EMPLOYMENT_STATUS_NOT_IN_LABOR_FORCE,
		};
		// MAR
		enum MARITAL_STATUS
		{
			MARRIED=1,
			WIDOWED,
			DIVORCED,
			SEPARATED,
			NEVER_MARRIED
		};
		// SCH
		enum SCHOOL_ENROLLMENT
		{
			ENROLLMENT_NA,
			ENROLLMENT_NONE,
			ENROLLMENT_PUBLIC,
			ENROLLMENT_PRIVATE
		};
		// SCHG
		enum SCHOOL_GRADE_LEVEL
		{
			GRADE_NA,
			GRADE_PRESCHOOL, GRADE_KINDERGARTEN,
			GRADE_1,GRADE_2,GRADE_3,GRADE_4,GRADE_5,GRADE_6,GRADE_7,GRADE_8,GRADE_9,GRADE_10,GRADE_11,GRADE_12,
			GRADE_UNDERGRADUATE, GRADE_GRADUATE
		};
		// SCHL
		enum EDUCATION_LEVEL
		{
			EDUC_NA, EDUC_NONE,
			EDUC_PRESCHOOL, EDUC_KINDERGARTEN,
			EDUC_1,EDUC_2,EDUC_3,EDUC_4,EDUC_5,EDUC_6,EDUC_7,EDUC_8,EDUC_9,EDUC_10,EDUC_11,EDUC_12,
			EDUC_UNDERGRADUATE, EDUC_GRADUATE
		};
		// PAOC
		enum PRESENCE_AGE_OWN_CHILDREN
		{
			OWN_CHILDREN_NA,
			OWN_CHILDREN_UNDER_6,
			OWN_CHILDREN_6_TO_17,
			OWN_CHILDREN_BOTH,
			OWN_CHILDREN_NONE
		};
		// JWTR
		enum JOURNEY_TO_WORK_MODE
		{
			WORKMODE_NA, WORKMODE_AUTOMOBILE,WORKMODE_BUS, WORKMODE_STREETCAR,WORKMODE_SUBWAY,WORKMODE_RAILROAD,WORKMODE_FERRY,WORKMODE_TAXI,WORKMODE_MOTORCYCLE,WORKMODE_BICYCLE,WORKMODE_WALK,WORKMODE_WORK_AT_HOME,WORKMODE_OTHER
		};
		// INDP - Industry
		enum EMPLOYMENT_INDUSTRY
		{
			INDUSTRY_NA=0,
			AGR_CROP_PRODUCTION=170,
			AGR_ANIMAL_PRODUCTION=180,
			AGR_FORESTRY_EXCEPT_LOGGING=190,
			AGR_LOGGING=270,
			AGR_FISHING=280,
			AGR_SUPPORT_ACTIVITIES_FOR_AGRICULTURE_AND_FORESTRY=290,
			EXT_OIL_AND_GAS_EXTRACTION=370,
			EXT_COAL_MINING=380,
			EXT_METAL_ORE_MINING=390,
			EXT_NONMETALLIC_MINERAL_MINING_AND_QUARRYING=470,
			EXT_NOT_SPECIFIED_TYPE_OF_MINING=480,
			EXT_SUPPORT_ACTIVITIES_FOR_MINING=490,
			UTL_ELECTRIC_POWER_GENERATION=570,
			UTL_NATURAL_GAS_DISTRIBUTION=580,
			UTL_ELECTRIC_AND_GAS=590,
			UTL_WATER=670,
			UTL_SEWAGE_TREATMENT_FACILITIES=680,
			UTL_NOT_SPECIFIED_UTILITIES=690,
			CON_CONSTRUCTION=770,
			MFG_ANIMAL_FOOD=1070,
			MFG_SUGAR_AND_CONFECTIONERY_PRODUCTS=1080,
			MFG_FRUIT_AND_VEGETABLE_PRESERVING_AND_SPECIALTY_FOODS=1090,
			MFG_DAIRY_PRODUCTS=1170,
			MFG_ANIMAL_SLAUGHTERING_AND_PROCESSING=1180,
			MFG_RETAIL_BAKERIES=1190,
			MFG_BAKERIES=1270,
			MFG_SEAFOOD_AND_OTHER_MISCELLANEOUS_FOODS=1280,
			MFG_NOT_SPECIFIED_FOOD_INDUSTRIES=1290,
			MFG_BEVERAGE=1370,
			MFG_TOBACCO=1390,
			MFG_FIBER=1470,
			MFG_FABRIC_MILLS=1480,
			MFG_TEXTILE_AND_FABRIC_FINISHING_AND_FABRIC_COATING_MILLS=1490,
			MFG_CARPET_AND_RUG_MILLS=1570,
			MFG_TEXTILE_PRODUCT_MILLS=1590,
			MFG_KNITTING_FABRIC_MILLS=1670,
			MFG_CUT_AND_SEW_APPAREL=1680,
			MFG_APPAREL_ACCESSORIES_AND_OTHER_APPAREL60=1690,
			MFG_FOOTWEAR=1770,
			MFG_LEATHER_TANNING_AND_FINISHING_AND_OTHER_ALLIED_PRODUCTS=1790,
			MFG_PULP=1870,
			MFG_PAPERBOARD_CONTAINERS_AND_BOXES=1880,
			MFG_MISCELLANEOUS_PAPER_AND_PULP_PRODUCTS=1890,
			MFG_PRINTING_AND_RELATED_SUPPORT_ACTIVITIES=1990,
			MFG_PETROLEUM_REFINING=2070,
			MFG_MISCELLANEOUS_PETROLEUM_AND_COAL_PRODUCTS=2090,
			MFG_RESIN=2170,
			MFG_AGRICULTURAL_CHEMICALS=2180,
			MFG_PHARMACEUTICALS_AND_MEDICINES=2190,
			MFG_PAINT=2270,
			MFG_SOAP=2280,
			MFG_INDUSTRIAL_AND_MISCELLANEOUS_CHEMICALS=2290,
			MFG_PLASTICS_PRODUCTS=2370,
			MFG_TIRES=2380,
			MFG_RUBBER_PRODUCTS=2390,
			MFG_POTTERY=2470,
			MFG_STRUCTURAL_CLAY_PRODUCTS=2480,
			MFG_GLASS_AND_GLASS_PRODUCTS=2490,
			MFG_CEMENT=2570,
			MFG_MISCELLANEOUS_NONMETALLIC_MINERAL_PRODUCTS=2590,
			MFG_IRON_AND_STEEL_MILLS_AND_STEEL_PRODUCTS=2670,
			MFG_ALUMINUM_PRODUCTION_AND_PROCESSING=2680,
			MFG_NONFERROUS_METAL=2690,
			MFG_FOUNDRIES=2770,
			MFG_METAL_FORGINGS_AND_STAMPINGS=2780,
			MFG_CUTLERY_AND_HAND_TOOLS=2790,
			MFG_STRUCTURAL_METALS=2870,
			MFG_MACHINE_SHOPS=2880,
			MFG_COATING=2890,
			MFG_ORDNANCE=2970,
			MFG_MISCELLANEOUS_FABRICATED_METAL_PRODUCTS=2980,
			MFG_NOT_SPECIFIED_METAL_INDUSTRIES=2990,
			MFG_AGRICULTURAL_IMPLEMENTS=3070,
			MFG_CONSTRUCTION=3080,
			MFG_COMMERCIAL_AND_SERVICE_INDUSTRY_MACHINERY=3090,
			MFG_METALWORKING_MACHINERY=3170,
			MFG_ENGINES=3180,
			MFG_MACHINERY=3190,
			MFG_NOT_SPECIFIED_MACHINERY=3290,
			MFG_COMPUTER_AND_PERIPHERAL_EQUIPMENT=3360,
			MFG_COMMUNICATIONS=3370,
			MFG_NAVIGATIONAL=3380,
			MFG_ELECTRONIC_COMPONENTS_AND_PRODUCTS=3390,
			MFG_HOUSEHOLD_APPLIANCES=3470,
			MFG_ELECTRIC_LIGHTING_AND_ELECTRICAL_EQUIPMENT_MANUFACTURING=3490,
			MFG_MOTOR_VEHICLES_AND_MOTOR_VEHICLE_EQUIPMENT=3570,
			MFG_AIRCRAFT_AND_PARTS61=3580,
			MFG_AEROSPACE_PRODUCTS_AND_PARTS=3590,
			MFG_RAILROAD_ROLLING_STOCK=3670,
			MFG_SHIP_AND_BOAT_BUILDING=3680,
			MFG_OTHER_TRANSPORTATION_EQUIPMENT=3690,
			MFG_SAWMILLS_AND_WOOD_PRESERVATION=3770,
			MFG_VENEER=3780,
			MFG_PREFABRICATED_WOOD_BUILDINGS_AND_MOBILE_HOMES=3790,
			MFG_MISCELLANEOUS_WOOD_PRODUCTS=3870,
			MFG_FURNITURE_AND_RELATED_PRODUCTS=3890,
			MFG_MEDICAL_EQUIPMENT_AND_SUPPLIES=3960,
			MFG_SPORTING_AND_ATHLETIC_GOODS=3970,
			MFG_MISCELLANEOUS_MANUFACTURING=3980,
			MFG_NOT_SPECIFIED_MANUFACTURING_INDUSTRIES=3990,
			WHL_MOTOR_VEHICLES=4070,
			WHL_FURNITURE_AND_HOME_FURNISHING_MERCHANT_WHOLESALERS=4080,
			WHL_LUMBER_AND_OTHER_CONSTRUCTION_MATERIALS_MERCHANT=4090,
			WHL_PROFESSIONAL_AND_COMMERCIAL_EQUIPMENT_AND_SUPPLIES=4170,
			WHL_METALS_AND_MINERALS=4180,
			WHL_ELECTRICAL_AND_ELECTRONIC_GOODS_MERCHANT_WHOLESALERS=4190,
			WHL_HARDWARE=4260,
			WHL_MACHINERY=4270,
			WHL_RECYCLABLE_MATERIAL_MERCHANT_WHOLESALERS=4280,
			WHL_MISCELLANEOUS_DURABLE_GOODS_MERCHANT_WHOLESALERS=4290,
			WHL_PAPER_AND_PAPER_PRODUCTS_MERCHANT_WHOLESALERS=4370,
			WHL_DRUGS=4380,
			WHL_APPAREL=4390,
			WHL_GROCERIES_AND_RELATED_PRODUCTS_MERCHANT_WHOLESALERS=4470,
			WHL_FARM_PRODUCT_RAW_MATERIALS_MERCHANT_WHOLESALERS=4480,
			WHL_PETROLEUM_AND_PETROLEUM_PRODUCTS_MERCHANT_WHOLESALERS=4490,
			WHL_ALCOHOLIC_BEVERAGES_MERCHANT_WHOLESALERS=4560,
			WHL_FARM_SUPPLIES_MERCHANT_WHOLESALERS=4570,
			WHL_MISCELLANEOUS_NONDURABLE_GOODS_MERCHANT_WHOLESALERS=4580,
			WHL_ELECTRONIC_MARKETS_AND_AGENTS_AND_BROKERS=4585,
			WHL_NOT_SPECIFIED_WHOLESALE_TRADE=4590,
			RET_AUTOMOBILE_DEALERS=4670,
			RET_OTHER_MOTOR_VEHICLE_DEALERS=4680,
			RET_AUTO_PARTS=4690,
			RET_FURNITURE_AND_HOME_FURNISHINGS_STORES=4770,
			RET_HOUSEHOLD_APPLIANCE_STORES=4780,
			RET_RADIO=4790,
			RET_BUILDING_MATERIAL_AND_SUPPLIES_DEALERS=4870,
			RET_HARDWARE_STORES=4880,
			RET_LAWN_AND_GARDEN_EQUIPMENT_AND_SUPPLIES_STORES=4890,
			RET_GROCERY_STORES=4970,
			RET_SPECIALTY_FOOD_STORES=4980,
			RET_BEER=4990,
			RET_PHARMACIES_AND_DRUG_STORES=5070,
			RET_HEALTH_AND_PERSONAL_CARE=5080,
			RET_GASOLINE_STATIONS=5090,
			RET_CLOTHING_STORES=5170,
			RET_SHOE_STORES=5180,
			RET_JEWELRY=5190,
			RET_SPORTING_GOODS=5270,
			RET_SEWING=5280,
			RET_MUSIC_STORES=5290,
			RET_BOOK_STORES_AND_NEWS_DEALERS=5370,
			RET_DEPARTMENT_AND_DISCOUNT_STORES=5380,
			RET_MISCELLANEOUS_GENERAL_MERCHANDISE_STORES=5390,
			RET_FLORISTS=5470,
			RET_OFFICE_SUPPLIES_AND_STATIONARY_STORES=5480,
			RET_USED_MERCHANDISE_STORES=5490,
			RET_GIFT=5570,
			RET_MISCELLANEOUS_RETAIL_STORES=5580,
			RET_ELECTRONIC_SHOPPING=5590,
			RET_ELECTRONIC_AUCTIONS=5591,
			RET_MAIL_ORDER_HOUSES=5592,
			RET_VENDING_MACHINE_OPERATORS=5670,
			RET_FUEL_DEALERS=5680,
			RET_OTHER_DIRECT_SELLING_ESTABLISHMENTS=5690,
			RET_NOT_SPECIFIED_RETAIL_TRADE=5790,
			TRN_AIR_TRANSPORTATION=6070,
			TRN_RAIL_TRANSPORTATION=6080,
			TRN_WATER_TRANSPORTATION=6090,
			TRN_TRUCK_TRANSPORTATION=6170,
			TRN_BUS_SERVICE_AND_URBAN_TRANSIT=6180,
			TRN_TAXI_AND_LIMOUSINE_SERVICE=6190,
			TRN_PIPELINE_TRANSPORTATION=6270,
			TRN_SCENIC_AND_SIGHTSEEING_TRANSPORTATION=6280,
			TRN_SERVICES_INCIDENTAL_TO_TRANSPORTATION=6290,
			TRN_POSTAL_SERVICE=6370,
			TRN_COURIERS_AND_MESSENGERS=6380,
			TRN_WAREHOUSING_AND_STORAGE=6390,
			INF_NEWSPAPER_PUBLISHERS=6470,
			INF_PERIODICAL=6480,
			INF_SOFTWARE_PUBLISHERS=6490,
			INF_MOTION_PICTURE_AND_VIDEO_INDUSTRIES=6570,
			INF_SOUND_RECORDING_INDUSTRIES=6590,
			INF_BROADCASTING=6670,
			INF_INTERNET_PUBLISHING_AND_BROADCASTING_AND_WEB_SEARCH=6672,
			INF_WIRED_TELECOMMUNICATIONS_CARRIERS=6680,
			INF_TELECOMMUNICATIONS=6690,
			INF_DATA_PROCESSING=6695,
			INF_LIBRARIES_AND_ARCHIVES=6770,
			INF_OTHER_INFORMATION_SERVICES=6780,
			FIN_BANKING_AND_RELATED_ACTIVITIES=6870,
			FIN_SAVINGS_INSTITUTIONS=6880,
			FIN_NON_DEPOSITORY_CREDIT_AND_RELATED_ACTIVITIES=6890,
			FIN_SECURITIES=6970,
			FIN_INSURANCE_CARRIERS_AND_RELATED_ACTIVITIES=6990,
			FIN_REAL_ESTATE=7070,
			FIN_AUTOMOTIVE_EQUIPMENT_RENTAL_AND_LEASING=7080,
			FIN_VIDEO_TAPE_AND_DISK_RENTAL=7170,
			FIN_OTHER_CONSUMER_GOODS_RENTAL=7180,
			FIN_COMMERCIAL=7190,
			PRF_LEGAL_SERVICES=7270,
			PRF_ACCOUNTING=7280,
			PRF_ARCHITECTURAL=7290,
			PRF_SPECIALIZED_DESIGN_SERVICES=7370,
			PRF_COMPUTER_SYSTEMS_DESIGN_AND_RELATED_SERVICES=7380,
			PRF_MANAGEMENT=7390,
			PRF_SCIENTIFIC_RESEARCH_AND_DEVELOPMENT_SERVICES=7460,
			PRF_ADVERTISING_AND_RELATED_SERVICES=7470,
			PRF_VETERINARY_SERVICES=7480,
			PRF_OTHER_PROFESSIONAL=7490,
			PRF_MANAGEMENT_OF_COMPANIES_AND_ENTERPRISES=7570,
			PRF_EMPLOYMENT_SERVICES=7580,
			PRF_BUSINESS_SUPPORT_SERVICES=7590,
			PRF_TRAVEL_ARRANGEMENTS_AND_RESERVATION_SERVICES=7670,
			PRF_INVESTIGATION_AND_SECURITY_SERVICES=7680,
			PRF_SERVICES_TO_BUILDINGS_AND_DWELLINGS=7690,
			PRF_LANDSCAPING_SERVICES=7770,
			PRF_OTHER_ADMINISTRATIVE_AND_OTHER_SUPPORT_SERVICES=7780,
			PRF_WASTE_MANAGEMENT_AND_REMEDIATION_SERVICES=7790,
			EDU_ELEMENTARY_AND_SECONDARY_SCHOOLS=7860,
			EDU_COLLEGES_AND_UNIVERSITIES=7870,
			EDU_BUSINESS=7880,
			EDU_OTHER_SCHOOLS_AND_INSTRUCTION=7890,
			MED_OFFICES_OF_PHYSICIANS=7970,
			MED_OFFICES_OF_DENTISTS=7980,
			MED_OFFICE_OF_CHIROPRACTORS=7990,
			MED_OFFICES_OF_OPTOMETRISTS=8070,
			MED_OFFICES_OF_OTHER_HEALTH_PRACTITIONERS=8080,
			MED_OUTPATIENT_CARE_CENTERS=8090,
			MED_HOME_HEALTH_CARE_SERVICES=8170,
			MED_OTHER_HEALTH_CARE_SERVICES=8180,
			MED_HOSPITALS=8190,
			MED_NURSING_CARE_FACILITIES=8270,
			MED_RESIDENTIAL_CARE_FACILITIES=8290,
			SCA_INDIVIDUAL_AND_FAMILY_SERVICES=8370,
			SCA_COMMUNITY_FOOD_AND_HOUSING=8380,
			SCA_VOCATIONAL_REHABILITATION_SERVICES=8390,
			SCA_CHILD_DAY_CARE_SERVICES=8470,
			ENT_INDEPENDENT_ARTISTS=8560,
			ENT_MUSEUMS=8570,
			ENT_BOWLING_CENTERS64=8580,
			ENT_OTHER_AMUSEMENT=8590,
			ENT_TRAVELER_ACCOMMODATION=8660,
			ENT_RECREATIONAL_VEHICLE_PARKS_AND_CAMPS=8670,
			ENT_RESTAURANTS_AND_OTHER_FOOD_SERVICES=8680,
			ENT_DRINKING_PLACES=8690,
			SRV_AUTOMOTIVE_REPAIR_AND_MAINTENANCE=8770,
			SRV_CAR_WASHES=8780,
			SRV_ELECTRONIC_AND_PRECISION_EQUIPMENT_REPAIR_AND=8790,
			SRV_COMMERCIAL_AND_INDUSTRIAL_MACHINERY_AND_EQUIPMENT=8870,
			SRV_PERSONAL_AND_HOUSEHOLD_GOODS_REPAIR_AND_MAINTENANCE=8880,
			SRV_BARBER_SHOPS=8970,
			SRV_BEAUTY_SALONS=8980,
			SRV_NAIL_SALONS_AND_OTHER_PERSONAL_CARE_SERVICES=8990,
			SRV_DRYCLEANING_AND_LAUNDRY_SERVICES=9070,
			SRV_FUNERAL_HOMES=9080,
			SRV_OTHER_PERSONAL_SERVICES=9090,
			SRV_RELIGIOUS_ORGANIZATIONS=9160,
			SRV_CIVIC=9170,
			SRV_LABOR_UNIONS=9180,
			SRV_BUSINESS=9190,
			SRV_PRIVATE_HOUSEHOLDS=9290,
			ADM_EXECUTIVE_OFFICES_AND_LEGISLATIVE_BODIES=9370,
			ADM_PUBLIC_FINANCE_ACTIVITIES=9380,
			ADM_OTHER_GENERAL_GOVERNMENT_AND_SUPPORT=9390,
			ADM_JUSTICE=9470,
			ADM_ADMINISTRATION_OF_HUMAN_RESOURCE_PROGRAMS=9480,
			ADM_ADMINISTRATION_OF_ENVIRONMENTAL_QUALITY_AND_HOUSING=9490,
			ADM_ADMINISTRATION_OF_ECONOMIC_PROGRAMS_AND_SPACE_RESEARCH=9570,
			ADM_NATIONAL_SECURITY_AND_INTERNATIONAL_AFFAIRS=9590,
			MIL_ARMY=9670,
			MIL_AIR_FORCE=9680,
			MIL_NAVY=9690,
			MIL_MARINES=9770,
			MIL_COAST_GUARD=9780,
			MIL_ARMED_FORCES=9790,
			MIL_MILITARY_RESERVES_OR_NATIONAL_GUARD=9870,
			INDUSTRY_UNEMPLOYED=9920,	
		};
		enum EMPLOYMENT_INDUSTRY_BASE
		{
			IND_NA,
			IND_AGRICULTURE,
			IND_EXTRACTION,
			IND_UTILITIES,
			IND_CONSTRUCTION,
			IND_MANUFACTURING,
			IND_WHOLESALE,
			IND_RETAIL,
			IND_TRANSPORTATION,
			IND_INFORMATION,
			IND_FINANCE,
			IND_REALESTATE,
			IND_PROFESSIONAL,
			IND_EDUCATION,
			IND_MEDICAL,
			IND_SOCIALWORK,
			IND_ENTERTAINMENT,
			IND_SERVICES,
			IND_ADMINISTRATION,
			IND_MILITARY,
			IND_UNEMPLOYED
		};
		enum EMPLOYMENT_INDUSTRY_SIMPLE
		{
			RETAIL,
			GOVERNMENT,
			SERVICE,
			INDUSTRIAL,
			MANUFACTURING,
			OTHER
		};
		enum TELECOMMUTE_FREQUENCY
		{
			TC_NEVER = 0,
			TC_YEARLY,
			TC_MONTHLY,
			TC_WEEKLY,
			TC_DAILY
		};
	}

	namespace Concepts
	{
		concept struct Is_Census_ACS_Compliant
		{
			// component versions
			check_data_member_type(has_gender,Gender, Types::GENDER);
			check_accessor_name(has_race,Race);
			check_data_member_type(has_employment_status,Employment_Status, Types::EMPLOYMENT_STATUS);

			check_accessor_name(has_work_travel_time,Journey_To_Work_Travel_Time);
			check_data_member_type(has_work_travel_mode,Journey_To_Work_Mode, Types::JOURNEY_TO_WORK_MODE);
			check_accessor_name(has_work_arrival_time,Journey_To_Work_Arrival_Time);

			// prototype versions
			check_data_member_type(has_gender_prototype,Component_Type::Gender, Types::GENDER);
			check_component_accessor_name(has_race_prototype,Race);
			check_data_member_type(has_employment_status_prototype,Component_Type::Employment_Status, Types::EMPLOYMENT_STATUS);

			check_component_accessor_name(has_work_travel_time_prototype,Journey_To_Work_Travel_Time);
			check_data_member_type(has_work_travel_mode_prototype,Component_Type::Journey_To_Work_Mode, Types::JOURNEY_TO_WORK_MODE);
			check_component_accessor_name(has_work_arrival_time_prototype,Journey_To_Work_Arrival_Time);

			// concept checks
			define_sub_check(has_demographics, (has_gender && has_race && has_employment_status) || (has_gender_prototype && has_race_prototype && has_employment_status_prototype));
			define_sub_check(has_journey_to_work_info, (has_work_travel_time && has_work_travel_mode && has_work_arrival_time) || (has_work_travel_time_prototype && has_work_travel_mode_prototype && has_work_arrival_time_prototype));
			define_default_check(has_demographics && has_journey_to_work_info);
		};
	}

	
	namespace Prototypes
	{
		prototype struct Person_Properties ADD_DEBUG_INFO
		{
			tag_as_prototype;

			typedef Person_Properties<ComponentType> This_Type;
			
			// accessor to parent class
			accessor(Parent_Person, NONE, NONE);

			// basic properties
			accessor(ID, NONE, NONE);
			accessor(Weight, NONE, NONE);
			accessor(Index, NONE, NONE);
			accessor(Test_Index, NONE, NONE);
			//accessor(Characteristics, NONE, NONE);

			// Static Census ACS type properties
			accessor(Gender, NONE, NONE);
			accessor(Race, NONE, NONE);
			accessor(Age, NONE, NONE);
			accessor(Class_of_worker, NONE, NONE);
			accessor(Journey_To_Work_Travel_Time, NONE, NONE);
			accessor(Journey_To_Work_Vehicle_Occupancy, NONE, NONE);
			accessor(Journey_To_Work_Mode, NONE, NONE);
			accessor(Marital_Status, NONE, NONE);
			accessor(School_Enrollment, NONE, NONE);
			accessor(School_Grade_Level, NONE, NONE);
			accessor(Educational_Attainment, NONE, NONE);
			accessor(Work_Hours, NONE, NONE);
			accessor(Employment_Status, NONE, NONE);
			accessor(Employment_Industry, NONE, NONE);	
			accessor(Journey_To_Work_Arrival_Time, check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value), NONE);
			accessor(Income, NONE, NONE);

			accessor(Telecommute_Frequency, NONE, NONE);

			// Non-static properties
			accessor(work_location_id, NONE, NONE);
			accessor(school_location_id, NONE, NONE);

			local_check_template_method_name(Average_Activity_Frequency_exists,Average_Activity_Frequency);
			template<typename TargetType, typename ReturnType, typename CType> ReturnType Average_Activity_Frequency(TargetType act_type)
			{
				assert_check(ComponentType,Average_Activity_Frequency_exists, "Component has no Average_Activity_Frequency.");

				return this_component()->Average_Activity_Frequency<TargetType, ReturnType>(act_type);
			}

			template<typename TargetType, typename TimeType> void Average_Activity_Frequency(TargetType act_type, TimeType set_value)
			{
				assert_check(ComponentType,Average_Activity_Frequency_exists, "Component has no Average_Activity_Frequency.");

				this_component()->Average_Activity_Frequency<TargetType, TimeType>(act_type, set_value);
			}


			local_check_template_method_name(Average_Activity_Duration_exists,Average_Activity_Duration);
			template<typename TargetType, typename ReturnType> ReturnType Average_Activity_Duration(TargetType act_type)
			{
				assert_check(ComponentType,Average_Activity_Duration_exists, "Component has no Average_Activity_Duration.");

				return this_component()->Average_Activity_Duration<TargetType, ReturnType>(act_type);
			}

			template<typename TargetType, typename TimeType> void Average_Activity_Duration(TargetType act_type, TimeType set_value)
			{
				assert_check(ComponentType,Average_Activity_Duration_exists, "Component has no Average_Activity_Duration.");

				this_component()->Average_Activity_Duration<TargetType, TimeType>(act_type, set_value);
			}

			
			// Pass through methods
			local_check_template_method_name(Initialize_exists,Initialize);
			template<typename TargetType> void Initialize()
			{
				assert_check(ComponentType,Initialize_exists,"ComponentType does not have 'Initialize' method defined.");

				this_component()->Initialize<TargetType>();
			}
			template<typename TargetType> void Initialize(TargetType home_zone)
			{
				assert_check(ComponentType,Initialize_exists,"ComponentType does not have 'Initialize' method defined.");

				this_component()->Initialize<TargetType>(home_zone);
			}

			local_check_template_method_name(Set_Locations_exists, Set_Locations);
			template<typename TargetType> void Set_Locations()
			{
				assert_check(ComponentType, Set_Locations_exists, "ComponentType does not have 'Set_Locations' method defined.");

				this_component()->Set_Locations<TargetType>();
			}


			local_check_template_method_name(Is_Employed_exists,Is_Employed);
			template<typename TargetType> bool Is_Employed()
			{
				assert_check(ComponentType,Is_Employed_exists,"ComponentType does not have 'Is_Employed' method defined.");

				return this_component()->Is_Employed<TargetType>();
			}
			bool Is_Fulltime_Worker()
			{
				return this_component()->Is_Employed<NT>() && this_component()->Work_Hours<Time_Minutes>() >= 30;

			}
			bool Is_Parttime_Worker()
			{
				return this_component()->Is_Employed<NT>() && this_component()->Work_Hours<Time_Minutes>() < 30;
			}

			local_check_template_method_name(Is_Student_exists,Is_Student);
			template<typename TargetType> bool Is_Student()
			{
				assert_check(ComponentType,Is_Student_exists,"ComponentType does not have 'Is_Student' method defined.");

				return this_component()->Is_Student<TargetType>();
			}

			template<typename TargetType> TargetType Value_of_Travel_Time_Adjustment()
			{
				return this_component()->Value_of_Travel_Time_Adjustment<TargetType>();
			}
			
			local_check_template_method_name(Characteristics_exists,Characteristics);
			template<typename TargetType> void Characteristics(TargetType data, requires(TargetType, check_2(TargetType, std::vector<double>*, is_same)))
			{
				assert_check(ComponentType,Characteristics_exists,"ComponentType does not have 'Characteristics' method defined.");

				this_component()->Characteristics<TargetType>(data);
			}
			template<typename TargetType> void Characteristics(TargetType data, requires(TargetType, !check_2(TargetType, std::vector<double>*, is_same)))
			{
				assert_check_2(TargetType, std::vector<double>*, is_same, "TargetType must be specified as a pointer to a vector of doubles.");
			}

			template<typename TargetType> Types::EMPLOYMENT_INDUSTRY_BASE Employment_Industry_Base()
			{
				int i = this->Employment_Industry<Types::EMPLOYMENT_INDUSTRY>();

				if (i <= 290) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_AGRICULTURE;
				else if (i <= 490) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_EXTRACTION;
				else if (i <= 690) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_UTILITIES;
				else if (i <= 770) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_CONSTRUCTION;
				else if (i <= 3990) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_MANUFACTURING;
				else if (i <= 4590) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_WHOLESALE;
				else if (i <= 5790) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_RETAIL;
				else if (i <= 6390) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_TRANSPORTATION;
				else if (i <= 6780) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_INFORMATION;
				else if (i <= 6990) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_FINANCE;
				else if (i <= 7190) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_REALESTATE;
				else if (i <= 7790) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_PROFESSIONAL;
				else if (i <= 7890) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_EDUCATION;
				else if (i <= 8290) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_MEDICAL;
				else if (i <= 8470) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_SOCIALWORK;
				else if (i <= 8690) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_ENTERTAINMENT;
				else if (i <= 9290) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_SERVICES;
				else if (i <= 9590) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_ADMINISTRATION;
				else if (i <= 9870) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_MILITARY;
				else if (i <= 9920) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_UNEMPLOYED;
				else return Types::EMPLOYMENT_INDUSTRY_BASE::IND_NA;
			}
			template<typename TargetType> Types::EMPLOYMENT_INDUSTRY_SIMPLE Employment_Industry_Simple()
			{
				int i = this->Employment_Industry<Types::EMPLOYMENT_INDUSTRY>();

				if (i <= 770 || (i >= 6070 && i <= 6390)) return Types::EMPLOYMENT_INDUSTRY_SIMPLE::INDUSTRIAL;
				else if (i <= 3990) return Types::EMPLOYMENT_INDUSTRY_SIMPLE::MANUFACTURING;
				else if (i <= 5790) return Types::EMPLOYMENT_INDUSTRY_SIMPLE::RETAIL;
				else if (i <= 7790 || (i >= 8660 && i <= 9290)) return Types::EMPLOYMENT_INDUSTRY_SIMPLE::SERVICE;
				else if ((i >= 7860 && i <= 7890) || (i >= 9370 && i <= 9870)) return Types::EMPLOYMENT_INDUSTRY_SIMPLE::GOVERNMENT;
				else return Types::EMPLOYMENT_INDUSTRY_SIMPLE::OTHER;
			}

			template<typename TargetType> void Normalize_Weight(TargetType normalization_factor, requires(TargetType,check(strip_modifiers(TargetType),is_arithmetic)))
			{
				this->Weight<TargetType>(this->Weight<TargetType>()/normalization_factor);
			}
			template<typename TargetType> void Normalize_Weight(TargetType normalization_factor, requires(TargetType,!check(strip_modifiers(TargetType),is_arithmetic)))
			{
				assert_check(strip_modifiers(TargetType),is_arithmetic,"The specified TargetType must be arithmetic to use this function");
			}
		};
	}
}

using namespace Person_Components::Types;
using namespace Person_Components::Prototypes;
