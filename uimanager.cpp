#include "UIManager.h"
#include "Tetromino.h"
#include "Constants.h"
#include <cmath>
#include <algorithm>

UIManager::UIManager(sf::RenderWindow& window, const sf::Font& font)
    : m_window(window), m_font(font) {
    updateScale();
}

UIManager::~UIManager() = default;

// ========== 缩放核心函数 ==========

void UIManager::updateScale() {
    sf::Vector2u size = m_window.getSize();
    if (size.x == 0 || size.y == 0) return;

    float scaleX = static_cast<float>(size.x) / 800.0f;
    float scaleY = static_cast<float>(size.y) / 600.0f;
    m_scale = std::min(scaleX, scaleY);

    if (m_scale < 0.5f) m_scale = 0.5f;
    if (m_scale > 2.0f) m_scale = 2.0f;
}

int UIManager::getScaledCellSize() const {
    sf::Vector2u size = m_window.getSize();
    int maxHeight = static_cast<int>(static_cast<float>(size.y) * 0.7f);
    int cellSize = maxHeight / DEFAULT_GRID_HEIGHT;
    cellSize = static_cast<int>(static_cast<float>(cellSize) * m_scale);

    if (cellSize < 15) cellSize = 15;
    if (cellSize > 50) cellSize = 50;
    return cellSize;
}

int UIManager::getFontSize(int base) const {
    int size = static_cast<int>(static_cast<float>(base) * m_scale);
    if (size < 8) size = 8;
    if (size > 48) size = 48;
    return size;
}

sf::Vector2f UIManager::getPos(float xRatio, float yRatio) const {
    sf::Vector2u size = m_window.getSize();
    return sf::Vector2f(
        static_cast<float>(size.x) * xRatio,
        static_cast<float>(size.y) * yRatio
    );
}

// ========== 基础绘制函数 ==========

void UIManager::drawGradientBackground() {
    sf::Vector2u size = m_window.getSize();
    int step = (size.y > 1080) ? 2 : 1;

    for (unsigned int i = 0; i <= size.y; i += step) {
        float ratio = static_cast<float>(i) / static_cast<float>(size.y);
        int r = static_cast<int>(20 + ratio * 30);
        int g = static_cast<int>(10 + ratio * 20);
        int b = static_cast<int>(40 + ratio * 30);

        r = std::max(0, std::min(255, r));
        g = std::max(0, std::min(255, g));
        b = std::max(0, std::min(255, b));

        sf::RectangleShape rect(sf::Vector2f(static_cast<float>(size.x), 1.0f));
        rect.setPosition(sf::Vector2f(0.0f, static_cast<float>(i)));
        rect.setFillColor(sf::Color(static_cast<std::uint8_t>(r),
            static_cast<std::uint8_t>(g),
            static_cast<std::uint8_t>(b)));
        m_window.draw(rect);
    }
}

void UIManager::drawTitle(const std::string& text, float yRatio) {
    sf::Vector2u size = m_window.getSize();
    sf::Text title(m_font, text, getFontSize(48));
    title.setFillColor(UI_TITLE_COLOR);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect bounds = title.getLocalBounds();
    title.setPosition(sf::Vector2f(
        static_cast<float>(size.x) / 2.0f - bounds.size.x / 2.0f,
        static_cast<float>(size.y) * yRatio - bounds.size.y / 2.0f
    ));
    m_window.draw(title);
}

void UIManager::drawTextCentered(const std::string& text, float yRatio, int fontSizeBase, const sf::Color& color) {
    sf::Vector2u size = m_window.getSize();
    sf::Text sfText(m_font, text, getFontSize(fontSizeBase));
    sfText.setFillColor(color);
    sf::FloatRect bounds = sfText.getLocalBounds();
    sfText.setPosition(sf::Vector2f(
        static_cast<float>(size.x) / 2.0f - bounds.size.x / 2.0f,
        static_cast<float>(size.y) * yRatio - bounds.size.y / 2.0f
    ));
    m_window.draw(sfText);
}

