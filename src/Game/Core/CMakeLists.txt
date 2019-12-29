add_subdirectory(States)

target_sources(IgnisCor PUBLIC
    BaseState.hpp
    BaseState.cpp
    EventDispatcher.hpp
    EventDispatcher.cpp
    EventObserver.hpp
    Game.hpp
    Game.cpp
    PlayerInput.hpp
    PlayerInput.cpp
)