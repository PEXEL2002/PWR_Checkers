#include "Board.h"
#include <iostream>

Board::Board() {
    board = {
            {0, 2, 0, 2, 0, 2, 0, 2},
            {2, 0, 2, 0, 2, 0, 2, 0},
            {0, 2, 0, 2, 0, 2, 0, 2},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 1, 0, 1, 0, 1, 0},
            {0, 1, 0, 1, 0, 1, 0, 1},
            {1, 0, 1, 0, 1, 0, 1, 0}
    };
    colorPlayer1 = 0;
    selectedPiece = sf::Vector2i(-1, -1);
    isMultiJump = false;
    isGameOver = false;
    winner = -1;

    if (!whitePawnTexture.loadFromFile("assets/W_pawn.png") ||
        !whiteQueenTexture.loadFromFile("assets/W_queen.png") ||
        !blackPawnTexture.loadFromFile("assets/B_pawn.png") ||
        !blackQueenTexture.loadFromFile("assets/B_queen.png")) {
        std::cerr << "Error loading piece textures" << std::endl;
    }
}

Board::~Board() = default;

void Board::drawBoard(sf::RenderWindow& window, const sf::Vector2u& windowSize) {
    sf::Texture texture;
    if (!texture.loadFromFile("assets/Board.png")) {
        std::cerr << "Error loading board texture" << std::endl;
        return;
    }

    sf::Sprite sprite(texture);

    float scaleX = static_cast<float>(windowSize.x) / texture.getSize().x;
    float scaleY = static_cast<float>(windowSize.y) / texture.getSize().y;

    if (colorPlayer1 == 1) {
        sprite.setScale(-scaleX, scaleY);
        sprite.setPosition(windowSize.x, 0);
    } else {
        sprite.setScale(scaleX, scaleY);
    }

    window.draw(sprite);
}

void Board::drawPieces(sf::RenderWindow& window) {
    sf::Sprite whitePawnSprite(whitePawnTexture);
    sf::Sprite whiteQueenSprite(whiteQueenTexture);
    sf::Sprite blackPawnSprite(blackPawnTexture);
    sf::Sprite blackQueenSprite(blackQueenTexture);

    float tileSize = window.getSize().x / 8.0f;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            sf::Sprite* pieceSprite = nullptr;

            if (board[i][j] == 1) {
                pieceSprite = &whitePawnSprite;
            } else if (board[i][j] == 2) {
                pieceSprite = &blackPawnSprite;
            } else if (board[i][j] == 101) {
                pieceSprite = &whiteQueenSprite;
            } else if (board[i][j] == 102) {
                pieceSprite = &blackQueenSprite;
            }

            if (pieceSprite) {
                pieceSprite->setScale(tileSize / pieceSprite->getTexture()->getSize().x,
                                      tileSize / pieceSprite->getTexture()->getSize().y);
                pieceSprite->setPosition(j * tileSize, i * tileSize);
                window.draw(*pieceSprite);
            }
        }
    }
}

bool Board::selectPiece(const sf::Vector2i& position, int playerTurn) {
    int piece = getPieceAt(position);
    if ((playerTurn == 0 && (piece == 1 || piece == 101)) || (playerTurn == 1 && (piece == 2 || piece == 102))) {
        selectedPiece = position;
        return true;
    }
    selectedPiece = sf::Vector2i(-1, -1);
    return false;
}

