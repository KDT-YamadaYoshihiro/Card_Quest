#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include "CSVLoad/CSVLoader.h"

class TextureLoader : public CSVLoader,public Singleton<TextureLoader>
{
	friend class Singleton<TextureLoader>;

	std::unordered_map<std::string, sf::Texture> m_textures;

public:

	// CSV読込
	bool LoadTextures(const std::string& path);

	// テクスチャ取得
	const sf::Texture* GetTextureID(const std::string& id) const
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