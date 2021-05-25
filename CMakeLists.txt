cmake_minimum_required(VERSION 3.2)
message(STATUS "Cmake version ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")

project(HyperFlowWrench)

set(CMAKE_CXX_STANDARD 14)

add_definitions("-Wall -Wno-unused-variable -Wno-unused-private-field")

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/conf/cmake/")
find_package(SimGrid REQUIRED)

# build the version number
set(WRENCH_HYPERFLOW_VERSION_MAJOR "1")
set(WRENCH_HYPERFLOW_VERSION_MINOR "0")
set(WRENCH_HYPERFLOW_VERSION_PATCH "0")
set(WRENCH_HYPERFLOW_VERSION_EXTRA "")

if (${WRENCH_HYPERFLOW_VERSION_PATCH} EQUAL "0")
    set(WRENCH_HYPERFLOW_RELEASE_VERSION "${WRENCH_HYPERFLOW_VERSION_MAJOR}.${WRENCH_HYPERFLOW_VERSION_MINOR}")
else ()
    set(WRENCH_HYPERFLOW_RELEASE_VERSION "$(WRENCH_HYPERFLOW_VERSION_MAJOR}.${WRENCH_HYPERFLOW_VERSION_MINOR}.${WRENCH_HYPERFLOW_VERSION_PATCH}")
endif ()

if (NOT ${WRENCH_HYPERFLOW_VERSION_EXTRA} EQUAL "")
    set(WRENCH_HYPERFLOW_RELEASE_VERSION "${WRENCH_HYPERFLOW_RELEASE_VERSION}-${WRENCH_HYPERFLOW_VERSION_EXTRA}")
endif ()

include_directories(src/ include/ /usr/local/include /usr/local/include/wrench /usr/local/include/wrench/tools/pegasus)

# source files
set(SOURCE_FILES
        src/hyperflow/scheduler/HyperFlowJobScheduler.cpp
        src/hyperflow/wms/HyperFlowWMS.cpp
        include/hyperflow/wms/HyperFlowWMS.h
        include/hyperflow/scheduler/HyperFlowJobScheduler.h
        src/hyperflow/lambda/LambdaComputeService.cpp
        src/hyperflow/lambda/LambdaComputeServiceMessage.cpp
        src/hyperflow/lambda/LambdaComputeServiceMessagePayload.cpp
        src/hyperflow/lambda/LambdaComputeServiceProperty.cpp
        include/hyperflow/lambda/LambdaComputeService.h
        include/hyperflow/lambda/LambdaComputeServiceMessage.h
        include/hyperflow/lambda/LambdaComputeServiceMessagePayload.h
        include/hyperflow/lambda/LambdaComputeServiceProperty.h
        src/hyperflow/StandardJobExecutorMessage.cpp include/hyperflow/StandardJobExecutorMessage.h)

set(TEST_FILES
        )

# wrench libraries
find_library(WRENCH_LIBRARY NAMES wrench)
find_library(WRENCH_PEGASUS_LIBRARY NAMES wrenchpegasusworkflowparser)
find_library(SIMGRID_LIBRARY NAMES simgrid)
find_library(PUGIXML_LIBRARY NAMES pugixml)
find_library(GTEST_LIBRARY NAMES gtest)

add_library(hyperflow STATIC ${SOURCE_FILES})
set_target_properties(hyperflow PROPERTIES VERSION ${WRENCH_HYPERFLOW_RELEASE_VERSION})
target_link_libraries(hyperflow ${WRENCH_LIBRARY} ${WRENCH_PEGASUS_LIBRARY} ${SimGrid_LIBRARY} ${PUGIXML_LIBRARY})

# hyperflow wrench version
add_custom_command(TARGET hyperflow PRE_LINK COMMAND ${CMAKE_COMMAND} -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR} -DWRENCH_RELEASE_VERSION=${WRENCH_HYPERFLOW_RELEASE_VERSION})

install(TARGETS hyperflow DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

# build examples
include(${CMAKE_HOME_DIRECTORY}/conf/cmake/Examples.cmake)


