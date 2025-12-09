#pragma once

class CostManager
{
    
    // コスト
    int m_cost = 0;
    // 最大コスト
    int m_maxCost = 5;

public:

    // 初期化
    void Init(int maxCost) {
        m_maxCost = maxCost;
        m_cost = maxCost;
    }

    // リセット
    void ResetCost()
    {
        m_cost = m_maxCost;
    }

    // 使用
    bool Use(int cost) {

        // コストチェック
        if (m_cost < cost) {
            // コスト不足
            return false;
        }
        // コスト消費
        m_cost -= cost;
        return true;
    }

    // 追加
    void Refill(int amount) {
        m_cost += amount;
        if (m_cost > m_maxCost) m_cost = m_maxCost;
    }
    bool CanUse(int cost) const
    {
        return m_cost >= cost;
    }
    // 現在のコストを取得
    int GetCost() const { return m_cost; }
};