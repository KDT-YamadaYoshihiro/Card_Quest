#pragma once
#include <SFML/Graphics.hpp>
class TriangleButton
{
public:

	/// <summary>
	/// 方向列挙型
	/// </summary>
	enum class Direction
	{
		Left,
		Right,
		Up,
		Down
	};

private:

	// 三角形の形状
	sf::ConvexShape m_shape;
	// 方向
	Direction dir;
	// サイズ
	sf::Vector2f size;
	// 座標
	sf::Vector2f pos;

public:

	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="arg_bottom">底辺</param>
	/// <param name="arg_height">高さ</param>
	/// <param name="arg_pos">座標</param>
	TriangleButton(sf::Vector2f arg_size, sf::Vector2f arg_pos, Direction arg_dir)
		: dir(arg_dir), 
		size(arg_size), 
		pos(arg_pos)
	{
		// 1. 三角形本体の設定
		m_shape.setPointCount(3);

		// 向きに合わせて頂点を設定（中心を 0,0 とした相対座標）
		float hw = arg_size.x / 2.0f;
		float hh = arg_size.y / 2.0f;

		switch (arg_dir)
		{
		case Direction::Left:
			m_shape.setPoint(0, { -hw, 0 });  // 頂点
			m_shape.setPoint(1, { hw, -hh }); // 右上
			m_shape.setPoint(2, { hw, hh });  // 右下
			break;
		case Direction::Right:
			m_shape.setPoint(0, { hw, 0 });   // 頂点
			m_shape.setPoint(1, { -hw, -hh });// 左上
			m_shape.setPoint(2, { -hw, hh }); // 左下
			break;
		case Direction::Up:
			m_shape.setPoint(0, { 0, -hh });  // 頂点
			m_shape.setPoint(1, { -hw, hh }); // 左下
			m_shape.setPoint(2, { hw, hh });  // 右下
			break;
		case Direction::Down:
			m_shape.setPoint(0, { 0, hh });   // 頂点
			m_shape.setPoint(1, { -hw, -hh });// 左上
			m_shape.setPoint(2, { hw, -hh }); // 右上
			break;
		}

		m_shape.setPosition(arg_pos);
		m_shape.setFillColor(sf::Color(200, 200, 200));

	}


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="render"></param>
	/// <param name="position"></param>
	/// <param name="size"></param>
	/// <param name="direction"></param>
	/// <param name="color"></param>
	void Draw(sf::RenderWindow& arg_window) const
	{
		arg_window.draw(m_shape);
	}


	/// <summary>
	/// ホバーしているか
	/// </summary>
	/// <param name="mousePos">マウス座標</param>
	/// <returns></returns>
	bool IsHovered(sf::Vector2f arg_mousePos) const
	{
		return m_shape.getGlobalBounds().contains(arg_mousePos);
	}

	/// <summary>
	/// クリックされたか
	/// </summary>
	/// <param name="mousePos">マウス座標</param>
	/// <param name="mousePressed">キー</param>
	/// <returns></returns>
	bool IsClicked(sf::Vector2f arg_mousePos, bool arg_mousePressed) const
	{
		return arg_mousePressed && IsHovered(arg_mousePos);
	}

	/// <summary>
	/// カラー変更
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const sf::Color& arg_color)
	{
		m_shape.setFillColor(arg_color);
	}

};