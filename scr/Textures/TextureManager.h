#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>

class TextureManager
{

	std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;

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

	// テクスチャの読み込み
	// return 成功：失敗
	bool TexturLoad(const std::string& id, const std::string& filePath)
	{

		// すでに取得済みなら何もしない
		if (textures.count(id) > 0)
		{
			return true;
		}

		//
		auto tex = std::make_unique<sf::Texture>();
		if (!tex->loadFromFile(filePath))
		{
			return false;
		}

		// 
		textures[id] = std::move(tex);
		return true;


	}


	// テクスチャを取得
	sf::Texture& GetTexture(const  std::string& id)
	{
		return *textures.at(id);

	}


	// 全削除
	void TextureClear()
	{
		textures.clear();
	}


};