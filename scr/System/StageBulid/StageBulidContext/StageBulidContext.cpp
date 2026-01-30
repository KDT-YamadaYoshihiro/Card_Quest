#include "StageBulidContext.h"
#include "CSVLoad/StageLoader/StageLoader.h"

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

