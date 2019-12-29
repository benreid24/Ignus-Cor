#ifndef AISCRIPT_HPP
#define AISCRIPT_HPP

#include "Shared/Scripts/Script.hpp"
#include "Shared/Entities/Entity.hpp"

class AiScript : public Script {
    Entity::Ptr entity;

public:
    AiScript(Entity::Ptr ai);

    //TODO - interaction methods? Or communicate with library functions through ContextData?
};

#endif // AISCRIPT_HPP
