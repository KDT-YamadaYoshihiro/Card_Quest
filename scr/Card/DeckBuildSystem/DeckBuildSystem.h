#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "../../Card/Card.h"
#include "../../Card/CardRenderer/CardRenderer.h"


class DeckBuildSystem
{
	// 表示用カード構造体
	struct DisplayCard
	{
		std::unique_ptr<Card> card;
		int count;
	};

	// デッキ編成上限枚数
	static const int MAX_DECK_SIZE = 30;
	// デッキ内カード
	std::vector<std::unique_ptr<Card>> m_deckCards;
	// プール内表示用カード
	std::vector<DisplayCard> m_displayPool;
	// カード描画
	std::shared_ptr<CardRenderer> m_renderer;
	// 描画位置
	sf::Vector2f m_deckStartPos;
	sf::Vector2f m_poolStartPos;
	// クリック開始位置
	sf::Vector2f m_mouseDownPos;
	// カード間隔
	float m_cardSpacing;
	// スクロール関連
	// プール座標
	float m_poolScrollX;
	bool  m_poolDragging;
	float m_poolDragStartX;
	// デッキ座標
	float m_deckScrollX;
	bool  m_deckDragging;
	float m_deckDragStartX;

	// 
	float m_lastMouseX;

	// ドラッグ状態管理
	bool  m_prevClick = false;
	bool m_dragStarted = false;

public:

	DeckBuildSystem()
		:m_renderer(std::make_shared<CardRenderer>()),
		m_deckStartPos({ 50.f, 50.f }),
		m_poolStartPos({ 50.f, 300.f }),
		m_mouseDownPos({ 0.f, 0.f }),
		m_cardSpacing(130.f),
		m_poolScrollX(0.0f),
		m_poolDragging(false),
		m_poolDragStartX(0.0f),
		m_deckScrollX(0.0f),
		m_deckDragging(false),
		m_deckDragStartX(0.0f),
		m_lastMouseX(0.0f)
	{
		RebuildDisplayPool();
	}

	// 初期化
	void Init();

	// 更新
	void Update(sf::Vector2f mousePos, bool isClick,bool isDragging, bool released,float weelDelta);

	// 描画
	void Draw(sf::RenderWindow& window, const sf::Font& font);


	// プールからデッキに追加
	bool AddFromPool(int poolIndex);

	// デッキからプールに戻す
	bool RemoveFromPool(int deckIndex);

	// デッキ内カード取得
	const std::vector<std::unique_ptr<Card>>& GetDeckCards() const;

	// デッキ内枚数取得
	int GetDeckSize() const;

	// デッキ編成上限枚数取得
	int GetMaxDeckSize() const;

	// デッキ編成完了判定
	bool IsComplete() const;

	std::vector<int> TakeDeck();

private:
	
	// プールクリック処理
	bool HandlePoolClick(sf::Vector2f mousePos);
	// デッキクリック処理
	bool HandleDeckClick(sf::Vector2f mousePos);
	// ドラッグ処理
	void HandleDrag(sf::Vector2f mousePos, bool isClick);
	// プール再構築
	void RebuildDisplayPool();
	// スクロール制限
	void ClampPoolScroll();
	void ClampDeckScroll();
};

