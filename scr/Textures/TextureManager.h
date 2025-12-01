#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include "../CSVLoad/CSVLoader.h"

class TextureManager : public CSVLoader
{

	std::unordered_map<std::string, sf::Texture> m_textures;

	TextureManager() = default;
	virtual ~TextureManager() = default;

public:


	// コピー、代入禁止
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator = (const TextureManager) = delete;
	static TextureManager& Instance()
	{
		static TextureManager instance;
		return instance;
	}

	// CSV読込
	bool Load(const std::string& path);

	// テクスチャ取得
	const sf::Texture* Get(const std::string& id) const
	{
		auto it = m_textures.find(id);
		return (it != m_textures.end()) ? &it->second : nullptr;
	}

	// 全削除
	void TextureClear()
	{
		m_textures.clear();
	}


};