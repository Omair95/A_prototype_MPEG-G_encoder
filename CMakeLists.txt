cmake_minimum_required(VERSION 3.9)
project(A_prototype_MPEG_G_encoder)

include_directories("/home/omair/TFG/A_prototype_MPEG-G_encoder/seqan/include")
include_directories("/home/omair/TFG/A_prototype_MPEG-G_encoder/boost_1_66_0")

set(CMAKE_CXX_STANDARD 14)

add_executable(A_prototype_MPEG_G_encoder main.cpp Utils.h AccessUnit.h AccessUnitBlock.h AccessUnit_P.h MpeggRecord.h AccessUnit_N.h AccessUnit_M.h AccessUnit_I.h AccessUnit_HM.h AccessUnit_U.h)
