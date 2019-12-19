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

set(INSTALL_DIR ${PROJECT_SOURCE_DIR}/install)
install(DIRECTORY DESTINATION ${INSTALL_DIR})

install(TARGETS IgnisCor DESTINATION ${INSTALL_DIR})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/Resources DESTINATION ${INSTALL_DIR})

if(WIN32)
    install(FILES ${PROJECT_SOURCE_DIR}/openal32.dll DESTINATION ${INSTALL_DIR})
endif()