project(CFGRecon)
cmake_minimum_required(VERSION 2.8)

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

set(PIN_LIB_DIRS
  ${PIN_ROOT_DIR}/extras/xed2-${PIN_CPU_ARCH_LONG}/lib
  ${PIN_ROOT_DIR}/${PIN_CPU_ARCH_LONG}/lib
  ${PIN_ROOT_DIR}/${PIN_CPU_ARCH_LONG}/lib-ext)

if(NOT PROTOBUF_ROOT_DIR)
  message(FATAL_ERROR
    "\nProtobuf library not found!\n"
    "Please set the environment variable PROTOBUF_ROOT_DIR\n"
  )
endif(NOT PROTOBUF_ROOT_DIR)
message(STATUS "PROTOBUF_ROOT_DIR: ${PROTOBUF_ROOT_DIR}")

set(PROTOBUF_LIB_DIRS ${PROTOBUF_ROOT_DIR}/lib)
set(PROTOBUF_INCLUDE_DIRS ${PROTOBUF_ROOT_DIR}/include)

if(NOT DEFINED BOOST_ROOT_DIR)
  message(FATAL_ERROR "\nBoost not found!\n"
    "Please set the environment variable BOOST_ROOT_DIR to the base directory"
    " of the Boost library.\n")
endif(NOT DEFINED BOOST_ROOT_DIR)
message(STATUS "BOOST_ROOT_DIR: ${BOOST_ROOT_DIR}")

set(PIN_LIBS pin pinvm libxed)
set(PROTOBUF_LIBS libprotobuf)

set(PIN_DEFS "/D TARGET_IA32 /D HOST_IA32 /D TARGET_WINDOWS /D USING_XED /D_SECURE_SCL=0 /D_ITERATOR_DEBUG_LEVEL=0 /Zc:auto")
set(PROTOBUF_DEFS "/D PROTOBUF_USE_DLLS")

include_directories(${BOOST_ROOT_DIR} ${PIN_INCLUDE_DIRS} ${PROTOBUF_INCLUDE_DIRS})
link_directories(${PIN_LIB_DIRS} ${PROTOBUF_LIB_DIRS})

add_definitions(${PIN_DEFS} ${PROTOBUF_DEFS})

set(SRC_LIST
  src/type/trace.pb.cc
  src/type/instruction.cpp
  src/type/operand.cpp
  src/graph/graph.cpp
  src/loader/loader.cpp
  src/main.cpp
)

#set(SRC_LIST ${LOADER_FILES} ${GRAPH_FILES})
#aux_source_directory(src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${PIN_LIBS} ${PROTOBUF_LIBS})

