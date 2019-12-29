#include "Shared/Scripts/Specialized/MapScript.hpp"
using namespace std;
using namespace sf;

namespace {

Value getEventMapName(const vector<Value>& args, Script*, const Script::ContextData& context) {
    auto i = context.find("map");
    if (i!=context.end())
        return i->second;
    return Value("ERROR: Map name not in MapScript context");
}

}

MapScript::MapScript(const Script& scr, string mapName, Vector2i pos, Vector2i size, Entity::Ptr entity, int runNumber) : Script(scr) {
    contextData["map"] = Value(mapName);
    contextData["posx"] = Value(pos.x);
    contextData["posy"] = Value(pos.y);
    contextData["sizex"] = Value(size.y);
    contextData["sizey"] = Value(size.y);
    //TODO - add info from entity
    contextData["runNumber"] = Value(runNumber);

    libraryFunctions["getEventMapName"] = &getEventMapName;
    //TODO - add custom library functions
}
