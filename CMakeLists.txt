cmake_minimum_required(VERSION 3.23)

project(MyGameLib
    LANGUAGES CXX
)

add_library(MyGameLib STATIC)

target_compile_features(MyGameLib PUBLIC cxx_std_17)

add_subdirectory(Modules/Core)

target_include_directories(MyGameLib PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Modules/Core/Include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

include(GNUInstallDirs)

install(TARGETS MyGameLib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY Modules/Core/Include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Core
)