#include "MinMaxAlgorithm.h"
#include <limits>

AI::AI(int depth) : depth(depth) {}

Move AI::findBestMove(Board& board, int playerTurn) {
    int bestValue = std::numeric_limits<int>::min();
    Move bestMove;
    bool jumpAvailable = board.isJumpAvailable(playerTurn);
    std::vector<Move> possibleMoves = board.getAllPossibleMoves(playerTurn);
    for (const Move& move : possibleMoves) {
        if (board.isValidJump(move, playerTurn) || board.isValidQueenJump(move, playerTurn)) {
            Board newBoard = board;
            newBoard.movePiece(move, playerTurn);

            if (!willMoveExposePiece(newBoard, move, playerTurn)) {
                int moveValue = minimax(newBoard, depth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);
                if (moveValue > bestValue) {
                    bestValue = moveValue;
                    bestMove = move;
                }
            }
        }
    }
    if (bestMove.fromX == 0 && bestMove.fromY == 0 && bestMove.toX == 0 && bestMove.toY == 0) {
        bestMove = findMostAdvancedPawnMove(board, playerTurn);
    }
    if (bestMove.fromX == 0 && bestMove.fromY == 0 && bestMove.toX == 0 && bestMove.toY == 0) {
        int minPiecesLost = std::numeric_limits<int>::max();
        for (const Move& move : possibleMoves) {
            Board newBoard = board;
            newBoard.movePiece(move, playerTurn);
            auto [player1Pieces, player2Pieces] = countPieces(newBoard);
            int piecesLost = (playerTurn == 0) ? player1Pieces : player2Pieces;
            if (piecesLost < minPiecesLost) {
                minPiecesLost = piecesLost;
                bestMove = move;
            }
        }
    }

    return bestMove;
}

int AI::minimax(Board& board, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0 || board.checkGameOver()) {
        return evaluateBoard(board);
    }
    if (maximizingPlayer) {
        int maxEval = std::numeric_limits<int>::min();
        std::vector<Move> possibleMoves = board.getAllPossibleMoves(maximizingPlayer);
        for (const Move& move : possibleMoves) {
            Board newBoard = board;
            newBoard.movePiece(move, maximizingPlayer);
            int eval = minimax(newBoard, depth - 1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        std::vector<Move> possibleMoves = board.getAllPossibleMoves(1 - maximizingPlayer);
        for (const Move& move : possibleMoves) {
            Board newBoard = board;
            newBoard.movePiece(move, 1 - maximizingPlayer);
            int eval = minimax(newBoard, depth - 1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}
int AI::evaluateBoard(const Board& board) {
    int score = 0;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            int piece = board.getPieceAt(sf::Vector2i(x, y));
            if (piece == 1) {
                score -= 10;
                if (isPieceInDanger(board, sf::Vector2i(x, y), piece)) {
                    score -= 20;
                }
                if (isInCenter(sf::Vector2i(x, y))) {
                    score -= 3;
                }
                if (isOnEdge(sf::Vector2i(x, y))) {
                    score -= 2;
                }
            } else if (piece == 101) {
                score -= 30;
                if (isPieceInDanger(board, sf::Vector2i(x, y), piece)) {
                    score -= 40;
                }
                if (isInCenter(sf::Vector2i(x, y))) {
                    score -= 5;
                }
                if (isOnEdge(sf::Vector2i(x, y))) {
                    score -= 10;
                }
            } else if (piece == 2) {
                score += 10;
                if (isPieceInDanger(board, sf::Vector2i(x, y), piece)) {
                    score -= 20;
                }
                if (isInCenter(sf::Vector2i(x, y))) {
                    score += 3;
                }
                if (isOnEdge(sf::Vector2i(x, y))) {
                    score += 2;
                }
            } else if (piece == 102) {
                score += 30;
                if (isPieceInDanger(board, sf::Vector2i(x, y), piece)) {
                    score -= 40;
                }
                if (isInCenter(sf::Vector2i(x, y))) {
                    score += 5;
                }
                if (isOnEdge(sf::Vector2i(x, y))) {
                    score += 10;
                }
            }
        }
    }
    return score;
}

bool AI::isPieceInDanger(const Board& board, const sf::Vector2i& position, int pieceType) {
    int directions[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

    for (auto& direction : directions) {
        int newX = position.x + 2 * direction[1];
        int newY = position.y + 2 * direction[0];
        int midX = position.x + direction[1];
        int midY = position.y + direction[0];

        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            int opponentPiece = (pieceType == 1 || pieceType == 101) ? 2 : 1;
            int opponentQueen = (pieceType == 1 || pieceType == 101) ? 102 : 101;

            if (board.getPieceAt(sf::Vector2i(newX, newY)) == 0 &&
                (board.getPieceAt(sf::Vector2i(midX, midY)) == opponentPiece ||
                 board.getPieceAt(sf::Vector2i(midX, midY)) == opponentQueen)) {
                return true;
            }
        }
    }
    return false;
}

bool AI::willMoveExposePiece(const Board& board, const Move& move, int playerTurn) {
    Board newBoard = board;
    newBoard.movePiece(move, playerTurn);

    sf::Vector2i newPiecePos(move.toX, move.toY);
    return isPieceInDanger(newBoard, newPiecePos, newBoard.getPieceAt(newPiecePos));
}

std::pair<int, int> AI::countPieces(const Board& board) const {
    int player1Pieces = 0;
    int player2Pieces = 0;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            int piece = board.getPieceAt(sf::Vector2i(x, y));
            if (piece == 1 || piece == 101) {
                ++player1Pieces;
            } else if (piece == 2 || piece == 102) {
                ++player2Pieces;
            }
        }
    }
    return {player1Pieces, player2Pieces};
}
bool AI::isInCenter(const sf::Vector2i& position) const {
    int x = position.x;
    int y = position.y;
    // Centralne cztery pola planszy
    return (x >= 3 && x <= 4 && y >= 3 && y <= 4);
}

bool AI::isOnEdge(const sf::Vector2i& position) const {
    int x = position.x;
    int y = position.y;
    // Linie boczne planszy
    return (x == 0 || x == 7 || y == 0 || y == 7);
}
Move AI::findMostAdvancedPawnMove(Board& board, int playerTurn) {
    int bestValue = std::numeric_limits<int>::min();
    Move bestMove;

    std::vector<Move> possibleMoves = board.getAllPossibleMoves(playerTurn);

    int mostAdvancedRow = playerTurn == 0 ? 8 : -1;

    for (const Move& move : possibleMoves) {
        if (!willMoveExposePiece(board, move, playerTurn)) {
            int currentRow = playerTurn == 0 ? move.fromY : (7 - move.fromY);

            if ((playerTurn == 0 && currentRow < mostAdvancedRow) ||
                (playerTurn == 1 && currentRow > mostAdvancedRow)) {
                mostAdvancedRow = currentRow;
                bestMove = move;
            }
        }
    }

    return bestMove;
}