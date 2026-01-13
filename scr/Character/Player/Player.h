#pragma once
#include "../Character.h"
#include "../../View/Render/Animetion/Animation.h"

class Player : public Character {


public:

	// コンストラクタ
	Player(const CharacterData& arg_data) :Character(arg_data)
	{
		m_faction = Faction::Player;
	}

	// 初期化
	void Init(const std::vector<sf::Texture>& arg_textures, const sf::Vector2f& arg_startPos);
	// 状態更新
	void Update() override;
	// 描画
	void Render(RenderSystem& render) override;
	// アクション
	void Action() override;
	// 当たり判定取得
	sf::FloatRect GetHitCircle() const override;

};