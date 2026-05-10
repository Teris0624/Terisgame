#pragma once
#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <string>
#include "Constants.h"

struct GameSettings {
    int gridWidth{ 10 };
    int gridHeight{ 20 };
    float initialFallSpeed{ 0.5f };
    int scoreMultiplier{ 1 };
    bool soundEnabled{ true };
    bool showGhostPiece{ true };
};

class SettingsManager {
public:
    SettingsManager();
    ~SettingsManager();

    GameSettings getSettings() const;
    void setSettings(const GameSettings& settings);
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    void resetToDefaults();

private:
    GameSettings m_settings;
};

#endif