bool Board::movePiece(const Move& move, int playerTurn) {
    bool jumpAvailable = isJumpAvailable(playerTurn);
    bool validMove = false;
    if (board[move.fromY][move.fromX] == 101 || board[move.fromY][move.fromX] == 102) {
        validMove = isValidQueenMove(move, playerTurn) || isValidQueenJump(move, playerTurn);
    } else {
        validMove = isValidMove(move, playerTurn) || isValidJump(move, playerTurn);
    }
    if (jumpAvailable && !isValidJump(move, playerTurn) && !isValidQueenJump(move, playerTurn)) {
        return false;
    }
    if (validMove) {
        bool jumpPerformed = false;
        if (abs(move.fromX - move.toX) == 2 && abs(move.fromY - move.toY) == 2) {
            int midX = (move.fromX + move.toX) / 2;
            int midY = (move.fromY + move.toY) / 2;
            board[midY][midX] = 0;
            jumpPerformed = true;
        } else if (isValidQueenJump(move, playerTurn)) {
            int stepX = (move.toX - move.fromX) / abs(move.toX - move.fromX);
            int stepY = (move.toY - move.fromY) / abs(move.toY - move.fromY);
            for (int i = 1; i < abs(move.toX - move.fromX); ++i) {
                int midX = move.fromX + i * stepX;
                int midY = move.fromY + i * stepY;
                if (isOpponentPiece(board[midY][midX], playerTurn)) {
                    board[midY][midX] = 0;
                    jumpPerformed = true;
                }
            }
        }
        board[move.toY][move.toX] = board[move.fromY][move.fromX];
        board[move.fromY][move.fromX] = 0;
        bool promoted = false;
        if ((playerTurn == 0 && move.toY == 0 && board[move.toY][move.toX] == 1) ||
            (playerTurn == 1 && move.toY == 7 && board[move.toY][move.toX] == 2)) {
            promoteToQueen(move.toX, move.toY);
            promoted = true;
        }
        selectedPiece = sf::Vector2i(move.toX, move.toY);
        if (jumpPerformed && areJumpsAvailableForPiece(selectedPiece, playerTurn)) {
            isMultiJump = true;
        } else {
            isMultiJump = false;
            selectedPiece = sf::Vector2i(-1, -1);
            if (!promoted) {
                playerTurn = 1 - playerTurn;
            }
        }
        checkGameOver();
        return true;
    }
    return false;
}
bool Board::isValidMove(const Move& move, int playerTurn) const {
    int piece = board[move.fromY][move.fromX];
    int target = board[move.toY][move.toX];

    if ((playerTurn == 0 && (piece != 1 && piece != 101)) || (playerTurn == 1 && (piece != 2 && piece != 102))) {
        return false;
    }

    if (target == 0) {
        int dx = abs(move.toX - move.fromX);
        int dy = abs(move.toY - move.fromY);

        if ((piece == 1 && move.toY >= move.fromY) || (piece == 2 && move.toY <= move.fromY)) {
            return false;
        }

        if (dx == 1 && dy == 1) {
            return true;
        }
    }
    return false;
}

bool Board::isValidJump(const Move& move, int playerTurn) const {
    int piece = board[move.fromY][move.fromX];
    int target = board[move.toY][move.toX];

    if ((playerTurn == 0 && (piece != 1 && piece != 101)) || (playerTurn == 1 && (piece != 2 && piece != 102))) {
        return false;
    }

    if (target == 0) {
        int dx = abs(move.toX - move.fromX);
        int dy = abs(move.toY - move.fromY);

        if (dx == 2 && dy == 2) {
            int midX = (move.fromX + move.toX) / 2;
            int midY = (move.fromY + move.toY) / 2;
            int midPiece = board[midY][midX];

            if (isOpponentPiece(midPiece, playerTurn)) {
                return true;
            }
        }
    }
    return false;
}

bool Board::isValidQueenMove(const Move& move, int playerTurn) const {
    int piece = board[move.fromY][move.fromX];
    int target = board[move.toY][move.toX];

    if ((playerTurn == 0 && piece != 101) || (playerTurn == 1 && piece != 102)) {
        return false;
    }

    if (target == 0) {
        int dx = abs(move.toX - move.fromX);
        int dy = abs(move.toY - move.fromY);

        if (dx == dy) {
            int stepX = (move.toX - move.fromX) / dx;
            int stepY = (move.toY - move.fromY) / dy;

            for (int i = 1; i < dx; ++i) {
                if (board[move.fromY + i * stepY][move.fromX + i * stepX] != 0) {
                    return false;
                }
            }

            return true;
        }
    }
    return false;
}

