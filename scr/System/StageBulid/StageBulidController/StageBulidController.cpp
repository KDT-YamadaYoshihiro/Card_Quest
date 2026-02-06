#include "StageBulidController.h"
#include "System/InPutManager/InPutMouseManager.h"

void StageBulidController::Update(sf::RenderWindow& window)
{
    auto& mouse = InPutMouseManager::GetInstance();
    if (!mouse.IsLeftClicked()) return;

    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

    // Contextから共有ポインタを介して判定
    auto* leftBtn = m_context.GetLeftArrow();
    auto* rightBtn = m_context.GetRightArrow();

    if (leftBtn && leftBtn->IsClicked(mousePos, true)) {
        m_context.SelectPrevStage();
    }
    if (rightBtn && rightBtn->IsClicked(mousePos, true)) {
        m_context.SelectNextStage();
    }

}