void UIManager::drawButton(const std::string& text, float xRatio, float yRatio, bool selected) {
    sf::Vector2u size = m_window.getSize();
    int fontSize = getFontSize(26);
    sf::Text button(m_font, text, fontSize);
    button.setFillColor(selected ? UI_TITLE_COLOR : UI_TEXT_COLOR);
    if (selected) button.setStyle(sf::Text::Bold);

    sf::FloatRect bounds = button.getLocalBounds();
    float x = static_cast<float>(size.x) * xRatio - bounds.size.x / 2.0f;
    float y = static_cast<float>(size.y) * yRatio - bounds.size.y / 2.0f;
    button.setPosition(sf::Vector2f(x, y));

    if (selected) {
        sf::RectangleShape bg(sf::Vector2f(bounds.size.x + 40, bounds.size.y + 20));
        bg.setFillColor(UI_BUTTON_COLOR);
        bg.setPosition(sf::Vector2f(x - 20, y - 10));
        bg.setOutlineColor(UI_TITLE_COLOR);
        bg.setOutlineThickness(2.0f);
        m_window.draw(bg);
    }
    m_window.draw(button);
}

void UIManager::drawBackButton() {
    sf::Vector2u size = m_window.getSize();
    float btnW = getScaledW(130.0f);
    float btnH = getScaledH(40.0f);
    float btnX = static_cast<float>(size.x) - btnW - getScaledW(20.0f);
    float btnY = static_cast<float>(size.y) - btnH - getScaledH(20.0f);

    sf::RectangleShape bg(sf::Vector2f(btnW, btnH));
    bg.setFillColor(m_backButtonHovered ? UI_BUTTON_HOVER_COLOR : UI_BUTTON_COLOR);
    bg.setOutlineColor(m_backButtonHovered ? UI_TITLE_COLOR : sf::Color(100, 100, 100));
    bg.setOutlineThickness(2.0f);
    bg.setPosition(sf::Vector2f(btnX, btnY));
    m_window.draw(bg);

    sf::Text text(m_font, "Back", getFontSize(18));
    text.setFillColor(UI_TEXT_COLOR);
    sf::FloatRect tb = text.getLocalBounds();
    text.setPosition(sf::Vector2f(
        btnX + btnW / 2.0f - tb.size.x / 2.0f,
        btnY + btnH / 2.0f - tb.size.y / 2.0f
    ));
    m_window.draw(text);
}

void UIManager::drawControlsPanel(float x, float y, float width, float height,
    const std::vector<std::pair<std::string, std::string>>& controls) {
    sf::RectangleShape panel(sf::Vector2f(width, height));
    panel.setFillColor(sf::Color(0, 0, 0, 180));
    panel.setOutlineColor(UI_HINT_COLOR);
    panel.setOutlineThickness(1.0f);
    panel.setPosition(sf::Vector2f(x, y));
    m_window.draw(panel);

    sf::Text title(m_font, "CONTROLS", getFontSize(14));
    title.setFillColor(UI_TITLE_COLOR);
    title.setPosition(sf::Vector2f(x + 10.0f, y + 5.0f));
    m_window.draw(title);

    float sepY = y + static_cast<float>(getFontSize(18));
    sf::RectangleShape sep(sf::Vector2f(width - 20.0f, 1.0f));
    sep.setFillColor(sf::Color(100, 100, 100));
    sep.setPosition(sf::Vector2f(x + 10.0f, sepY));
    m_window.draw(sep);

    float yOff = y + static_cast<float>(getFontSize(22));
    float keyX = x + 15.0f;
    float actionX = x + width * 0.45f;
    int ctrlFont = getFontSize(11);

    for (const auto& c : controls) {
        sf::Text key(m_font, c.first, ctrlFont);
        key.setFillColor(UI_HINT_COLOR);
        key.setPosition(sf::Vector2f(keyX, yOff));
        m_window.draw(key);

        sf::Text act(m_font, c.second, ctrlFont);
        act.setFillColor(sf::Color(200, 200, 200));
        act.setPosition(sf::Vector2f(actionX, yOff));
        m_window.draw(act);

        yOff += static_cast<float>(getFontSize(18));
    }
}