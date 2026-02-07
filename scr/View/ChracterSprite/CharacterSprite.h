#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "View/Render/RenderSystem.h"
#include "CSVLoad/TextureLoader/TextureLoader.h"
#include "Entity/Character/CharacterData.h"

enum class CharacterAnimState
{
	WAIT,
	ATTACK,
	MAGIC,
	DAMAGE,
	DEAD,
	WIN
};

struct  AnimRect
{
	sf::IntRect rect;
};

class CharacterSprite
{
private:

	sf::Sprite m_sprite;
	std::string m_textureKey;

	CharacterAnimState m_currentState{ CharacterAnimState::WAIT };
	std::unordered_map<CharacterAnimState, sf::IntRect> m_rectTable;

	sf::Vector2i m_cellSize{0, 0};

	// HPバー
	sf::RectangleShape m_fill;   // 塗り
	sf::RectangleShape m_frame;  // 枠


public:

	// 初期化
	CharacterSprite(const std::string& arg_textureKey);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="arg_textureKey">テクスチャキー</param>
	void Init(const std::string& arg_textureKey);

	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="arg_pos">設定座標</param>
	void SetPosition(const sf::Vector2f& arg_pos);

	/// <summary>
	/// 画像の水平反転
	/// </summary>
	void SetSpriteWidthMirror();

	/// <summary>
	/// 画像の垂直
	/// </summary>
	void SetSpriteHeightMirror();

	/// <summary>
	/// アニメーションステート設定
	/// </summary>
	/// <param name="arg_state">設定アニメーションステート</param>
	void SetState(CharacterAnimState arg_state);
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="arg_render">レンダーシステム</param>
	void Draw(RenderSystem& arg_render, const CharacterData& arg_data,bool hpDrawFlag);



};

