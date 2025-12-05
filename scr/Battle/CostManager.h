#pragma once

class CostManager {

	// 現在のコスト
	int m_currentCost;
	// 最大コスト
	const int m_maxCost = 10;

public:

	CostManager() : m_currentCost(0) {}

	// 初期化
	void Init() {
		m_currentCost = 0;
	}

	// ターン開始時にコストを設定
	void StartTurn(int turn) {
		m_currentCost = std::min(turn, m_maxCost);
	}

	// コスト使用
	bool UseCost(int cost) {
		if (m_currentCost >= cost) {
			m_currentCost -= cost;
			return true;
		}
		return false;
	}

	// 現在のコスト取得
	int GetCurrentCost() const {
		return m_currentCost;
	}
};