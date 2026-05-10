#include "UIManager.h"
#include "Constants.h"

void UIManager::drawMainMenu(float titleBobOffset, int selectedItem) {
    sf::Vector2u size = m_window.getSize();
    drawGradientBackground();

    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    m_window.draw(overlay);

    float titleY = 0.28f + titleBobOffset / static_cast<float>(size.y);
    drawTitle("TETRIS", titleY);

    sf::Text subtitle(m_font, "Classic Puzzle Game", getFontSize(16));
    subtitle.setFillColor(sf::Color(200, 200, 200));
    sf::FloatRect subBounds = subtitle.getLocalBounds();
    subtitle.setPosition(sf::Vector2f(
        static_cast<float>(size.x) / 2.0f - subBounds.size.x / 2.0f,
        static_cast<float>(size.y) * 0.32f + titleBobOffset * 0.5f
    ));
    m_window.draw(subtitle);

    float startY = 0.48f;
    float buttonSpacing = 0.09f;
    for (size_t i = 0; i < m_menuItems.size(); ++i) {
        bool isSelected = (selectedItem == static_cast<int>(i)) || (m_hoveredMenuItem == static_cast<int>(i));
        drawButton(m_menuItems[i], 0.5f, startY + static_cast<float>(i) * buttonSpacing, isSelected);
    }

    float pw = static_cast<float>(getFontSize(220));
    float ph = static_cast<float>(getFontSize(200));
    float px = static_cast<float>(size.x) - pw - getScaledW(20.0f);
    float py = static_cast<float>(size.y) - ph - getScaledH(20.0f);

    std::vector<std::pair<std::string, std::string>> controls = {
        {"Up / Down", "Navigate"}, {"Enter", "Select"},
        {"Mouse", "Click"}, {"ESC", "Exit"}
    };
    drawControlsPanel(px, py, pw, ph, controls);
}

void UIManager::drawSettingsMenu(int selectedItem, const GameSettings& settings) {
    sf::Vector2u size = m_window.getSize();
    drawGradientBackground();
    drawTitle("Settings", 0.12f);

    float startY = 0.28f;
    float labelX = 0.25f;
    float valueX = 0.65f;

    for (size_t i = 0; i < m_settingsItems.size(); ++i) {
        float y = startY + static_cast<float>(i) * 0.1f;

        sf::Text label(m_font, m_settingsItems[i], getFontSize(20));
        label.setFillColor(selectedItem == static_cast<int>(i) ? UI_TITLE_COLOR : UI_TEXT_COLOR);
        label.setPosition(getPos(labelX, y));
        m_window.draw(label);

        std::string value;
        if (i == 0) {
            value = std::to_string(settings.gridWidth) + "x" + std::to_string(settings.gridHeight);
        }
        else if (i == 1) {
            value = std::to_string(settings.initialFallSpeed);
        }
        else if (i == 2) {
            value = "x" + std::to_string(settings.scoreMultiplier);
        }
        else {
            value = settings.showGhostPiece ? "ON" : "OFF";
        }

        sf::Text valText(m_font, value, getFontSize(18));
        valText.setFillColor(UI_HINT_COLOR);
        valText.setPosition(getPos(valueX, y));
        m_window.draw(valText);

        if (selectedItem == static_cast<int>(i)) {
            float highW = static_cast<float>(getFontSize(350));
            float highH = static_cast<float>(getFontSize(35));
            sf::RectangleShape highlight(sf::Vector2f(highW, highH));
            highlight.setFillColor(sf::Color(80, 80, 120, 100));
            highlight.setPosition(getPos(0.22f, y - 0.03f));
            m_window.draw(highlight);
        }
    }

    drawBackButton();

    float pw = static_cast<float>(getFontSize(220));
    float ph = static_cast<float>(getFontSize(150));
    float px = static_cast<float>(size.x) - pw - getScaledW(20.0f);
    float py = static_cast<float>(size.y) - ph - getScaledH(20.0f);

    std::vector<std::pair<std::string, std::string>> controls = {
        {"Up / Down", "Navigate"}, {"Left / Right", "Adjust"},
        {"Mouse", "Click"}, {"ESC", "Back"}
    };
    drawControlsPanel(px, py, pw, ph, controls);
}

void UIManager::drawMapSettingsMenu(int selectedItem, int width, int height, int presetId) {
    sf::Vector2u size = m_window.getSize();
    drawGradientBackground();
    drawTitle("Map Settings", 0.12f);

    int safeId = presetId % static_cast<int>(m_presets.size());
    std::vector<std::string> items = {
        "Width: " + std::to_string(width),
        "Height: " + std::to_string(height),
        "Preset: " + m_presets[safeId],
        "Save Map", "Load Map", "Reset Map"
    };

    float startY = 0.28f;
    float labelX = 0.35f;

    for (size_t i = 0; i < items.size(); ++i) {
        float y = startY + static_cast<float>(i) * 0.09f;
        sf::Text item(m_font, items[i], getFontSize(18));
        item.setFillColor(selectedItem == static_cast<int>(i) ? UI_TITLE_COLOR : UI_TEXT_COLOR);
        item.setPosition(getPos(labelX, y));
        m_window.draw(item);

        if (selectedItem == static_cast<int>(i)) {
            float highW = static_cast<float>(getFontSize(280));
            float highH = static_cast<float>(getFontSize(32));
            sf::RectangleShape highlight(sf::Vector2f(highW, highH));
            highlight.setFillColor(sf::Color(80, 80, 120, 100));
            highlight.setPosition(getPos(0.32f, y - 0.03f));
            m_window.draw(highlight);
        }
    }

    drawBackButton();

    float pw = static_cast<float>(getFontSize(220));
    float ph = static_cast<float>(getFontSize(150));
    float px = static_cast<float>(size.x) - pw - getScaledW(20.0f);
    float py = static_cast<float>(size.y) - ph - getScaledH(20.0f);

    std::vector<std::pair<std::string, std::string>> controls = {
        {"Up / Down", "Navigate"}, {"Left / Right", "Change"},
        {"Enter", "Execute"}, {"ESC", "Back"}
    };
    drawControlsPanel(px, py, pw, ph, controls);
}

