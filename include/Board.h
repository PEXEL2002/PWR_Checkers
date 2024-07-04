#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Move.h"
/**
 * @brief class representing the board
 * @param board 2D vector representing the board
 * @param colorPlayer1 Color of player 1
 * @param selectedPiece Selected piece
 * @param isMultiJump If multi jump is available
 * @param isGameOver If game is over
 * @param winner Winner of the game
 */
class Board {
public:
/**
 * @brief Construct a new Board object
 */
    Board();
/**
 * @brief Destroy the Board object
 */
    ~Board();
/**
 * @brief Draw the board
 * @param window
 * @param windowSize
 */
    void drawBoard(sf::RenderWindow& window, const sf::Vector2u& windowSize);
/**
 * @brief Draw the pieces
 * @param window
 */
    void drawPieces(sf::RenderWindow& window);
/**
 * @brief Select a piece
 * @param position
 * @param playerTurn
 */
    bool selectPiece(const sf::Vector2i& position, int playerTurn);
/**
 * @brief Move a piece
 * @param move
 * @param playerTurn
 */
    bool movePiece(const Move& move, int playerTurn);
/**
 * @brief Check if move is valid
 * @param move
 * @param playerTurn
 */
    bool isValidMove(const Move& move, int playerTurn) const;
/**
 * @brief Check if jump is valid
 * @param move
 * @param playerTurn
 */
    bool isValidJump(const Move& move, int playerTurn) const;
/**
 * @brief Check if move is valid for queen
 * @param move
 * @param playerTurn
 */
    bool isValidQueenMove(const Move& move, int playerTurn) const;
/**
 * @brief Check if jump is valid for queen
 * @param move
 * @param playerTurn
 */
    bool isValidQueenJump(const Move& move, int playerTurn) const;
/**
 * @brief Check if piece is opponent piece
 * @param piece
 * @param playerTurn
 */
    static bool isOpponentPiece(int piece, int playerTurn) ;
/**
 * @brief Get piece at position
 * @param position
 */
    int getPieceAt(const sf::Vector2i& position) const;
/**
 * @brief Get selected piece
 */
    sf::Vector2i getSelectedPiece() const;
/**
 * @brief Deselect piece
 */
    void deselectPiece();
/**
 * @brief Check if jumps are available for piece
 * @param position
 * @param playerTurn
 */
    bool areJumpsAvailableForPiece(const sf::Vector2i& position, int playerTurn) const;
/**
 * @brief Get if multi jump is available
 */
    bool getIsMultiJump() const;
/**
 * @brief Update multiple jump
 * @param playerTurn
 */
    void updateMultipleJump(int playerTurn);
/**
 * @brief Check if piece can jump
 * @param position
 * @param playerTurn
 */
    bool canPieceJump(const sf::Vector2i& position, int playerTurn) const;
/**
 * @brief Remove piece
 * @param position
 */
    void removePiece(const sf::Vector2i& position);
/**
 * @brief Promote piece to queen
 * @param x
 * @param y
 */
    void promoteToQueen(int x, int y);
/**
 * @brief Check if game is over
 */
    bool checkGameOver();
/**
 * @brief Check if player has valid moves
 * @param playerTurn
 */
    bool hasValidMoves(int playerTurn) const;
/**
 * @brief Get winner of the game
 */
    int getWinner() const;
/**
 * @brief Get all possible moves
 * @param playerTurn
 * @return std::vector<Move>
 */
    std::vector<Move> getAllPossibleMoves(int playerTurn) const;
/**
 * @brief Check if jump is available
 * @param playerTurn
 */
    bool isJumpAvailable(int playerTurn) const;

private:
    std::vector<std::vector<int>> board;
    int colorPlayer1;
    sf::Vector2i selectedPiece;
    bool isMultiJump;
    bool isGameOver;
    int winner;

    sf::Texture whitePawnTexture; // Textures of white pawns
    sf::Texture whiteQueenTexture; // Textures of white queens
    sf::Texture blackPawnTexture; // Textures of black pawns
    sf::Texture blackQueenTexture; // Textures of black queens
};