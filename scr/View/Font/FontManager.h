#pragma once
#include <SFML/Graphics.hpp>

class FontManager : public Singleton<FontManager>
{

	friend class Singleton<FontManager>;

	sf::Font m_font;

	// デフォルトコンストラクタ
	FontManager() = default;
	virtual ~FontManager() = default;

public:

	// シングルトン コピー禁止
	FontManager(const FontManager&) = delete;
	FontManager& operator = (const FontManager&) = delete;

	// インスタンスの取得
	static FontManager& GetInstance()
	{
		static FontManager instance;
		return instance;
	}

	// フォントの取得
	const sf::Font& GetFont() const { return m_font; }

	// フォントの読み込み
	void FontLoad()
	{
		if (!m_font.openFromFile("data/Font/SoukouMincho.ttf"))
		{
			// エラー処理（assert / ログなど）
			throw std::runtime_error("Font load failed");
		}
	}

};