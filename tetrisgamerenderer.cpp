#include "TetrisGame.h"
#include "Tetromino.h"
#include "Constants.h"

void TetrisGame::render() {
    if (!m_window.isOpen()) return;

    // 检查窗口大小是否改变
    static sf::Vector2u lastSize = m_window.getSize();
    sf::Vector2u currentSize = m_window.getSize();
    if (currentSize != lastSize) {
        lastSize = currentSize;
        if (m_uiManager) {
            m_uiManager->updateScale();
        }
        // 更新视口
        sf::View newView(sf::FloatRect({ 0.0f, 0.0f },
            { static_cast<float>(currentSize.x),
            static_cast<float>(currentSize.y) }));
        m_window.setView(newView);
    }

    m_window.clear();

    if (!m_uiManager) {
        m_window.display();
        return;
    }

    m_uiManager->updateScale();

    switch (m_state) {
    case GameState::MENU:
        m_uiManager->drawMainMenu(m_titleBobOffset, m_selectedMenuItem);
        break;

    case GameState::PLAYING:
        if (m_mapManager && m_currentPiece && m_nextPiece && m_settingsManager) {
            int w = getCurrentWidth();
            int h = getCurrentHeight();
            w = std::max(1, std::min(w, 16));
            h = std::max(1, std::min(h, 20));

            int temp[20][16] = { 0 };
            for (int i = 0; i < h && i < 20; ++i) {
                for (int j = 0; j < w && j < 16; ++j) {
                    temp[i][j] = m_mapManager->getCell(j, i);
                }
            }

            GameSettings s = m_settingsManager->getSettings();
            bool showGhost = s.showGhostPiece;

            m_uiManager->drawGameArea(temp, w, h, m_currentPiece.get(), showGhost);
            m_uiManager->drawGameUI(m_score, m_level, m_linesCleared, w,
                m_nextPiece->color, m_nextPiece->shape);
        }
        break;

    case GameState::PAUSED:
        if (m_mapManager && m_currentPiece && m_nextPiece) {
            int w = getCurrentWidth();
            int h = getCurrentHeight();
            w = std::max(1, std::min(w, 16));
            h = std::max(1, std::min(h, 20));

            int temp[20][16] = { 0 };
            for (int i = 0; i < h && i < 20; ++i) {
                for (int j = 0; j < w && j < 16; ++j) {
                    temp[i][j] = m_mapManager->getCell(j, i);
                }
            }

            m_uiManager->drawGameArea(temp, w, h, m_currentPiece.get(), false);
            m_uiManager->drawGameUI(m_score, m_level, m_linesCleared, w,
                m_nextPiece->color, m_nextPiece->shape);
        }

        if (m_uiManager) {
            m_uiManager->setSelectedPauseItem(m_selectedPauseItem);
            m_uiManager->drawPauseMenu();
        }
        break;

    case GameState::GAME_OVER:
        if (m_mapManager && m_currentPiece && m_nextPiece) {
            int w = getCurrentWidth();
            int h = getCurrentHeight();
            w = std::max(1, std::min(w, 16));
            h = std::max(1, std::min(h, 20));

            int temp[20][16] = { 0 };
            for (int i = 0; i < h && i < 20; ++i) {
                for (int j = 0; j < w && j < 16; ++j) {
                    temp[i][j] = m_mapManager->getCell(j, i);
                }
            }

            m_uiManager->drawGameArea(temp, w, h, m_currentPiece.get(), false);
            m_uiManager->drawGameUI(m_score, m_level, m_linesCleared, w,
                m_nextPiece->color, m_nextPiece->shape);
        }
        m_uiManager->drawGameOver(m_score);
        break;

    case GameState::SETTINGS:
        if (m_settingsManager && m_uiManager) {
            m_uiManager->drawSettingsMenu(m_selectedSettingsItem, m_settingsManager->getSettings());
        }
        break;

    case GameState::MAP_SETTINGS:
        if (m_settingsManager && m_uiManager) {
            GameSettings s = m_settingsManager->getSettings();
            m_uiManager->drawMapSettingsMenu(m_selectedMapItem, s.gridWidth, s.gridHeight, m_currentPresetId);
        }
        break;

    case GameState::KEY_SETTINGS:
        if (m_inputManager && m_uiManager) {
            m_uiManager->drawKeySettingsMenu(m_selectedKeyItem,
                m_inputManager->getKeyBindings(),
                m_waitingAction);
        }
        break;

    default:
        break;
    }

    m_window.display();
}