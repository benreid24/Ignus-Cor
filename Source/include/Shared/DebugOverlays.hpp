#ifndef DEBUGOVERLAYS_HPP
#define DEBUGOVERLAYS_HPP

#include <SFML/System.hpp>
#include <iostream>
#include <string>

/**
 * Enum representing the different debug overlays
 *
 * \ingroup Global
 */
class DebugOverlays {
public:
    enum Type {
        None = 0,
        EntityInfo = 1 << 0,
        BoundingBoxes = 1 << 2,
        CombatData = 1 << 3
    };

private:
    static Type& p_get() { static Type debug = None; return debug; }
    static int& p_lastUpdate() { static int ltime = 0; return ltime; }
    static sf::Clock& p_timer() { static sf::Clock clock; return clock; }

public:
    static inline bool isOverlayActive(Type overlay) {
        return (p_get() & (int)overlay) != 0;
    }

    static inline std::string toString(Type tp) {
        std::string ret;

        if (isOverlayActive(EntityInfo))
            ret = "EntityInfo";
        if (isOverlayActive(BoundingBoxes))
            ret += std::string((ret.size()>0)?(" | "):("")) + "BoundingBoxes";
        if (isOverlayActive(CombatData))
            ret += std::string((ret.size()>0)?(" | "):("")) + "CombatData";

        if (ret.size() == 0)
            ret = "None";

        return ret;
    }

    static inline void toggleOverlay(Type overlay) {
        if (p_timer().getElapsedTime().asMilliseconds()-p_lastUpdate() >= 500) {
            p_get() = Type((int)p_get() ^ (int)overlay);
            p_lastUpdate() = p_timer().getElapsedTime().asMilliseconds();
            std::cout << "Active Debug Overlays: " << toString(p_get()) << std::endl;
        }
    }
};

#endif // DEBUGOVERLAYS_HPP