bool Board::isValidQueenJump(const Move& move, int playerTurn) const {
    int piece = board[move.fromY][move.fromX];
    int target = board[move.toY][move.toX];
    if ((playerTurn == 0 && piece != 101) || (playerTurn == 1 && piece != 102)) {
        return false;
    }
    if (target == 0) {
        int dx = abs(move.toX - move.fromX);
        int dy = abs(move.toY - move.fromY);
        if (dx == dy && dx > 1) {
            int stepX = (move.toX - move.fromX) / dx;
            int stepY = (move.toY - move.fromY) / dy;
            bool opponentPieceFound = false;
            int midX = -1, midY = -1;
            for (int i = 1; i < dx; ++i) {
                int currentX = move.fromX + i * stepX;
                int currentY = move.fromY + i * stepY;
                int currentPiece = board[currentY][currentX];

                if (currentPiece != 0) {
                    if (isOpponentPiece(currentPiece, playerTurn)) {
                        if (opponentPieceFound) {
                            return false;
                        }
                        opponentPieceFound = true;
                        midX = currentX;
                        midY = currentY;
                    } else {
                        return false;
                    }
                }
            }

            if (opponentPieceFound) {
                return true;
            }
        }
    }
    return false;
}

bool Board::isOpponentPiece(int piece, int playerTurn) {
    if (playerTurn == 0 && (piece == 2 || piece == 102)) return true;
    if (playerTurn == 1 && (piece == 1 || piece == 101)) return true;
    return false;
}

int Board::getPieceAt(const sf::Vector2i& position) const {
    if (position.x >= 0 && position.x < 8 && position.y >= 0 && position.y < 8) {
        return board[position.y][position.x];
    }
    return -1;
}

sf::Vector2i Board::getSelectedPiece() const {
    return selectedPiece;
}

void Board::deselectPiece() {
    selectedPiece = sf::Vector2i(-1, -1);
    isMultiJump = false;
}

bool Board::areJumpsAvailableForPiece(const sf::Vector2i& position, int playerTurn) const {
    int piece = board[position.y][position.x];
    if ((playerTurn == 0 && (piece != 1 && piece != 101)) || (playerTurn == 1 && (piece != 2 && piece != 102))) {
        return false;
    }

    int directions[4][2] = {{-2, -2}, {-2, 2}, {2, -2}, {2, 2}};

    for (auto& direction : directions) {
        int newX = position.x + direction[1];
        int newY = position.y + direction[0];

        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            int midX = (position.x + newX) / 2;
            int midY = (position.y + newY) / 2;
            int midPiece = board[midY][midX];
            int target = board[newY][newX];

            if (target == 0 && isOpponentPiece(midPiece, playerTurn)) {
                return true;
            }
        }
    }

    if (piece == 101 || piece == 102) {
        int queenDirections[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (auto& direction : queenDirections) {
            int newX = position.x + direction[1];
            int newY = position.y + direction[0];
            while (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                if (isValidQueenJump(Move(position.x, position.y, newX, newY), playerTurn)) {
                    return true;
                }
                newX += direction[1];
                newY += direction[0];
            }
        }
    }
    return false;
}

bool Board::getIsMultiJump() const {
    return isMultiJump;
}

void Board::updateMultipleJump(int playerTurn) {
    if (isMultiJump) {
        if (!areJumpsAvailableForPiece(selectedPiece, playerTurn)) {
            deselectPiece();
        }
    }
}

bool Board::canPieceJump(const sf::Vector2i& position, int playerTurn) const {
    int piece = board[position.y][position.x];
    if ((playerTurn == 0 && (piece != 1 && piece != 101)) || (playerTurn == 1 && (piece != 2 && piece != 102))) {
        return false;
    }

    int directions[4][2] = {{-2, -2}, {-2, 2}, {2, -2}, {2, 2}};

    for (auto& direction : directions) {
        int newX = position.x + direction[1];
        int newY = position.y + direction[0];

        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            int midX = (position.x + newX) / 2;
            int midY = (position.y + newY) / 2;
            int midPiece = board[midY][midX];
            int target = board[newY][newX];

            if (target == 0 && isOpponentPiece(midPiece, playerTurn)) {
                return true;
            }
        }
    }

    if (piece == 101 || piece == 102) {
        int queenDirections[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (auto& direction : queenDirections) {
            int newX = position.x + direction[1];
            int newY = position.y + direction[0];
            while (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                if (isValidQueenJump(Move(position.x, position.y, newX, newY), playerTurn)) {
                    return true;
                }
                newX += direction[1];
                newY += direction[0];
            }
        }
    }
    return false;
}

