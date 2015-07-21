project(cfg-reconstruction)
cmake_minimum_required(VERSION 2.8)

set(SRC_LIST
  src/loader/trace.pb.cc
  src/graph/graph.cpp
  src/main.cpp
)

add_definitions("-std=c++11")

#aux_source_directory(src/loader LOADER_FILES)
#aux_source_directory(/src/graph GRAPH_FILES)
#aux_source_directory(src MAIN_FILES)

#set(SRC_LIST ${LOADER_FILES} ${GRAPH_FILES})
#aux_source_directory(src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

