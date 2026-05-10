#include "TetrisGame.h"
#include "Constants.h"
#include <vector>

void TetrisGame::handleMenuInput() {
    if (!m_inputManager) return;

    if (m_inputManager->isActionJustPressed("Move Down")) {
        m_selectedMenuItem = (m_selectedMenuItem + 1) % 5;
    }
    if (m_inputManager->isActionJustPressed("Move Up")) {
        m_selectedMenuItem = (m_selectedMenuItem - 1 + 5) % 5;
    }
    if (m_inputManager->isActionJustPressed("Rotate") || m_inputManager->isActionJustPressed("Hard Drop")) {
        switch (m_selectedMenuItem) {
        case 0: resetGame(); break;
        case 1: m_state = GameState::SETTINGS; m_selectedSettingsItem = 0; break;
        case 2: m_state = GameState::MAP_SETTINGS; m_selectedMapItem = 0; break;
        case 3: m_state = GameState::KEY_SETTINGS; m_selectedKeyItem = 0; break;
        case 4: m_window.close(); break;
        }
    }
}

void TetrisGame::handleSettingsInput() {
    if (!m_settingsManager || !m_inputManager) return;
    GameSettings s = m_settingsManager->getSettings();

    if (m_inputManager->isActionJustPressed("Move Up")) {
        m_selectedSettingsItem = (m_selectedSettingsItem - 1 + 4) % 4;
    }
    if (m_inputManager->isActionJustPressed("Move Down")) {
        m_selectedSettingsItem = (m_selectedSettingsItem + 1) % 4;
    }
    if (m_inputManager->isActionJustPressed("Move Left")) {
        switch (m_selectedSettingsItem) {
        case 0:
            s.gridWidth = std::max(MIN_GRID_WIDTH, s.gridWidth - 1);
            if (m_mapManager) m_mapManager->resize(s.gridWidth, s.gridHeight);
            break;
        case 1:
            s.initialFallSpeed = std::max(0.05f, s.initialFallSpeed - 0.05f);
            m_fallSpeed = s.initialFallSpeed;
            break;
        case 2:
            s.scoreMultiplier = std::max(1, s.scoreMultiplier - 1);
            break;
        case 3:
            s.showGhostPiece = !s.showGhostPiece;
            break;
        }
        m_settingsManager->setSettings(s);
    }
    if (m_inputManager->isActionJustPressed("Move Right")) {
        switch (m_selectedSettingsItem) {
        case 0:
            s.gridWidth = std::min(MAX_GRID_WIDTH, s.gridWidth + 1);
            if (m_mapManager) m_mapManager->resize(s.gridWidth, s.gridHeight);
            break;
        case 1:
            s.initialFallSpeed = std::min(2.0f, s.initialFallSpeed + 0.05f);
            m_fallSpeed = s.initialFallSpeed;
            break;
        case 2:
            s.scoreMultiplier = std::min(10, s.scoreMultiplier + 1);
            break;
        case 3:
            s.showGhostPiece = !s.showGhostPiece;
            break;
        }
        m_settingsManager->setSettings(s);
    }
    if (m_inputManager->isActionJustPressed("Menu")) {
        m_state = GameState::MENU;
    }
}

void TetrisGame::handleMapSettingsInput() {
    if (!m_settingsManager || !m_mapManager || !m_inputManager) return;
    GameSettings s = m_settingsManager->getSettings();

    if (m_inputManager->isActionJustPressed("Move Up")) {
        m_selectedMapItem = (m_selectedMapItem - 1 + 6) % 6;
    }
    if (m_inputManager->isActionJustPressed("Move Down")) {
        m_selectedMapItem = (m_selectedMapItem + 1) % 6;
    }
    if (m_inputManager->isActionJustPressed("Move Left")) {
        switch (m_selectedMapItem) {
        case 0:
            s.gridWidth = std::max(MIN_GRID_WIDTH, s.gridWidth - 1);
            m_settingsManager->setSettings(s);
            m_mapManager->resize(s.gridWidth, s.gridHeight);
            break;
        case 1:
            s.gridHeight = std::max(MIN_GRID_HEIGHT, s.gridHeight - 1);
            m_settingsManager->setSettings(s);
            m_mapManager->resize(s.gridWidth, s.gridHeight);
            break;
        case 2:
            m_currentPresetId = (m_currentPresetId - 1 + 5) % 5;
            m_mapManager->loadPreset(m_currentPresetId);
            break;
        }
    }
    if (m_inputManager->isActionJustPressed("Move Right")) {
        switch (m_selectedMapItem) {
        case 0:
            s.gridWidth = std::min(MAX_GRID_WIDTH, s.gridWidth + 1);
            m_settingsManager->setSettings(s);
            m_mapManager->resize(s.gridWidth, s.gridHeight);
            break;
        case 1:
            s.gridHeight = std::min(MAX_GRID_HEIGHT, s.gridHeight + 1);
            m_settingsManager->setSettings(s);
            m_mapManager->resize(s.gridWidth, s.gridHeight);
            break;
        case 2:
            m_currentPresetId = (m_currentPresetId + 1) % 5;
            m_mapManager->loadPreset(m_currentPresetId);
            break;
        }
    }
    if (m_inputManager->isActionJustPressed("Rotate")) {
        switch (m_selectedMapItem) {
        case 3: m_mapManager->saveToFile("custom_map.txt"); break;
        case 4: m_mapManager->loadFromFile("custom_map.txt"); break;
        case 5: m_mapManager->clear(); break;
        }
    }
    if (m_inputManager->isActionJustPressed("Menu")) {
        m_state = GameState::MENU;
    }
}

void TetrisGame::handleKeySettingsInput() {
    if (!m_inputManager) return;
    auto bindings = m_inputManager->getKeyBindings();
    std::vector<std::string> actions;
    for (const auto& b : bindings) {
        actions.push_back(b.first);
    }
    if (actions.empty()) return;

    if (m_inputManager->isActionJustPressed("Move Up")) {
        m_selectedKeyItem = (m_selectedKeyItem - 1 + static_cast<int>(actions.size())) % static_cast<int>(actions.size());
    }
    if (m_inputManager->isActionJustPressed("Move Down")) {
        m_selectedKeyItem = (m_selectedKeyItem + 1) % static_cast<int>(actions.size());
    }
    if (m_inputManager->isActionJustPressed("Rotate") && !m_waitingForKey) {
        m_waitingAction = actions[m_selectedKeyItem];
        m_waitingForKey = true;
    }
    if (m_inputManager->isActionJustPressed("Menu")) {
        m_state = GameState::MENU;
        m_waitingForKey = false;
    }
}