#include "StageBulidController.h"
#include "System/InPutManager/InPutMouseManager.h"

void StageBulidController::Update(sf::RenderWindow& window)
{

	if (!InPutMouseManager::GetInstance().IsLeftClicked())
		return;

	auto mouse = sf::Mouse::getPosition(window);

	// 左側リストクリック判定
	int index = (mouse.y - 80) / 40;
	const auto& ids = m_context.GetStageIds();
	if (index >= 0 && index < ids.size())
	{
		m_context.SelectStage(ids[index]);
	}
}
