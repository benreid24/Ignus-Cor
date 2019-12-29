add_subdirectory(Behaviors)
add_subdirectory(Instances)
add_subdirectory(Virtual)

target_sources(Shared PUBLIC
    Entity.hpp
    Entity.cpp
    EntityBehavior.hpp
    EntityBehavior.cpp
    EntityBubble.hpp
    EntityBubble.cpp
    EntityConversation.hpp
    EntityConversation.cpp
    EntityConversationNodeFactory.hpp
    EntityConversationNodeFactory.cpp
    EntityInventory.hpp
    EntityInventory.cpp
    EntityManager.hpp
    EntityManager.cpp
    EntityVisual.hpp
    EntityVisual.cpp
)