void UIManager::drawKeySettingsMenu(int selectedItem,
    const std::map<std::string, sf::Keyboard::Key>& bindings,
    const std::string& waitingForAction) {
    sf::Vector2u size = m_window.getSize();
    drawGradientBackground();
    drawTitle("Key Settings", 0.12f);

    float startY = 0.28f;
    int i = 0;
    for (const auto& binding : bindings) {
        float y = startY + static_cast<float>(i) * 0.08f;
        sf::Text item(m_font, binding.first + ": " + getKeyName(binding.second), getFontSize(16));
        item.setFillColor(selectedItem == i ? UI_TITLE_COLOR : UI_TEXT_COLOR);
        item.setPosition(getPos(0.3f, y));
        m_window.draw(item);

        if (selectedItem == i) {
            float highW = static_cast<float>(getFontSize(300));
            float highH = static_cast<float>(getFontSize(30));
            sf::RectangleShape highlight(sf::Vector2f(highW, highH));
            highlight.setFillColor(sf::Color(80, 80, 120, 100));
            highlight.setPosition(getPos(0.27f, y - 0.03f));
            m_window.draw(highlight);
        }
        ++i;
    }

    if (!waitingForAction.empty()) {
        drawTextCentered("Press a key for: " + waitingForAction, 0.85f, 16, UI_ERROR_COLOR);
    }

    drawBackButton();

    float pw = static_cast<float>(getFontSize(220));
    float ph = static_cast<float>(getFontSize(130));
    float px = static_cast<float>(size.x) - pw - getScaledW(20.0f);
    float py = static_cast<float>(size.y) - ph - getScaledH(20.0f);

    std::vector<std::pair<std::string, std::string>> controls = {
        {"Up / Down", "Navigate"}, {"Enter", "Change"},
        {"ESC", "Cancel"}, {"Mouse", "Click"}
    };
    drawControlsPanel(px, py, pw, ph, controls);
}

void UIManager::drawGameOver(int score) {
    sf::Vector2u size = m_window.getSize();
    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    m_window.draw(overlay);

    drawTitle("GAME OVER", 0.35f);
    drawTextCentered("Final Score: " + std::to_string(score), 0.48f, 24, UI_TEXT_COLOR);

    std::vector<std::string> opts = { "Restart", "Main Menu", "Quit" };
    float startY = 0.58f;
    for (size_t i = 0; i < opts.size(); ++i) {
        bool sel = (m_hoveredGameOverItem == static_cast<int>(i));
        bool quit = (opts[i] == "Quit");
        sf::Text opt(m_font, opts[i], getFontSize(20));
        opt.setFillColor(quit ? (sel ? sf::Color(255, 100, 100) : sf::Color(200, 100, 100))
            : (sel ? UI_TITLE_COLOR : UI_TEXT_COLOR));
        if (sel) opt.setStyle(sf::Text::Bold);
        sf::FloatRect b = opt.getLocalBounds();
        float y = static_cast<float>(size.y) * (startY + static_cast<float>(i) * 0.09f);
        opt.setPosition(sf::Vector2f(
            static_cast<float>(size.x) / 2.0f - b.size.x / 2.0f,
            y - b.size.y / 2.0f
        ));
        m_window.draw(opt);
    }
}

void UIManager::drawPauseMenu() {
    sf::Vector2u size = m_window.getSize();
    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    m_window.draw(overlay);

    drawTitle("PAUSED", 0.32f);

    std::vector<std::string> opts = { "Resume", "Main Menu", "Quit" };
    float startY = 0.5f;
    for (size_t i = 0; i < opts.size(); ++i) {
        bool sel = (m_selectedPauseItem == static_cast<int>(i)) || (m_hoveredPauseItem == static_cast<int>(i));
        bool quit = (opts[i] == "Quit");
        sf::Text opt(m_font, opts[i], getFontSize(24));
        opt.setFillColor(quit ? (sel ? sf::Color(255, 100, 100) : sf::Color(200, 100, 100))
            : (sel ? UI_TITLE_COLOR : UI_TEXT_COLOR));
        if (sel) opt.setStyle(sf::Text::Bold);
        sf::FloatRect b = opt.getLocalBounds();
        float y = static_cast<float>(size.y) * (startY + static_cast<float>(i) * 0.1f);
        opt.setPosition(sf::Vector2f(
            static_cast<float>(size.x) / 2.0f - b.size.x / 2.0f,
            y - b.size.y / 2.0f
        ));
        if (sel && m_hoveredPauseItem == static_cast<int>(i)) {
            sf::RectangleShape hl(sf::Vector2f(b.size.x + 40.0f, b.size.y + 15.0f));
            hl.setFillColor(sf::Color(80, 80, 120, 150));
            hl.setPosition(sf::Vector2f(opt.getPosition().x - 20.0f, opt.getPosition().y - 8.0f));
            m_window.draw(hl);
        }
        m_window.draw(opt);
    }
}