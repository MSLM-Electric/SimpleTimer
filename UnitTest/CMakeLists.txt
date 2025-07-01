cmake_minimum_required(VERSION 3.0.2)
set(PROJ_NAME simpletimer_unittest)
project(${PROJ_NAME})
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

message("Start building project...")

set(BACK_FROM_ROOT "../")
set(EXTERNAL_LIBS "${BACK_FROM_ROOT}")


set(SOURCES
	${ROOT_PATH}/ConsoleMenuOptions.c
	${EXTERNAL_LIBS}/SimpleTimerWP.c
	${ROOT_PATH}/MultiThreadSupport.c
	#${ROOT_PATH}/CmdsScenarioScripts.c
	${ROOT_PATH}/MultiThread_Template_main.c
)

set(HEADERS
	#${ROOT_PATH}/debug_print.h
	
	${ROOT_PATH}/ConsoleMenuOptions.h
	${ROOT_PATH}/type_def.h
	${EXTERNAL_LIBS}/SimpleTimerWP.h
	${ROOT_PATH}/MultiThreadSupport.h
	#${ROOT_PATH}/CmdsScenarioScripts.h
	#${EXTERNAL_LIBS}/ComplexTest/vssimulation_port.h

)

add_definitions(-DDEBUG_ON_VS)
add_definitions(-DTIMER_TICK_16BIT_EN=1)
add_definitions(-DTIMER_TICK_32BIT_EN=0)

add_executable(${PROJ_NAME} ${SOURCES} ${HEADERS})


if(${CMAKE_VERSION} VERSION_LESS "2.8.12")
    include_directories(${PROJ_NAME} PUBLIC ${INCLUDE_DIR})
else()
    target_include_directories(${PROJ_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)
endif()

target_link_options(${PROJ_NAME} PRIVATE "/MAP:${PROJECT_SOURCE_DIR}/outputProject.map")