void Board::removePiece(const sf::Vector2i& position) {
    board[position.y][position.x] = 0;
}

void Board::promoteToQueen(int x, int y) {
    if (board[y][x] == 1) {
        board[y][x] = 101;
    } else if (board[y][x] == 2) {
        board[y][x] = 102;
    }
}

bool Board::checkGameOver() {
    int player1Pieces = 0;
    int player2Pieces = 0;
    bool player1HasMoves = false;
    bool player2HasMoves = false;
    bool player1HasOnlyQueens = true;
    bool player2HasOnlyQueens = true;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            int piece = board[y][x];
            if (piece == 1 || piece == 101) {
                ++player1Pieces;
                if (piece == 1) {
                    player1HasOnlyQueens = false;
                }
                if (!player1HasMoves && hasValidMoves(0)) {
                    player1HasMoves = true;
                }
            } else if (piece == 2 || piece == 102) {
                ++player2Pieces;
                if (piece == 2) {
                    player2HasOnlyQueens = false;
                }
                if (!player2HasMoves && hasValidMoves(1)) {
                    player2HasMoves = true;
                }
            }
        }
    }

    if (player1Pieces == 0 || !player1HasMoves) {
        isGameOver = true;
        winner = 2;
    } else if (player2Pieces == 0 || !player2HasMoves) {
        isGameOver = true;
        winner = 1;
    } else if (player1HasOnlyQueens && player2HasOnlyQueens && !player1HasMoves && !player2HasMoves) {
        isGameOver = true;
        winner = 0;
    } else {
        isGameOver = false;
    }

    return isGameOver;
}

bool Board::hasValidMoves(int playerTurn) const {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if ((playerTurn == 0 && (board[y][x] == 1 || board[y][x] == 101)) ||
                (playerTurn == 1 && (board[y][x] == 2 || board[y][x] == 102))) {
                if (canPieceJump(sf::Vector2i(x, y), playerTurn)) {
                    return true;
                }

                int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
                for (auto& direction : directions) {
                    int newX = x + direction[1];
                    int newY = y + direction[0];

                    if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                        Move move(x, y, newX, newY);
                        if (isValidMove(move, playerTurn) || isValidQueenMove(move, playerTurn)) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

int Board::getWinner() const {
    return winner;
}
std::vector<Move> Board::getAllPossibleMoves(int playerTurn) const {
    std::vector<Move> moves;
    bool jumpAvailable = isJumpAvailable(playerTurn);
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if ((playerTurn == 0 && (board[y][x] == 1 || board[y][x] == 101)) ||
                (playerTurn == 1 && (board[y][x] == 2 || board[y][x] == 102))) {
                if (jumpAvailable) {
                    for (int dy = -2; dy <= 2; dy += 4) {
                        for (int dx = -2; dx <= 2; dx += 4) {
                            int newX = x + dx;
                            int newY = y + dy;
                            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                                Move jump(x, y, newX, newY);
                                if (isValidJump(jump, playerTurn) || isValidQueenJump(jump, playerTurn)) {
                                    moves.push_back(jump);
                                }
                            }
                        }
                    }
                } else {
                    for (int dy = -1; dy <= 1; dy += 2) {
                        for (int dx = -1; dx <= 1; dx += 2) {
                            int newX = x + dx;
                            int newY = y + dy;
                            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                                Move move(x, y, newX, newY);
                                if (isValidMove(move, playerTurn) || isValidQueenMove(move, playerTurn)) {
                                    moves.push_back(move);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return moves;
}

bool Board::isJumpAvailable(int playerTurn) const {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if ((playerTurn == 0 && (board[y][x] == 1 || board[y][x] == 101)) ||
                (playerTurn == 1 && (board[y][x] == 2 || board[y][x] == 102))) {
                if (canPieceJump(sf::Vector2i(x, y), playerTurn)) {
                    return true;
                }
            }
        }
    }
    return false;
}
