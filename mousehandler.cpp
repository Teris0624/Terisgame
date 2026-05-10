#include "UIManager.h"
#include "Constants.h"

void UIManager::handleMouseMove(const sf::Vector2i& mousePos) {
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    sf::Vector2u size = m_window.getSize();

    // 主菜单按钮悬停检测
    m_hoveredMenuItem = -1;
    float startY = 0.48f;
    for (size_t i = 0; i < m_menuItems.size(); ++i) {
        float buttonY = static_cast<float>(size.y) * (startY + static_cast<float>(i) * 0.09f);
        int fs = getFontSize(26);
        sf::Text temp(m_font, m_menuItems[i], fs);
        sf::FloatRect bounds = temp.getLocalBounds();
        float buttonX = static_cast<float>(size.x) * 0.5f - bounds.size.x / 2.0f;
        sf::FloatRect rect(
            sf::Vector2f(buttonX, buttonY - bounds.size.y / 2.0f),
            sf::Vector2f(bounds.size.x, bounds.size.y)
        );
        if (rect.contains(mousePosF)) {
            m_hoveredMenuItem = static_cast<int>(i);
            break;
        }
    }

    // 暂停菜单按钮悬停检测
    m_hoveredPauseItem = -1;
    std::vector<std::string> pauseOpts = { "Resume", "Main Menu", "Quit" };
    for (size_t i = 0; i < pauseOpts.size(); ++i) {
        float buttonY = static_cast<float>(size.y) * (0.5f + static_cast<float>(i) * 0.1f);
        int fs = getFontSize(24);
        sf::Text temp(m_font, pauseOpts[i], fs);
        sf::FloatRect bounds = temp.getLocalBounds();
        float buttonX = static_cast<float>(size.x) * 0.5f - bounds.size.x / 2.0f;
        sf::FloatRect rect(
            sf::Vector2f(buttonX, buttonY - bounds.size.y / 2.0f),
            sf::Vector2f(bounds.size.x, bounds.size.y)
        );
        if (rect.contains(mousePosF)) {
            m_hoveredPauseItem = static_cast<int>(i);
            break;
        }
    }

    // 游戏结束菜单按钮悬停检测
    m_hoveredGameOverItem = -1;
    std::vector<std::string> gameOverOpts = { "Restart", "Main Menu", "Quit" };
    for (size_t i = 0; i < gameOverOpts.size(); ++i) {
        float buttonY = static_cast<float>(size.y) * (0.58f + static_cast<float>(i) * 0.09f);
        int fs = getFontSize(20);
        sf::Text temp(m_font, gameOverOpts[i], fs);
        sf::FloatRect bounds = temp.getLocalBounds();
        float buttonX = static_cast<float>(size.x) * 0.5f - bounds.size.x / 2.0f;
        sf::FloatRect rect(
            sf::Vector2f(buttonX, buttonY - bounds.size.y / 2.0f),
            sf::Vector2f(bounds.size.x, bounds.size.y)
        );
        if (rect.contains(mousePosF)) {
            m_hoveredGameOverItem = static_cast<int>(i);
            break;
        }
    }

    // 返回按钮悬停检测
    m_backButtonHovered = false;
    float btnW = getScaledW(130.0f);
    float btnH = getScaledH(40.0f);
    float btnX = static_cast<float>(size.x) - btnW - getScaledW(20.0f);
    float btnY = static_cast<float>(size.y) - btnH - getScaledH(20.0f);
    sf::FloatRect backBounds(
        sf::Vector2f(btnX, btnY),
        sf::Vector2f(btnW, btnH)
    );
    if (backBounds.contains(mousePosF)) {
        m_backButtonHovered = true;
    }
}