add_executable(Fixed_Demand_Simulator NetworkModel.cpp)	
target_link_libraries(Fixed_Demand_Simulator ${libtcmalloc} ${antareslibs} ${iolibs} Scenario_Manager Core)
set_target_properties(Fixed_Demand_Simulator PROPERTIES FOLDER apps)