#pragma once
#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include "Constants.h"
#include "SettingsManager.h"

class Tetromino;

class UIManager {
public:
    UIManager(sf::RenderWindow& window, const sf::Font& font);
    ~UIManager();

    // ========== 缩放相关 ==========
    void updateScale();
    int getScaledCellSize() const;
    int getFontSize(int base) const;
    float getScale() const { return m_scale; }
    sf::Vector2f getPos(float xRatio, float yRatio) const;
    float getScaledX(float x) const { return x * m_scale; }
    float getScaledY(float y) const { return y * m_scale; }
    float getScaledW(float w) const { return w * m_scale; }
    float getScaledH(float h) const { return h * m_scale; }

    // ========== 鼠标交互 ==========
    void handleMouseMove(const sf::Vector2i& mousePos);
    int getHoveredMenuItem() const { return m_hoveredMenuItem; }
    int getHoveredPauseItem() const { return m_hoveredPauseItem; }
    int getHoveredGameOverItem() const { return m_hoveredGameOverItem; }
    bool isBackButtonHovered() const { return m_backButtonHovered; }

    void setSelectedPauseItem(int item) { m_selectedPauseItem = item; }
    int getSelectedPauseItem() const { return m_selectedPauseItem; }

    // ========== 基础绘制函数 ==========
    void drawGradientBackground();
    void drawTitle(const std::string& text, float yRatio);
    void drawTextCentered(const std::string& text, float yRatio, int fontSizeBase, const sf::Color& color);
    void drawButton(const std::string& text, float xRatio, float yRatio, bool selected);
    void drawBackButton();
    void drawControlsPanel(float x, float y, float width, float height,
        const std::vector<std::pair<std::string, std::string>>& controls);

    // ========== 菜单绘制 ==========
    void drawMainMenu(float titleBobOffset, int selectedItem);
    void drawSettingsMenu(int selectedItem, const GameSettings& settings);
    void drawMapSettingsMenu(int selectedItem, int width, int height, int presetId);
    void drawKeySettingsMenu(int selectedItem,
        const std::map<std::string, sf::Keyboard::Key>& bindings,
        const std::string& waitingForAction);
    void drawGameOver(int score);
    void drawPauseMenu();

    // ========== 游戏界面绘制 ==========
    void drawGameUI(int score, int level, int lines, int currentWidth,
        const sf::Color& nextColor, const int nextShape[4][4]);
    void drawGameArea(int grid[20][16], int width, int height,
        const Tetromino* piece, bool showGhost);

private:
    sf::RenderWindow& m_window;
    const sf::Font& m_font;
    float m_scale{ 1.0f };
    int m_selectedPauseItem{ 0 };

    int m_hoveredMenuItem{ -1 };
    int m_hoveredPauseItem{ -1 };
    int m_hoveredGameOverItem{ -1 };
    bool m_backButtonHovered{ false };

    std::vector<std::string> m_menuItems = { "Start Game", "Settings", "Map Settings", "Key Settings", "Quit" };
    std::vector<std::string> m_settingsItems = { "Grid Size", "Fall Speed", "Score Multiplier", "Ghost Piece" };
    std::vector<std::string> m_presets = { "Classic", "Walls", "Pit", "Random", "Maze" };
};

#endif