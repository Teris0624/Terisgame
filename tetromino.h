#pragma once
#ifndef TETROMINO_H
#define TETROMINO_H

#include <SFML/Graphics.hpp>

class Tetromino {
public:
    explicit Tetromino(int t);
    void rotate();
    void rotateBack();
    void resetToState0();
    int getCurrentState() const { return m_currentState; }
    void setCurrentState(int state);

    int shape[4][4]{};
    int type{ 0 };
    int x{ 0 };
    int y{ 0 };
    sf::Color color;

private:
    int m_currentState{ 0 };
    int m_shapes[4][4][4];
    void buildAllRotations();
};

#endif