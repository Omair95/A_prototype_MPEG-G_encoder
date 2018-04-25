cmake_minimum_required(VERSION 3.1)

project(A_prototype_MPEG_G_encoder)

# Add executables
add_executable(A_prototype_MPEG_G_encoder
        main.cpp
        Utils.h
        AccessUnit.h AccessUnit.cpp
        AccessUnitBlock.h AccessUnitBlock.cpp
        AccessUnit_P.h AccessUnit_P.cpp
        MpeggRecord.h
        AccessUnit_N.h AccessUnit_N.cpp
        AccessUnit_M.h AccessUnit_M.cpp
        AccessUnit_I.h AccessUnit_I.cpp
        AccessUnit_HM.h AccessUnit_HM.cpp
        AccessUnit_U.h AccessUnit_U.cpp
        FileManager.h FileManager.cpp Utils.cpp)

# require C++14
target_compile_features(A_prototype_MPEG_G_encoder PRIVATE cxx_decltype_auto)

# You don't have cmake 3.1? Comment out the above. And comment in the below.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(CXX11_FOUND TRUE)

# Set paths for FindSeqAn.cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/seqan/util/cmake")
message("CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")

set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/seqan/include")
message("CMAKE_INCLUDE_PATH = ${CMAKE_INCLUDE_PATH}")

#use ZLIB module
find_package(ZLIB)

# Use SeqAn module
find_package (SeqAn REQUIRED)

# Was SeqAn included?
if (NOT (SEQAN_FOUND))
    message (FATAL_ERROR "SeqAn not found")
endif ()

# Add include directories set by find_package (SeqAn).
set(SEQAN_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/seqan/include")
include_directories (${SEQAN_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}//boost_1_66_0)

message("SEQAN_INCLUDE_DIRS = ${SEQAN_INCLUDE_DIRS}")

# Add definitions set by find_package (SeqAn).
add_definitions (${SEQAN_DEFINITIONS})
message("SEQAN_DEFINITIONS = ${SEQAN_DEFINITIONS}")

# Add dependencies found by find_package (SeqAn).
target_link_libraries (A_prototype_MPEG_G_encoder ${SEQAN_LIBRARIES})
message("SEQAN_LIBRARIES = ${SEQAN_LIBRARIES}")

# Add CXX flags found by find_package (SeqAn).
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SEQAN_CXX_FLAGS} -pthread")
