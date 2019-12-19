add_executable(IgnisCorEditor
    AnimationEditor.hpp
    AnimationEditor.cpp
    ConversationEditor.hpp
    ConversationEditor.cpp
    ItemDbEditor.hpp
    ItemDbEditor.cpp
    main.cpp
    MapEditor.hpp
    MapEditor.cpp
    MapEditorDialogs.cpp
    NpcEditor.hpp
    ScriptEditor.hpp
    ScriptEditor.cpp
)

add_subdirectory(GUI)
add_subdirectory(Helpers)

add_definitions(-DEDITOR)

set(SFGUI_DIR ${PROJECT_SOURCE_DIR}/lib/SFGUI)
target_include_directories(IgnisCorEditor PUBLIC ${SFGUI_DIR}/include)

target_link_libraries(IgnisCorEditor
    Shared
    ${SFGUI_DIR}/lib/libsfgui-s.a
    sfml-graphics
    sfml-window
    sfml-network
    sfml-audio
    sfml-system
    sfml-main
)

install(TARGETS IgnisCorEditor DESTINATION ${PROJECT_SOURCE_DIR})
if (WIN32)
    install(FILES ${SFGUI_DIR}/bin/sfgui.dll DESTINATION ${PROJECT_SOURCE_DIR})
endif()