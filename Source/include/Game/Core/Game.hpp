#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include "Shared/Entities/EntityManager.hpp"
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

public:
	/**
	 * Initializes core game data structures
	 */
	Game();

	/**
	 * Frees all memory
	 */
	 ~Game();

	 /**
	  * Runs the game
	  */
	 void run();

	 std::stack<BaseState*> states;
	 EntityManager entityManager;
};

#endif // GAME_HPP
