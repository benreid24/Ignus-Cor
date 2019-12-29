add_subdirectory(Specialized)
add_subdirectory(Util)

target_sources(Shared PUBLIC
    Script.hpp
    Script.cpp
    ScriptManager.hpp
    ScriptManager.cpp
)