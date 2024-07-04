#include "Game.h"
#include <iostream>

Game::Game() : ai(4) {
    std::cout << "Enter 0 to play with another player, 1 to play with AI: ";
    std::cin >> ifPlayWithAI;
    playerTurn = 0;
    window.create(sf::VideoMode(800, 800), "Checkers");
    board = Board();
}
Game::~Game() {
    window.close();
}
void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        else {
            if (event.type == sf::Event::MouseButtonPressed) { // Mouse click
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i position = sf::Mouse::getPosition(window);
                    sf::Vector2u windowSize = window.getSize();
                    int tileSize = windowSize.x / 8;
                    sf::Vector2i boardPos(position.x / tileSize, position.y / tileSize);

                    if (board.getSelectedPiece().x == -1) {
                        board.selectPiece(boardPos, playerTurn);
                    } else {
                        sf::Vector2i selected = board.getSelectedPiece();
                        if (board.movePiece(Move(selected.x, selected.y, boardPos.x, boardPos.y), playerTurn)) {
                            if (!board.getIsMultiJump()) {
                                playerTurn = 1 - playerTurn;
                            }
                        }
                        board.deselectPiece();
                    }

                    if (board.checkGameOver()) {
                        int winner = board.getWinner();
                        if (winner == 1) {
                            std::cout << "Game Over! White wins!" << std::endl;
                        } else if (winner == 2) {
                            std::cout << "Game Over! Black wins!" << std::endl;
                        } else {
                            std::cout << "Game Over! It's a draw!" << std::endl;
                        }
                        window.close();
                    } else {
                        std::cout << "Player " << (playerTurn == 0 ? "White" : "Black") << "'s turn." << std::endl;
                    }
                }
            }
        }
    }
    render();
}

void Game::update() {
    if (ifPlayWithAI && playerTurn == 1) {
        Move aiMove = ai.findBestMove(board, playerTurn);
        board.movePiece(aiMove, playerTurn);
        if (!board.getIsMultiJump()) {
            playerTurn = 1 - playerTurn;
        }
        board.deselectPiece();

        if (board.checkGameOver()) {
            int winner = board.getWinner();
            if (winner == 1) {
                std::cout << "Game Over! White wins!" << std::endl;
            } else if (winner == 2) {
                std::cout << "Game Over! Black wins!" << std::endl;
            } else {
                std::cout << "Game Over! It's a draw!" << std::endl;
            }
            window.close();
        } else {
            std::cout << "Player " << (playerTurn == 0 ? "White" : "Black") << "'s turn." << std::endl;
        }
    }
    render();
}

void Game::render() {
    window.clear();
    sf::Vector2u windowSize = window.getSize();
    board.drawBoard(window, windowSize);
    board.drawPieces(window);
    window.display();
}
