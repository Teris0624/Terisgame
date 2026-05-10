#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>

// ========== 游戏尺寸常量 ==========
const int DEFAULT_GRID_WIDTH = 10;
const int DEFAULT_GRID_HEIGHT = 20;
const int MIN_GRID_WIDTH = 6;
const int MAX_GRID_WIDTH = 16;
const int MIN_GRID_HEIGHT = 12;
const int MAX_GRID_HEIGHT = 24;
const int BASE_CELL_SIZE = 30;
const int BASE_SIDEBAR_WIDTH = 280;
const int BASE_WINDOW_WIDTH = DEFAULT_GRID_WIDTH * BASE_CELL_SIZE + BASE_SIDEBAR_WIDTH;
const int BASE_WINDOW_HEIGHT = DEFAULT_GRID_HEIGHT * BASE_CELL_SIZE;

// ========== 游戏状态枚举 ==========
enum class GameState { MENU, PLAYING, GAME_OVER, SETTINGS, MAP_SETTINGS, KEY_SETTINGS, PAUSED };

// ========== 方块形状 ==========
const int TETROMINOS[7][4][4] = {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
    {{0,0,0,0},{0,1,0,0},{1,1,1,0},{0,0,0,0}},
    {{0,0,0,0},{1,0,0,0},{1,1,1,0},{0,0,0,0}},
    {{0,0,0,0},{0,0,1,0},{1,1,1,0},{0,0,0,0}},
    {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
    {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}}
};

// ========== 颜色定义 ==========
const sf::Color COLORS[8] = {
    sf::Color(0,0,0,0), sf::Color(0,255,255), sf::Color(255,255,0),
    sf::Color(128,0,128), sf::Color(255,165,0), sf::Color(0,0,255),
    sf::Color(0,255,0), sf::Color(255,0,0)
};

// ========== UI颜色 ==========
const sf::Color UI_BG_COLOR = sf::Color(20, 20, 40);
const sf::Color UI_BUTTON_COLOR = sf::Color(60, 60, 100);
const sf::Color UI_BUTTON_HOVER_COLOR = sf::Color(80, 80, 120);
const sf::Color UI_TEXT_COLOR = sf::Color(255, 255, 255);
const sf::Color UI_TITLE_COLOR = sf::Color(255, 200, 50);
const sf::Color UI_ERROR_COLOR = sf::Color(255, 80, 80);
const sf::Color UI_HINT_COLOR = sf::Color(150, 200, 255);
const sf::Color UI_BUTTON_RED = sf::Color(180, 60, 60);
const sf::Color UI_BUTTON_RED_HOVER = sf::Color(220, 80, 80);

// ========== 按键名称获取 ==========
inline std::string getKeyName(sf::Keyboard::Key key) {
    switch (key) {
    case sf::Keyboard::Key::Space: return "Space";
    case sf::Keyboard::Key::Escape: return "Escape";
    case sf::Keyboard::Key::Enter: return "Enter";
    case sf::Keyboard::Key::Q: return "Q";
    case sf::Keyboard::Key::E: return "E";
    case sf::Keyboard::Key::S: return "S";
    case sf::Keyboard::Key::W: return "W";
    case sf::Keyboard::Key::P: return "P";
    case sf::Keyboard::Key::Up: return "Up";
    case sf::Keyboard::Key::Down: return "Down";
    case sf::Keyboard::Key::Left: return "Left";
    case sf::Keyboard::Key::Right: return "Right";
    default: return "Unknown";
    }
}

// ========== 默认按键绑定 ==========
inline std::map<std::string, sf::Keyboard::Key> getDefaultKeyBindings() {
    return {
        {"Move Left", sf::Keyboard::Key::Q},
        {"Move Right", sf::Keyboard::Key::E},
        {"Move Down", sf::Keyboard::Key::S},
        {"Rotate", sf::Keyboard::Key::W},
        {"Hard Drop", sf::Keyboard::Key::Space},
        {"Pause", sf::Keyboard::Key::P},
        {"Menu", sf::Keyboard::Key::Escape}
    };
}

// ========== 字体路径 ==========
inline std::vector<std::string> getFontPaths() {
    return {
        "arial.ttf",
        "C:\\Windows\\Fonts\\arial.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/System/Library/Fonts/Helvetica.ttc"
    };
}

#endif