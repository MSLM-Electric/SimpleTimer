cmake_minimum_required(VERSION 3.0.2)
set(PROJ_NAME simpletimer_unittest)
project(${PROJ_NAME})
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(ADDIT_LIBS "${ROOT_PATH}/../")

message("Start building project...")

set(BACK_FROM_ROOT "../../")
set(EXTERNAL_LIBS "${BACK_FROM_ROOT}")


set(SOURCES
	${ADDIT_LIBS}/ConsoleMenuOptions.c
	${EXTERNAL_LIBS}/SimpleTimerWP.c
	${ADDIT_LIBS}/MultiThreadSupport.c
	${ADDIT_LIBS}/EqualCheck.c
	${ROOT_PATH}/MultiThread_Template_main.c
)

set(HEADERS
	#${ROOT_PATH}/debug_print.h
	
	${ADDIT_LIBS}/ConsoleMenuOptions.h
	${ADDIT_LIBS}/type_def.h
	${EXTERNAL_LIBS}/SimpleTimerWP.h
	${ADDIT_LIBS}/MultiThreadSupport.h
	${ADDIT_LIBS}/EqualCheck.h
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