#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class CameraManager
{
	sf::View m_view;

	float m_currentZoom = 1.0f;
	float m_targetZoom = 1.0f;
	float m_zoomSpeed = 1.0f;
	bool m_zoomEnabled = false;

	sf::Vector2f m_moveTarget;
	float m_moveSpeed = 5.0f;
	bool m_moveEnabled = false;

	sf::Vector2f m_followTarget;
	float m_followSpeed = 5.0f;
	bool m_followEnabled = false;

	float m_shakePower = 0.0f;
	float m_shakeTime = 0.0f;
	sf::Vector2f m_originalCenter;

	CameraManager() = default;
	virtual ~CameraManager() = default;

public:

	// 代入禁止、コピー禁止
	CameraManager(const CameraManager&) = delete;
	CameraManager& operator = (const CameraManager&) = delete;
	static CameraManager& GetInstance()
	{
		static CameraManager instance;
		return instance;
	}

	// 初期化
	void ViewInit(const sf::Vector2f& center, const sf::Vector2f& size);
	// 更新
	void ViewUpdate(float arg_dt);

	// ズーム
	void SetZoom(float arg_target, float arg_speed);
	void ViewZoomTo(float arg_dt);

	// 移動
	void SetMove(const sf::Vector2f& arg_target, float arg_speed);
	void ViewMoveTo(float arg_dt);

	// 追尾
	void SetFollow(const sf::Vector2f& arg_target, float arg_speed);
	void ViewFollow(float arg_dt);

	// 追尾停止
	void ViewStopFollow();

	// シェイク
	void SetShake(float arg_power, float arg_time);
	void ViewShake(float arg_dt);

	sf::View& GetView() { return m_view; }
};