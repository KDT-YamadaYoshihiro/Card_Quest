#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class CameraManager
{
	sf::View m_view;

	float m_currentZoom = 1.0f;
	float m_raegetZoom = 1.0f;
	float m_zoomSpeed = 1.0f;
	bool m_zoomEnabled = false;

	sf::Vector2f m_moveTarget;
	float m_moveSpeed = 5.0f;
	bool m_followEnabled = false;

	sf::Vector2f m_followTarget;
	float m_followSpeed = 5.0f;
	bool m_followEnabled = false;

	float m_shakePower = 0.0f;
	float m_dhakeTime = 0.0f;

	CameraManager() = default;
	virtual ~CameraManager() = default;

public:

	// コピー代入禁止
	CameraManager(const CameraManager&) = delete;
	CameraManager& operator = (const CameraManager&) = delete;
	static CameraManager& Instance()
	{
		static CameraManager instance;
		return instance;
	}
	CameraManager(const sf::Vector2f& size)
		:m_view(sf::FloatRect(0, 0, size.x, size.y))
	{

	}

	// 更新
	void Update(float arg_dt);

	// 追尾
	void Follow(const sf::Vector2f& arg_target, float arg_speed);

	// 移動
	void MoveTo(const sf::Vector2f& target, float arg_speed);
	
	// ズーム
	void ZoomTo(float arg_target, float arg_speed);

	// シェイク
	void Shake(float arg_power, float arg_time);

	const sf::View& GetView() const { return m_view; }

};