set(io_headers Demand.h Supply.h Result.h System.h Network_Event.h Popsyn.h)

if (UNIX)
	# set(odb_path /mnt/lustre/app/polaris/bin/odb)
	set(odb_path ${DEPS_DIR}/odb-2.4.0-x86_64-linux-gnu/bin/odb)
else()
	# set(odb_path c:/opt/polarisdeps/odb-2.2.0-i686-windows/bin/odb) %%%RLW
	set(odb_path ${DEPS_DIR}/odb-2.4.0-i686-windows/bin/odb)
endif()

#file(GLOB headerfiles *.h)
set(headerfiles ${io_headers} Database.h Utils.h)


foreach (headerfile ${io_headers})
	message(${headerfile})
	string(REPLACE  ".h" "-odb.cxx" cxxfile ${headerfile})
	message(${cxxfile})
	string(REPLACE  ".h" "" schema ${headerfile})
	
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${cxxfile}
		COMMAND ${odb_path} 
		--std c++11 
		--database sqlite 
		--generate-query --generate-schema --generate-session
		--output-dir ${CMAKE_CURRENT_SOURCE_DIR}
		--default-pointer std::shared_ptr 
		--schema-name ${schema} 
		${CMAKE_CURRENT_SOURCE_DIR}/${headerfile}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${headerfile}
		COMMENT "Running odb compiler on ${headerfile}"
	)
	list(APPEND srcfiles ${cxxfile})
	#list(APPEND headerfiles ${headerfile})
endforeach()

add_library(Io ${srcfiles} ${headerfiles})
set_target_properties(Io PROPERTIES FOLDER libs)