add_executable(Integrated_Model Integrated_Model.cpp)	
target_link_libraries(Integrated_Model ${libtcmalloc} ${antareslibs} ${iolibs} Scenario_Manager Core)
set_target_properties(Integrated_Model PROPERTIES FOLDER apps)