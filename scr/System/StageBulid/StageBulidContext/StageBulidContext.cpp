#include "StageBulidContext.h"
#include "CSVLoad/StageLoader/StageLoader.h"
#include "UI/TriangleButton.h"

void StageBulidContext::Init()
{
    m_stageIds = StageLoader::GetInstance().GetAllStageIds();

    if (!m_stageIds.empty())
    {
        m_selectedStageId = m_stageIds.front();
    }
    else
    {
        m_selectedStageId = -1;
    }
}

const std::vector<int>& StageBulidContext::GetStageIds() const
{
	return m_stageIds;
}

void StageBulidContext::SelectStage(int stageId)
{
	m_selectedStageId = stageId;
}

const StageData* StageBulidContext::GetSelectedStage() const
{
	return StageLoader::GetInstance().GetStageData(m_selectedStageId);
}

void StageBulidContext::SelectNextStage()
{
    auto it = std::find(m_stageIds.begin(), m_stageIds.end(), m_selectedStageId);

    if (it != m_stageIds.end()) {
        int nextIdx = (std::distance(m_stageIds.begin(), it) + 1) % m_stageIds.size();
        m_selectedStageId = m_stageIds[nextIdx];
    }
}

void StageBulidContext::SelectPrevStage()
{

    auto it = std::find(m_stageIds.begin(), m_stageIds.end(), m_selectedStageId);
    if (it != m_stageIds.end()) {
        int prevIdx = (std::distance(m_stageIds.begin(), it) - 1 + m_stageIds.size()) % m_stageIds.size();
        m_selectedStageId = m_stageIds[prevIdx];
    }

}

void StageBulidContext::SetNavigationButtons(std::unique_ptr<TriangleButton> left, std::unique_ptr<TriangleButton> right)
{
    m_leftArrow = std::move(left);
    m_rightArrow = std::move(right);
}

void StageBulidContext::SetSceneButton(std::unique_ptr<BoxButton> nextbutton, std::unique_ptr<BoxButton> backButton)
{
	m_nextButton = std::move(nextbutton);
	m_backButton = std::move(backButton);
}

TriangleButton* StageBulidContext::GetLeftArrow() const
{
    return m_leftArrow.get();
}

TriangleButton* StageBulidContext::GetRightArrow() const
{
    return m_rightArrow.get(); 
}

BoxButton* StageBulidContext::GetNextButton() const
{
	return m_nextButton.get();
}

BoxButton* StageBulidContext::GetBackButton() const
{
	return m_backButton.get();
}

