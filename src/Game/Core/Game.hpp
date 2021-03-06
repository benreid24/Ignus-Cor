#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include <SFML/Window.hpp>
#include "Shared/Media/Playlist.hpp"
#include "Shared/Media/SoundEngine.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Maps/MapManager.hpp"
#include "Game/Core/EventDispatcher.hpp"
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
    std::stack<BaseState*> states;
    friend class BaseState;

    /**
	 * Initializes core game data structures
	 */
	Game();

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

public:
	/**
	 * Frees all memory
	 */
	 ~Game();

	 /**
	  * Returns a reference to the instance of the Game
	  */
     static Game& get();

	 /**
	  * Runs the game
	  */
	 void run();

	 /**
	  * Signals to the currently running state to run a new state
	  */
    void runNewState(BaseState::Ptr state);

	 sf::RenderWindow window;
	 EventDispatcher eventDispatcher;
	 bool inFocus;

	 Entity::Ptr player;
};

#endif // GAME_HPP
