#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include <SFML/Window.hpp>
#include "Shared/Media/Playlist.hpp"
#include "Shared/Media/SoundEngine.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Maps/MapManager.hpp"
#include "BaseState.hpp"

/**
 * \defgroup Core The core workings of the game. Includes the main Game class as well as all the states and shared data
 */

 /**
  * Acts as the owner of all the game data and is responsible for running the game
  *
  * \ingroup Core
  */
class Game {
    /**
	 * Initializes core game data structures
	 */
	Game();

public:
	/**
	 * Frees all memory
	 */
	 ~Game();

	 /**
	  * Returns a pointer to the instance of the Game
	  */
     static Game* get();

	 /**
	  * Runs the game
	  */
	 void run();

	 sf::RenderWindow window;
	 bool inFocus;

	 std::stack<BaseState*> states;
	 Entity::Ptr player;

	 MapManager mapManager;
	 EntityManager entityManager;

	 SoundEngine soundEngine;
	 Playlist playlist;
};

#endif // GAME_HPP
