cmake_minimum_required(VERSION 3.23)

project(MyGameLib
    LANGUAGES CXX
)

add_library(MyGameLib STATIC
    Modules/Core/Src/BoneTransform.cpp
    Modules/Core/Src/GameMath.cpp
    Modules/Core/Src/Random.cpp
)

target_include_directories(MyGameLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Modules/Core/Include
)

target_compile_features(MyGameLib PUBLIC cxx_std_17)