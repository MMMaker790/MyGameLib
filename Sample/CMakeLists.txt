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

# Jsonサンプル用データディレクトリをexeと同じディレクトリにコピー
add_custom_command(TARGET JsonSample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/JsonSample"
        "$<TARGET_FILE_DIR:JsonSample>/JsonSample"
)

add_executable(RenderingSample
    RenderingSample.cpp
)
target_link_libraries(RenderingSample
    PRIVATE MyGameLib
)
# Renderingサンプル用データディレクトリをexeと同じディレクトリにコピー
add_custom_command(TARGET RenderingSample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/RenderingSample"
        "$<TARGET_FILE_DIR:RenderingSample>/RenderingSample"
)