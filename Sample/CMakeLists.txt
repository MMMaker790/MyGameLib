add_executable(CoreSample
    CoreSample.cpp
)

target_link_libraries(CoreSample
    PRIVATE MyGameLib
)