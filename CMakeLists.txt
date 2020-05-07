cmake_minimum_required (VERSION 3.9)
project (termlib VERSION 1.0.0 DESCRIPTION "A simple text UI in C")

#[[ Building Library ]]

include(GNUInstallDirs)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)


add_subdirectory(
	log_system/
)

add_subdirectory(
	resman/
)


set(INCLUDE_FILE
	inc/termlib.h
	log_system/inc/log_system.h
)

set(LIBRARY_SOURCE_FILES
	src/cursor.c
	src/screen.c
	src/termlib.c
	src/termlib_server.c
)


add_library(termlib STATIC
	${INCLUDE_FILE}
	${LIBRARY_SOURCE_FILES}
)

set (ALL_LIBS 
     resman
	 log_system
	 pthread
	 m
)

target_link_libraries(termlib
	${ALL_LIBS}
)	

set_target_properties(termlib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(termlib PROPERTIES PUBLIC_HEADER inc/termlib.h)

target_include_directories(termlib PRIVATE inc)
target_include_directories(termlib PRIVATE src)

install(TARGETS termlib
	  	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#[[ Building Client]]

add_executable(termlib_client
	src/termlib_client.c
	${INCLUDE_FILE}
)


#[[ Building Tests ]]

	#[[ Compiling Main ]]


add_executable(clock_exe
	examples/clock.c
	${INCLUDE_FILE}
)

add_executable(main_exe
	examples/main.c
	${INCLUDE_FILE}
)


target_include_directories(clock_exe PRIVATE inc)
target_include_directories(main_exe PRIVATE inc)


target_link_libraries(clock_exe
	termlib
	m
	pthread
)	

target_link_libraries(main_exe
	termlib
	m
	pthread
)	

