add_executable(CoreSample
    CoreSample.cpp
)

target_link_libraries(CoreSample
    PRIVATE MyGameLib
)

add_executable(JsonSample
    JsonSample.cpp
)

target_link_libraries(JsonSample
    PRIVATE MyGameLib
)