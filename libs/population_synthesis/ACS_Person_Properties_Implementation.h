#pragma once

#include "activity_simulator/Person_Properties_Prototype.h"
//#include "Person_Prototype.h"
//#include "Movement_Plan_Prototype.h"
//#include "Network_Skimming_Prototype.h"
//#include "Activity_Prototype.h"
//#include "Population_Unit_Implementations.h"
//#include "Person_Implementations.h"

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Properties classes
		//----------------------------------------------------------------------------------
		implementation struct ACS_Person_Static_Properties_Implementation : public Polaris_Component< MasterType,INHERIT(ACS_Person_Static_Properties_Implementation), Data_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(ACS_Person_Static_Properties_Implementation),Data_Object>::Component_Type ComponentType;

			//=================================================================
			// Basic Person Characteristics Used in Popsyn algorithms
			m_data(double,ID, NONE, NONE);
			m_data(double,Weight, NONE, NONE);
			m_data(uint,Index, NONE, NONE);				 //index into the joint-distribution matrix of the region (convert using region.get_index())
			m_data(uint,Test_Index, NONE, NONE);		//index into the test marginal distribution for this person
			//m_container(std::vector<double>, Characteristics, NONE, NONE);

			//=================================================================
			// Census specific individual data, used in ABM routines
			m_data(Types::GENDER, Gender, NONE, NONE);
			m_data(Types::RACE, Race, NONE, NONE);	
			m_data(Types::CLASS_OF_WORKER, Class_of_worker, NONE, NONE);
			m_data(Types::JOURNEY_TO_WORK_MODE, Journey_To_Work_Mode, NONE, NONE);		
			m_data(Types::MARITAL_STATUS, Marital_Status, NONE, NONE);
			m_data(Types::SCHOOL_ENROLLMENT, School_Enrollment, NONE, NONE);
			m_data(Types::SCHOOL_GRADE_LEVEL, School_Grade_Level, NONE, NONE);
			m_data(Types::EDUCATION_LEVEL, Educational_Attainment, NONE, NONE);	
			m_data(Types::EMPLOYMENT_STATUS, Employment_Status, NONE, NONE);
			m_data(Types::EMPLOYMENT_INDUSTRY, Employment_Industry, NONE, NONE);	
			template<typename TargetType> bool Is_Employed()
			{
				bool is_employed = false;
				switch(this->_Employment_Status)
				{
					case EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_CIVILIAN_AT_WORK :
					case EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_ARMED_FORCES_AT_WORK :
						is_employed=true;
						break;
					default:
						is_employed=false;
				}
				return is_employed;
			}
			template<typename TargetType> bool Is_Student()
			{
				bool is_student = false;
				switch(this->_School_Enrollment)
				{
					case SCHOOL_ENROLLMENT::ENROLLMENT_PRIVATE :
					case SCHOOL_ENROLLMENT::ENROLLMENT_PUBLIC :
						is_student=true;
						break;
					default:
						is_student=false;
				}
				return is_student;
			}

			m_data(int, Journey_To_Work_Vehicle_Occupancy, NONE, NONE);
			m_data(int, Age, NONE, NONE);
			
			member_component_and_feature_accessor(Work_Hours, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(Journey_To_Work_Travel_Time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(Income, Value, Basic_Units::Prototypes::Currency, Basic_Units::Implementations::Currency_Implementation<NT>);


			// work arrival time conversion functions
			m_data(Basic_Units::Implementations::Time_Implementation<NT>,_Journey_To_Work_Arrival_Time, NONE, NONE);
			template<typename TargetType> void Journey_To_Work_Arrival_Time(TargetType value, requires(TargetType,check(TargetType,Basic_Units::Concepts::Is_Time_Value)))
			{
				typedef Basic_Units::Prototypes::Time<type_of(typename ComponentType::_Journey_To_Work_Arrival_Time)> _Journey_To_Work_Arrival_Time_itf;
				_Journey_To_Work_Arrival_Time_itf* itf = this->template _Journey_To_Work_Arrival_Time<_Journey_To_Work_Arrival_Time_itf*>();
				itf->template Value<TargetType>(value);
			}
			template<typename TargetType> void Journey_To_Work_Arrival_Time(TargetType CENSUS_CODE, requires(TargetType,!check(TargetType,Basic_Units::Concepts::Is_Time_Value)))
			{
				int val=0;
				#pragma region CENSUS_CODE_SWITCH
				switch(CENSUS_CODE)
				{
					case 1:  val = (int)(GLOBALS::Uniform_RNG.template Next_Rand<float>() * 1440.0f); break;
					case 2:  val=5; break;
					case 3:  val=10; break;
					case 4:  val=15; break;
					case 5:  val=20; break;
					case 6:  val=25; break;
					case 7:  val=30; break;
					case 8:  val=40; break;
					case 9:  val=45; break;
					case 10:  val=50; break;
					case 11:  val=60; break;
					case 12:  val=65; break;
					case 13:  val=70; break;
					case 14:  val=75; break;
					case 15:  val=80; break;
					case 16:  val=85; break;
					case 17:  val=90; break;
					case 18:  val=95; break;
					case 19:  val=100; break;
					case 20:  val=105; break;
					case 21:  val=110; break;
					case 22:  val=120; break;
					case 23:  val=125; break;
					case 24:  val=130; break;
					case 25:  val=135; break;
					case 26:  val=140; break;
					case 27:  val=145; break;
					case 28:  val=150; break;
					case 29:  val=155; break;
					case 30:  val=160; break;
					case 31:  val=165; break;
					case 32:  val=170; break;
					case 33:  val=175; break;
					case 34:  val=180; break;
					case 35:  val=185; break;
					case 36:  val=190; break;
					case 37:  val=195; break;
					case 38:  val=200; break;
					case 39:  val=205; break;
					case 40:  val=210; break;
					case 41:  val=215; break;
					case 42:  val=220; break;
					case 43:  val=225; break;
					case 44:  val=230; break;
					case 45:  val=235; break;
					case 46:  val=240; break;
					case 47:  val=245; break;
					case 48:  val=250; break;
					case 49:  val=255; break;
					case 50:  val=260; break;
					case 51:  val=265; break;
					case 52:  val=270; break;
					case 53:  val=275; break;
					case 54:  val=280; break;
					case 55:  val=285; break;
					case 56:  val=290; break;
					case 57:  val=295; break;
					case 58:  val=300; break;
					case 59:  val=305; break;
					case 60:  val=310; break;
					case 61:  val=315; break;
					case 62:  val=320; break;
					case 63:  val=325; break;
					case 64:  val=330; break;
					case 65:  val=335; break;
					case 66:  val=340; break;
					case 67:  val=345; break;
					case 68:  val=350; break;
					case 69:  val=355; break;
					case 70:  val=360; break;
					case 71:  val=365; break;
					case 72:  val=370; break;
					case 73:  val=375; break;
					case 74:  val=380; break;
					case 75:  val=385; break;
					case 76:  val=390; break;
					case 77:  val=395; break;
					case 78:  val=400; break;
					case 79:  val=405; break;
					case 80:  val=410; break;
					case 81:  val=415; break;
					case 82:  val=420; break;
					case 83:  val=425; break;
					case 84:  val=430; break;
					case 85:  val=435; break;
					case 86:  val=440; break;
					case 87:  val=445; break;
					case 88:  val=450; break;
					case 89:  val=455; break;
					case 90:  val=460; break;
					case 91:  val=465; break;
					case 92:  val=470; break;
					case 93:  val=475; break;
					case 94:  val=480; break;
					case 95:  val=485; break;
					case 96:  val=490; break;
					case 97:  val=495; break;
					case 98:  val=500; break;
					case 99:  val=505; break;
					case 100:  val=510; break;
					case 101:  val=515; break;
					case 102:  val=520; break;
					case 103:  val=525; break;
					case 104:  val=530; break;
					case 105:  val=535; break;
					case 106:  val=540; break;
					case 107:  val=545; break;
					case 108:  val=550; break;
					case 109:  val=555; break;
					case 110:  val=560; break;
					case 111:  val=565; break;
					case 112:  val=570; break;
					case 113:  val=575; break;
					case 114:  val=580; break;
					case 115:  val=585; break;
					case 116:  val=590; break;
					case 117:  val=595; break;
					case 118:  val=600; break;
					case 119:  val=605; break;
					case 120:  val=610; break;
					case 121:  val=615; break;
					case 122:  val=620; break;
					case 123:  val=625; break;
					case 124:  val=630; break;
					case 125:  val=635; break;
					case 126:  val=640; break;
					case 127:  val=645; break;
					case 128:  val=650; break;
					case 129:  val=655; break;
					case 130:  val=660; break;
					case 131:  val=665; break;
					case 132:  val=670; break;
					case 133:  val=675; break;
					case 134:  val=680; break;
					case 135:  val=685; break;
					case 136:  val=690; break;
					case 137:  val=695; break;
					case 138:  val=700; break;
					case 139:  val=705; break;
					case 140:  val=710; break;
					case 141:  val=715; break;
					case 142:  val=720; break;
					case 143:  val=725; break;
					case 144:  val=730; break;
					case 145:  val=735; break;
					case 146:  val=740; break;
					case 147:  val=745; break;
					case 148:  val=750; break;
					case 149:  val=755; break;
					case 150:  val=760; break;
					case 151:  val=765; break;
					case 152:  val=770; break;
					case 153:  val=775; break;
					case 154:  val=780; break;
					case 155:  val=785; break;
					case 156:  val=790; break;
					case 157:  val=795; break;
					case 158:  val=800; break;
					case 159:  val=805; break;
					case 160:  val=810; break;
					case 161:  val=815; break;
					case 162:  val=820; break;
					case 163:  val=825; break;
					case 164:  val=830; break;
					case 165:  val=835; break;
					case 166:  val=840; break;
					case 167:  val=845; break;
					case 168:  val=850; break;
					case 169:  val=855; break;
					case 170:  val=860; break;
					case 171:  val=865; break;
					case 172:  val=870; break;
					case 173:  val=875; break;
					case 174:  val=880; break;
					case 175:  val=885; break;
					case 176:  val=890; break;
					case 177:  val=895; break;
					case 178:  val=900; break;
					case 179:  val=905; break;
					case 180:  val=910; break;
					case 181:  val=915; break;
					case 182:  val=920; break;
					case 183:  val=925; break;
					case 184:  val=930; break;
					case 185:  val=935; break;
					case 186:  val=940; break;
					case 187:  val=945; break;
					case 188:  val=950; break;
					case 189:  val=955; break;
					case 190:  val=960; break;
					case 191:  val=965; break;
					case 192:  val=970; break;
					case 193:  val=975; break;
					case 194:  val=980; break;
					case 195:  val=985; break;
					case 196:  val=990; break;
					case 197:  val=995; break;
					case 198:  val=1000; break;
					case 199:  val=1005; break;
					case 200:  val=1010; break;
					case 201:  val=1015; break;
					case 202:  val=1020; break;
					case 203:  val=1025; break;
					case 204:  val=1030; break;
					case 205:  val=1035; break;
					case 206:  val=1040; break;
					case 207:  val=1045; break;
					case 208:  val=1050; break;
					case 209:  val=1055; break;
					case 210:  val=1060; break;
					case 211:  val=1065; break;
					case 212:  val=1070; break;
					case 213:  val=1075; break;
					case 214:  val=1080; break;
					case 215:  val=1085; break;
					case 216:  val=1090; break;
					case 217:  val=1095; break;
					case 218:  val=1100; break;
					case 219:  val=1105; break;
					case 220:  val=1110; break;
					case 221:  val=1115; break;
					case 222:  val=1120; break;
					case 223:  val=1125; break;
					case 224:  val=1130; break;
					case 225:  val=1135; break;
					case 226:  val=1140; break;
					case 227:  val=1145; break;
					case 228:  val=1150; break;
					case 229:  val=1155; break;
					case 230:  val=1160; break;
					case 231:  val=1165; break;
					case 232:  val=1170; break;
					case 233:  val=1175; break;
					case 234:  val=1180; break;
					case 235:  val=1185; break;
					case 236:  val=1190; break;
					case 237:  val=1195; break;
					case 238:  val=1200; break;
					case 239:  val=1205; break;
					case 240:  val=1210; break;
					case 241:  val=1215; break;
					case 242:  val=1220; break;
					case 243:  val=1225; break;
					case 244:  val=1230; break;
					case 245:  val=1235; break;
					case 246:  val=1240; break;
					case 247:  val=1245; break;
					case 248:  val=1250; break;
					case 249:  val=1255; break;
					case 250:  val=1260; break;
					case 251:  val=1265; break;
					case 252:  val=1270; break;
					case 253:  val=1275; break;
					case 254:  val=1280; break;
					case 255:  val=1285; break;
					case 256:  val=1290; break;
					case 257:  val=1295; break;
					case 258:  val=1300; break;
					case 259:  val=1305; break;
					case 260:  val=1310; break;
					case 261:  val=1315; break;
					case 262:  val=1320; break;
					case 263:  val=1325; break;
					case 264:  val=1330; break;
					case 265:  val=1335; break;
					case 266:  val=1340; break;
					case 267:  val=1345; break;
					case 268:  val=1350; break;
					case 269:  val=1355; break;
					case 270:  val=1360; break;
					case 271:  val=1365; break;
					case 272:  val=1370; break;
					case 273:  val=1375; break;
					case 274:  val=1380; break;
					case 275:  val=1385; break;
					case 276:  val=1390; break;
					case 277:  val=1395; break;
					case 278:  val=1400; break;
					case 279:  val=1405; break;
					case 280:  val=1410; break;
					case 281:  val=1415; break;
					case 282:  val=1420; break;
					case 283:  val=1425; break;
					case 284:  val=1430; break;
					case 285:  val=1435; break;
					default: val = 420 + (int)(GLOBALS::Uniform_RNG.template Next_Rand<float>() * 120.0f); /*THROW_WARNING("Warning, arrival time case value '"<<CENSUS_CODE<<"' found in census data input file is not a valid arrival time to work code.  Arrival time defaulting to 8:00 AM.");*/
				}
				#pragma endregion

				typedef Basic_Units::Prototypes::Time<type_of(typename ComponentType::_Journey_To_Work_Arrival_Time)> _Journey_To_Work_Arrival_Time_itf;
				_Journey_To_Work_Arrival_Time_itf* itf = this->template _Journey_To_Work_Arrival_Time<_Journey_To_Work_Arrival_Time_itf*>();
				itf->template Value<Time_Minutes>(val);
			}
			template<typename TargetType> TargetType Journey_To_Work_Arrival_Time()
			{
				typedef Basic_Units::Prototypes::Time<type_of(typename ComponentType::_Journey_To_Work_Arrival_Time)> _Journey_To_Work_Arrival_Time_itf;
				_Journey_To_Work_Arrival_Time_itf* itf = this->template _Journey_To_Work_Arrival_Time<_Journey_To_Work_Arrival_Time_itf*>();
				return itf->template Value<TargetType>();
			}
			tag_getter_setter_as_available(Journey_To_Work_Arrival_Time);

			// Characteristics setter
			template<typename TargetType> void Characteristics(std::vector<double>* data)
			{
				if (data->size() != 16) THROW_EXCEPTION("ERROR: linker file definition out of date.")
				// these setters correspond exactly to the ACS-PUMS definitions and layout as given in pums_file.txt.  if pumsfile changes change these functions
				typedef Prototypes::Person_Properties<ComponentType> this_itf;
				this_itf* pthis = (this_itf*)this;
				
				pthis->Age<int>((*data)[0]);
				pthis->Class_of_worker<Types::CLASS_OF_WORKER>((Types::CLASS_OF_WORKER)(int)(*data)[1]);
				pthis->Educational_Attainment<Types::EDUCATION_LEVEL>((Types::EDUCATION_LEVEL)(int)(*data)[2]);
				pthis->Employment_Industry<Types::EMPLOYMENT_INDUSTRY>((Types::EMPLOYMENT_INDUSTRY)(int)(*data)[3]);
				pthis->Employment_Status<Types::EMPLOYMENT_STATUS>((Types::EMPLOYMENT_STATUS)(int)(*data)[4]);
				pthis->Gender<Types::GENDER>((Types::GENDER)(int)(*data)[5]);
				pthis->Income<Basic_Units::Currency_Variables::Dollars>((*data)[6]);
				pthis->Journey_To_Work_Arrival_Time<int>((*data)[7]);
				pthis->Journey_To_Work_Mode<Types::JOURNEY_TO_WORK_MODE>((Types::JOURNEY_TO_WORK_MODE)(int)(*data)[8]);
				pthis->Journey_To_Work_Travel_Time<Time_Minutes>((*data)[9]);
				pthis->Journey_To_Work_Vehicle_Occupancy<int>((*data)[10]);
				pthis->Marital_Status<Types::MARITAL_STATUS>((Types::MARITAL_STATUS)(int)(*data)[11]);
				pthis->Race<Types::RACE>((Types::RACE)(int)(*data)[12]);
				pthis->School_Enrollment<Types::SCHOOL_ENROLLMENT>((Types::SCHOOL_ENROLLMENT)(int)(*data)[13]);
				pthis->School_Grade_Level<Types::SCHOOL_GRADE_LEVEL>((Types::SCHOOL_GRADE_LEVEL)(int)(*data)[14]);
				pthis->Work_Hours<Time_Hours>((*data)[15]);
			}
		};

	}
}

