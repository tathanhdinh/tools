# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

# project(PinICap)
# cmake_minimum_required(VERSION 2.8)

# #set(CMAKE_BUILD_TYPE "Debug")

# include(FindPin)
# include(UsePin)

# list(APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

# #include_directories("/home/bigraph/.local/include")
# if(BOOST_INCLUDE_DIR)
#   include_directories(AFTER ${BOOST_INCLUDE_DIR})
# endif(BOOST_INCLUDE_DIR)

# ADD_PINTOOL(vtrace
#   src/main.cpp
#   src/lib/cap/tracer.cpp
#   src/lib/cap/parser.cpp
# #  src/lib/cap/saver.cpp
#   src/lib/cap/graph.cpp
# #  src/lib/cap/trace_graph.cpp
#   src/lib/type/trace.pb.cc
#   src/lib/type/instruction.cpp)

#ADD_PINTOOL(print_value
#  src/print_value.cpp
#  src/lib/cap/tracer.cpp
#  src/lib/cap/parser.cpp
#  src/lib/type/trace.pb.cc
#  src/lib/type/instruction.cpp)

cmake_minimum_required(VERSION 2.8)
project(VTrace)

find_path(PIN_ROOT_DIR
  NAMES source/include/pin/pin.H
  PATHS $ENV{PIN_ROOT_DIR}
  DOC "Pin's base directory"
)

if(NOT PIN_ROOT_DIR)
  message(FATAL_ERROR
    "\nPin not found!\n"
    "Please set the environment variable PIN_ROOT_DIR to the base directory"
    " of the pin library.\n"
  )
endif(NOT PIN_ROOT_DIR)
message(STATUS "PIN_ROOT_DIR: ${PIN_ROOT_DIR}")

set(PIN_CPU_ARCH "ia32")
set(PIN_CPU_ARCH_LONG "ia32")

set(PIN_INCLUDE_DIRS
  ${PIN_ROOT_DIR}/extras/xed2-${PIN_CPU_ARCH_LONG}/include
  ${PIN_ROOT_DIR}/source/include/pin
  ${PIN_ROOT_DIR}/source/include/pin/gen
  ${PIN_ROOT_DIR}/extras/components/include
  ${LOCAL_INCLUDE_DIR})

set(PIN_LIBRARY_DIRS
  ${PIN_ROOT_DIR}/extras/xed2-${PIN_CPU_ARCH_LONG}/lib
  ${PIN_ROOT_DIR}/${PIN_CPU_ARCH_LONG}/lib
  ${PIN_ROOT_DIR}/${PIN_CPU_ARCH_LONG}/lib-ext)

set(PIN_DEFINITIONS "")
list(APPEND PIN_DEFINITIONS
  TARGET_LINUX
  BIGARRAY_MULTIPLIER=1
  USING_XED
  TARGET_IA32 HOST_IA32)

add_custom_command(
  OUTPUT  trace.pb.cc
  COMMAND protoc --cpp_out ../src/lib/type/ -I ../../../proto/ ../../../proto/trace.proto
  COMMENT "Launch protoc")

 add_custom_target(run ALL
   DEPENDS trace.pb.cc)

set(PIN_VERSION_SCRIPT ${PIN_ROOT_DIR}/source/include/pin/pintool.ver)

set(PIN_COMPILE_FLAGS "-Wall -Wno-unknown-pragmas -std=c++11 -m32 -MMD -O3 -fomit-frame-pointer -fno-strict-aliasing -fno-stack-protector")

set(PIN_LINKER_FLAGS "-Wl,--hash-style=sysv -Wl,-Bsymbolic -Wl,--version-script=${PIN_VERSION_SCRIPT}")

set(PIN_LINK_LIBS pin xed dwarf elf dl protobuf)

link_directories(${PIN_LIBRARY_DIRS})
include_directories(${PIN_INCLUDE_DIRS})

# Pintool sources
set(PINTOOL_SRCS
  src/main.cpp
  src/lib/cap/tracer.cpp
  src/lib/cap/parser.cpp
#  src/lib/cap/graph.cpp
  src/lib/type/instruction.cpp
  src/lib/type/trace.pb.cc)

# add Pintool
add_library(vtrace SHARED ${PINTOOL_SRCS})

# set compile and link flags
set_source_files_properties(${PINTOOL_SRCS} PROPERTIES
  COMPILE_FLAGS "${PIN_COMPILE_FLAGS}"
  COMPILE_DEFINITIONS "${PIN_DEFINITIONS}")

set_target_properties(vtrace PROPERTIES
  PREFIX ""
  SUFFIX ".pin"
  COMPILE_DEFINITIONS "${PIN_DEFINITIONS}"
  COMPILE_FLAGS "${PIN_COMPILE_FLAGS}"
  LINK_FLAGS "${PIN_LINKER_FLAGS} -v -m32")

target_link_libraries(vtrace ${PIN_LINK_LIBS})
