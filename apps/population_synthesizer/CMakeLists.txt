add_executable(population_synthesizer population_synthesizer.cpp)	
target_link_libraries(population_synthesizer ${libtcmalloc} ${antareslibs} ${iolibs} Scenario_Manager Core)
set_target_properties(population_synthesizer PROPERTIES FOLDER apps)