#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../CameraManager/CameraManager.h"

class RenderSystem
{

	sf::RenderWindow& m_window;

public:

	RenderSystem(sf::RenderWindow& arg_window)
		:m_window(arg_window)
	{
	}

	// 描画関連
	// カメラ適用
	void ApplyCamera();
	// カメラリセット
	void ResetCamera();
	// 描画
	void Draw(const sf::Drawable& drawable);
	// 描画（位置・回転）
	void Draw(const sf::Drawable& drawable, const sf::RenderStates& states);
	// 画面クリア
	void Clear(const sf::Color& color = sf::Color::Black);
	// 画面表示
	void Display();

	// Windowへのアクセス
	sf::RenderWindow& GetWindow() { return m_window; }
};