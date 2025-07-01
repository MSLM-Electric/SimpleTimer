cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(PROJ_NAME SimpleTimer)
project(${PROJ_NAME} C)
set(VERSION_GENERATOR_ENABLE 1)
if(${VERSION_GENERATOR_ENABLE} EQUAL 1)
#=======================================================================================
    message(STATUS "Version generator enabled!")
    include_directories(${CMAKE_BINARY_DIR}/../version_generator)
    set(VERSION_GENERATOR_PATH ${CMAKE_SOURCE_DIR}/version_generator)
    add_executable(${PROJ_NAME}_versiontestexecutable ${VERSION_GENERATOR_PATH}/runversiontest.c ${VERSION_GENERATOR_PATH}/funcGetVersion.c ${VERSION_GENERATOR_PATH}/funcGetVersion.h)
    find_package(Git)
    add_custom_target(version
      ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/version_generator/generateheader_version.h.in
                       -D DST=${CMAKE_SOURCE_DIR}/${PROJ_NAME}_version.h
                       -D PROJECT_NAME=${PROJ_NAME}
                       -D SHOWFUNC_SRC=${VERSION_GENERATOR_PATH}/generateruntest_file.c.in
                       -D SHOWFUNC_DST=${VERSION_GENERATOR_PATH}/funcGetVersion.c
                       -D GIT_EXECUTABLE=${GIT_EXECUTABLE}                   
                       -P ${CMAKE_SOURCE_DIR}/version_generator/GenerateVersionHeader.cmake
      )
    add_dependencies(${PROJ_NAME}_versiontestexecutable version)
#=======================================================================================
endif()

set(MAINSRC ${CMAKE_SOURCE_DIR}/SimpleTimerWP.c ${CMAKE_SOURCE_DIR}/mainDemo_stopwatch.c)
set(MAINHDR ${CMAKE_SOURCE_DIR}/SimpleTimerWP.h)
add_executable(${PROJ_NAME} ${MAINSRC} ${MAINHDR})
