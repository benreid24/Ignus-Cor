add_library(Shared STATIC
    DebugOverlays.hpp
    Properties.hpp
    Properties.cpp
)

add_subdirectory(Combat)
add_subdirectory(Entities)
add_subdirectory(Items)
add_subdirectory(Maps)
add_subdirectory(Media)
add_subdirectory(Particles)
add_subdirectory(Scripts)
add_subdirectory(Util)
