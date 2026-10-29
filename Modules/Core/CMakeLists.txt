target_sources(MyGameLib PRIVATE
    Src/BoneTransform.cpp
    Src/GameMath.cpp
    Src/Random.cpp
)

target_include_directories(MyGameLib PUBLIC
    Include
)