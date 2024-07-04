#pragma once
#include "Board.h"
#include <limits>

class AI {
public:
/**
 * @brief Construct a new AI object
 * @param depth
 */
    AI(int depth);
/**
 * @brief Find the best move
 * @param board
 * @param playerTurn
 * @return
 */
    Move findBestMove(Board& board, int playerTurn);
private:
    int depth;
/**
 * @brief Minimax algorithm
 * @param board
 * @param depth
 * @param alpha
 * @param beta
 * @param maximizingPlayer
 * @return value of the best move
 */
    int minimax(Board& board, int depth, int alpha, int beta, bool maximizingPlayer);
/**
 * @brief Evaluate the board
 * @param board
 * @return  value of the board
 */
    int evaluateBoard(const Board& board);
/**
 * @brief Check if piece is in danger
 * @param board
 * @param position
 * @param pieceType
 * @return  true if piece is in danger
 */
    static bool isPieceInDanger(const Board& board, const sf::Vector2i& position, int pieceType) ;
/**
 * @brief Check if move will expose piece
 * @param board
 * @param move
 * @param playerTurn
 * @return true if move will expose piece
 */
    bool willMoveExposePiece(const Board &board, const Move &move, int playerTurn);
/**
 * @brief Count pieces of each player
 * @param board
 * @return sum of pieces of each player
 */
    std::pair<int, int> countPieces(const Board &board) const;
/**
 * @brief Check if position is in center
 * @param position
 * @return
 */
    bool isInCenter(const sf::Vector2i& position) const;
/**
 * @brief Check if position is on edge
 * @param position
 * @return
 */
    bool isOnEdge(const sf::Vector2i& position) const;

/**
 * @brief Find the most advanced pawn move
 * @param board
 * @param playerTurn
 */
    Move findMostAdvancedPawnMove(Board &board, int playerTurn);
};
