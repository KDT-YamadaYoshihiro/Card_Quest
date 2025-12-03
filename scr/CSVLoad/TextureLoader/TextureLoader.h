#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include "../CSVLoad/CSVLoader.h"

class TextureLoader : public CSVLoader
{

	std::unordered_map<std::string, sf::Texture> m_textures;

	TextureLoader() = default;
	virtual ~TextureLoader() = default;

public:


	// コピー、代入禁止
	TextureLoader(const TextureLoader&) = delete;
	TextureLoader& operator = (const TextureLoader) = delete;
	static TextureLoader& Instance()
	{
		static TextureLoader instance;
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