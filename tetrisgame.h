#pragma once
#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include "Constants.h"
#include "InputManager.h"
#include "SettingsManager.h"
#include "MapManager.h"
#include "UIManager.h"

class Tetromino;

class TetrisGame {
public:
    TetrisGame();
    ~TetrisGame();
    void run();

private:
    // ========== 初始化 ==========
    void initGame();
    void loadFont();

    // ========== 事件处理 ==========
    void handleEvents();
    void handleMouseClick(const sf::Vector2i& mousePos);
    void handlePlayingInput(sf::Keyboard::Key key);
    void handlePausedInput(sf::Keyboard::Key key);
    void handleMenuInput();
    void handleSettingsInput();
    void handleMapSettingsInput();
    void handleKeySettingsInput();

    // ========== 游戏逻辑 ==========
    void update(float dt);
    bool movePiece(int dx, int dy);
    void rotatePiece();
    void hardDrop();
    bool checkCollision(const Tetromino* piece, int x, int y);
    void mergePiece();
    void clearLines();
    void updateScore(int lines);
    void spawnNewPiece();
    void resetGame();

    // ========== 渲染 ==========
    void render();

    // ========== 辅助函数 ==========
    int getCurrentWidth() const;
    int getCurrentHeight() const;

    // ========== 成员变量 ==========
    sf::RenderWindow m_window;
    std::unique_ptr<Tetromino> m_currentPiece;
    std::unique_ptr<Tetromino> m_nextPiece;
    std::unique_ptr<MapManager> m_mapManager;
    std::unique_ptr<InputManager> m_inputManager;
    std::unique_ptr<SettingsManager> m_settingsManager;
    std::unique_ptr<UIManager> m_uiManager;

    GameState m_state{ GameState::MENU };
    int m_score{ 0 };
    int m_level{ 1 };
    int m_linesCleared{ 0 };
    float m_fallSpeed{ 0.5f };
    float m_fallTimer{ 0.0f };
    // 在 TetrisGame.h 的 private 部分确保有这些声
    bool canRotateAt(int x, int y);
    // 在 TetrisGame.h 的 private 部分添加
    void fixPosition(Tetromino* piece, int& x, int& y);
    std::mt19937 m_rng;
    std::uniform_int_distribution<int> m_dist{ 0, 6 };

    sf::Font m_font;
    bool m_fontLoaded{ false };

    int m_selectedMenuItem{ 0 };
    int m_selectedSettingsItem{ 0 };
    int m_selectedMapItem{ 0 };
    int m_selectedKeyItem{ 0 };
    int m_selectedPauseItem{ 0 };
    int m_currentPresetId{ 0 };
    float m_titleBobOffset{ 0.0f };
    float m_titleBobTimer{ 0.0f };

    bool m_waitingForKey{ false };
    std::string m_waitingAction;

    bool m_leftPressed{ false };
    bool m_rightPressed{ false };
    float m_moveDelay{ 0.0f };
    float m_moveRepeatDelay{ 0.15f };
    float m_moveRepeatInterval{ 0.05f };

    float m_rotateCooldown{ 0.0f };
    float m_rotateDelay{ 0.05f };

    bool m_initialized{ false };
};

#endif