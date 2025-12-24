#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// 前方宣言
class RenderSystem;

class Animation
{
	// フレーム群
	std::vector<sf::Texture> m_frames;
	// スプライト
	sf::Sprite m_sprite;

	// アニメーション制御用
	float m_frameTiem = 0.1f;
	// 経過時間
	float m_timer = 0.0f;
	// 現在のフレームインデックス
	size_t m_index = 0;

public:

	// フレーム追加
	void AddFrame(const sf::Texture& arg_texture);
	// 位置設定
	void SetPosition(const sf::Vector2f& arg_pos);
	// 更新
	void Update(float arg_dt);
	// 描画
	void Draw(RenderSystem& render);
	// スプライト取得
	const sf::Sprite& GetSprite() const { return m_sprite; }

};