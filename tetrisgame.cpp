#include "TetrisGame.h"
#include "Tetromino.h"
#include "Constants.h"
#include <iostream>
#include <chrono>
#include <cmath>
#include <memory>

TetrisGame::TetrisGame() :
    m_rng(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())) {
    try {
        m_window.create(sf::VideoMode({ BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT }), "Tetris");
        if (!m_window.isOpen()) {
            std::cerr << "Error: Failed to create window!\n";
            return;
        }
        m_window.setFramerateLimit(60);

        loadFont();
        m_settingsManager = std::make_unique<SettingsManager>();
        m_inputManager = std::make_unique<InputManager>();
        m_mapManager = std::make_unique<MapManager>(DEFAULT_GRID_WIDTH, DEFAULT_GRID_HEIGHT);

        if (!m_settingsManager || !m_inputManager || !m_mapManager) {
            std::cerr << "Error: Failed to create managers!\n";
            return;
        }

        m_settingsManager->loadFromFile("settings.cfg");
        m_inputManager->loadFromFile("keys.cfg");

        GameSettings settings = m_settingsManager->getSettings();
        m_fallSpeed = settings.initialFallSpeed;
        m_mapManager->resize(settings.gridWidth, settings.gridHeight);

        if (m_fontLoaded) {
            m_uiManager = std::make_unique<UIManager>(m_window, m_font);
        }

        initGame();
        m_currentPiece = std::make_unique<Tetromino>(m_dist(m_rng));
        m_nextPiece = std::make_unique<Tetromino>(m_dist(m_rng));

        if (!m_currentPiece || !m_nextPiece) {
            std::cerr << "Error: Failed to create tetrominos!\n";
            return;
        }

        int startX = getCurrentWidth() / 2 - 2;
        startX = std::max(0, std::min(startX, getCurrentWidth() - 4));
        m_currentPiece->x = startX;
        m_currentPiece->y = 0;

        m_initialized = true;
        std::cout << "Game initialized successfully!\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << '\n';
        m_initialized = false;
    }
}

TetrisGame::~TetrisGame() {
    if (m_settingsManager) m_settingsManager->saveToFile("settings.cfg");
    if (m_inputManager) m_inputManager->saveToFile("keys.cfg");
}

void TetrisGame::run() {
    if (!m_initialized) {
        std::cerr << "Error: Game not properly initialized!\n";
        return;
    }
    sf::Clock clock;
    while (m_window.isOpen()) {
        float dt = clock.restart().asSeconds();
        dt = std::min(dt, 0.1f);
        handleEvents();
        update(dt);
        render();
    }
}

int TetrisGame::getCurrentWidth() const {
    if (!m_settingsManager) return DEFAULT_GRID_WIDTH;
    int w = m_settingsManager->getSettings().gridWidth;
    return std::max(MIN_GRID_WIDTH, std::min(w, MAX_GRID_WIDTH));
}

int TetrisGame::getCurrentHeight() const {
    if (!m_settingsManager) return DEFAULT_GRID_HEIGHT;
    int h = m_settingsManager->getSettings().gridHeight;
    return std::max(MIN_GRID_HEIGHT, std::min(h, MAX_GRID_HEIGHT));
}

void TetrisGame::initGame() {
    if (!m_settingsManager || !m_mapManager) return;
    m_fallSpeed = m_settingsManager->getSettings().initialFallSpeed;
    m_mapManager->clear();
}

void TetrisGame::loadFont() {
    for (const auto& path : getFontPaths()) {
        if (m_font.openFromFile(path)) {
            m_fontLoaded = true;
            std::cout << "Font loaded: " << path << '\n';
            return;
        }
    }
    std::cout << "Warning: Font not loaded!\n";
}