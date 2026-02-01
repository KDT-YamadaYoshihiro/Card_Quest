#pragma once
#include "System/Singleton/Singleton.h"

class CostManager : public Singleton<CostManager>
{
private:

    friend class Singleton<CostManager>;

    int m_currentCost = 0;
    int m_maxCost = 1;

public:
    // 初期化
    void Init(int maxCost)
    {
        m_maxCost = maxCost;
        m_currentCost = maxCost;
    }

    // ターン開始時
    void ResetCost()
    {
        m_currentCost = m_maxCost;
    }

    // 判定
    bool CanConsume(int cost) const
    {
        return m_currentCost >= cost;
    }

    bool IsEmpty() const
    {
        return m_currentCost <= 0;
    }

    // 消費
    bool Consume(int cost)
    {
        if (!CanConsume(cost))
            return false;

        m_currentCost -= cost;
        return true;
    }

    // カード効果用
    void AddCost(int amount)
    {
        m_currentCost += amount;
        if (m_currentCost > m_maxCost)
        {
            m_currentCost = m_maxCost;
        }
    }

	// 最大コスト増加
    void IncreaseMaxCost(int amount)
    {
        m_maxCost += amount;
        m_currentCost += amount;
    }

    // 取得
    int GetCurrentCost() const { return m_currentCost; }
    int GetMaxCost() const { return m_maxCost; }
};
