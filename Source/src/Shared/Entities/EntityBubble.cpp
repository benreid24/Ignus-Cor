#include "Shared/Entities/EntityBubble.hpp"
using namespace sf;
using namespace std;

EntityBubble::EntityBubble(Mode m) {
	mode = m;
	spr.setTexture(txtr.getTexture());
	spr.setScale(1,0.7);
	//TODO - make EntityBubble
}

void EntityBubble::render(sf::RenderTarget& target, sf::Vector2f position) {
	//
}
