add_executable(IgnisCor main.cpp)

add_definitions(-DGAME)

add_subdirectory(Core)
add_subdirectory(Menu)

target_link_libraries(IgnisCor
    Shared
    sfml-graphics
    sfml-window
    sfml-network
    sfml-audio
    sfml-system
    sfml-main
)

install(TARGETS IgnisCor DESTINATION ${PROJECT_SOURCE_DIR})
if (WIN32)
    install(FILES ${CMAKE_BINARY_DIR}/build/bin/openal32.dll DESTINATION ${PROJECT_SOURCE_DIR})
endif()