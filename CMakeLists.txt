cmake_minimum_required(VERSION 3.23)

project(MyGameLib
    LANGUAGES CXX
)

add_library(MyGameLib STATIC)

target_compile_features(MyGameLib PUBLIC cxx_std_17)

add_subdirectory(Modules/Core)