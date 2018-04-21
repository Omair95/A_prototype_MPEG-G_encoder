cmake_minimum_required(VERSION 3.9)
project(A_prototype_MPEG_G_encoder)

include_directories("${PROJECT_SOURCE_DIR}/seqan/include")

set(CMAKE_CXX_STANDARD 14)

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
