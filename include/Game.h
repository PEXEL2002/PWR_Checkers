#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "MinMaxAlgorithm.h"
/**
 * @brief Class representing the game
    @param window Window of the game
    @param board Board of the game
    @param ai AI of the game
    @param playerTurn Player turn
    @param ifPlayWithAI If play with AI
 */
class Game {
public:
/**
 * @brief Construct a new Game object
 */
    Game();
/**
 * @brief Destroy the Game object
 */
    ~Game();
/**
 * @brief Run the game
 */
    void run();

private:
/**
 * @brief Process events
 */
    void processEvents();
/**
 * @brief Update the game
 */
    void update();
/**
 * @brief Render the game
 */
    void render();

    sf::RenderWindow window;
    Board board;
    AI ai;
    int playerTurn;
    int ifPlayWithAI;
};
