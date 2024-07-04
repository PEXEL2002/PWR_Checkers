#pragma once

struct Move{
    int fromX, fromY, toX, toY;
    Move(int fromX, int fromY, int toX, int toY) : fromX(fromX), fromY(fromY), toX(toX), toY(toY) {}
    Move() : fromX(0), fromY(0), toX(0), toY(0) {}
};
