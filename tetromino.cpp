#include "Tetromino.h"
#include "Constants.h"

Tetromino::Tetromino(int t) : type(t), m_currentState(0) {
    if (t < 0 || t >= 7) type = 0;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m_shapes[0][i][j] = TETROMINOS[type][i][j];

    buildAllRotations();

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            shape[i][j] = m_shapes[0][i][j];

    color = COLORS[type + 1];
}

void Tetromino::buildAllRotations() {
    for (int state = 1; state < 4; state++) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m_shapes[state][j][3 - i] = m_shapes[state - 1][i][j];
            }
        }
    }
}

void Tetromino::rotate() {
    m_currentState = (m_currentState + 1) % 4;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            shape[i][j] = m_shapes[m_currentState][i][j];
}

void Tetromino::rotateBack() {
    m_currentState = (m_currentState - 1 + 4) % 4;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            shape[i][j] = m_shapes[m_currentState][i][j];
}

void Tetromino::resetToState0() {
    m_currentState = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            shape[i][j] = m_shapes[0][i][j];
}

void Tetromino::setCurrentState(int state) {
    m_currentState = state % 4;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            shape[i][j] = m_shapes[m_currentState][i][j];
}