#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "../../View/Render/RenderSystem.h"
#include "../../CSVLoad/TextureLoader/TextureLoader.h"

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
	/// アニメーションステート設定
	/// </summary>
	/// <param name="arg_state">設定アニメーションステート</param>
	void SetState(CharacterAnimState arg_state);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="arg_render">レンダーシステム</param>
	void Draw(RenderSystem& arg_render);

private:

	sf::Sprite m_sprite;
	std::string m_textureKey;

	CharacterAnimState m_currentState{ CharacterAnimState::WAIT };
	std::unordered_map<CharacterAnimState, sf::IntRect> m_rectTable;


};

