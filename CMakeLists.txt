#CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(ANAROOT)

set(CMAKE_CXX_FLAGS "-fPIC -Wall -Wextra -O3 -mtune=native -march=native -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64")
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11 -O3 -mtune=native -march=native")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_INCLUDE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/include")

# for ROOT
if(DEFINED ENV{ROOTSYS})
message("ROOTSYS: " $ENV{ROOTSYS})
else()
message(FATAL_ERROR "\$ROOTSYS is not defined. please define the environment for ROOT.")
endif()

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread)
include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
#set(CMAKE_MODULE_PATH "${ROOTSYS}/cmake" ${CMAKE_MODULE_PATH})

add_subdirectory(sources)
#set_target_properties(AMDFormat PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/container)

#add_executable(${PROJECT_NAME} AMDROOTConverter.cc)
#target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/container)
#target_link_libraries(${PROJECT_NAME} ${ROOT_LIBRARIES} AMDFormat)

set(ROOTSYS $ENV{ROOTSYS})
set(PPWWDD ${CMAKE_SOURCE_DIR})
if(enable_minos)
set(MINOSFEMLIB ":${MINOS_FEM_LIBRARY_DIRS}")
endif()
configure_file(${CMAKE_SOURCE_DIR}/scripts/setup.sh.in
	${CMAKE_SOURCE_DIR}/scripts/setup.sh
)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/scripts/
                  DESTINATION scripts)
