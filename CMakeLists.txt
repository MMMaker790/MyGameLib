cmake_minimum_required(VERSION 3.23)

project(MyGameLib
    LANGUAGES CXX
)

add_library(MyGameLib STATIC)

target_compile_features(MyGameLib PUBLIC cxx_std_17)

# Core
add_subdirectory(Modules/Core)
target_include_directories(MyGameLib PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Modules/Core/Include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Rendering
# add_subdirectory(Modules/Rendering)
# target_include_directories(MyGameLib PUBLIC
#     "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Modules/Rendering/Include>"
#     "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
# )

# Json
add_subdirectory(Modules/Json)
target_include_directories(MyGameLib PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Modules/Json/Include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# 外部ライブラリ
target_include_directories(MyGameLib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/External/include"
)

# SDL2
set_target_properties(MyGameLib PROPERTIES
    IMPORTED_IMPLIB
        "${CMAKE_CURRENT_SOURCE_DIR}/External/lib/SDL2/SDL2.lib"
    IMPORTED_LOCATION
        "${CMAKE_CURRENT_SOURCE_DIR}/External/lib/SDL2/SDL2.dll"
)

# glew
target_link_libraries(MyGameLib PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/External/lib/GL/x64/glew32s.lib"
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

# install(DIRECTORY Modules/Rendering/Include/
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Rendering
# )

install(DIRECTORY Modules/Json/Include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Json
)

option(MYGAMELIB_BUILD_SAMPLES "Build MyGameLib samples" OFF)
if(MYGAMELIB_BUILD_SAMPLES)
    add_subdirectory(Sample)
endif()

