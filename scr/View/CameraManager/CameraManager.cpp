#include "CameraManager.h"

void CameraManager::ViewInit(const sf::Vector2f& center, const sf::Vector2f& size)
{
	m_view.setSize(size);
	m_view.setCenter(center);
	m_moveTarget = center;
	m_currentZoom = 1.0f;
	m_targetZoom = 1.0f;
}

void CameraManager::ViewUpdate(float arg_dt)
{
	// ズーム
	ViewZoomTo(arg_dt);
	// 移動
	ViewMoveTo(arg_dt);
	// 追尾
	ViewFollow(arg_dt);
	// シェイク
	ViewShake(arg_dt);
}

void CameraManager::SetZoom(float arg_target, float arg_speed)
{
	m_targetZoom = arg_target;
	m_zoomSpeed = arg_speed;
	m_zoomEnabled = true;
}

void CameraManager::ViewZoomTo(float arg_dt)
{
	if (m_zoomEnabled)
	{
		float diff = m_targetZoom - m_currentZoom;
		float step = m_zoomSpeed * arg_dt;

		if (std::fabs(diff) <= step)
		{
			m_currentZoom = m_targetZoom;
			m_zoomEnabled = false;
		}
		else
		{
			m_currentZoom += (diff > 0 ? step : -step);
		}

		sf::Vector2f baseSize = { m_view.getSize().x / (m_view.getSize().x / m_view.getSize().x), m_view.getSize().y }; 
		m_view.setSize(sf::Vector2f(1280.0f * m_currentZoom, 720.0f * m_currentZoom)); 
	
	}
}

void CameraManager::SetMove(const sf::Vector2f& arg_target, float arg_speed)
{
	m_moveTarget = arg_target;
	m_moveSpeed = arg_speed;
	m_moveEnabled = true;
}

void CameraManager::ViewMoveTo(float arg_dt)
{
	if (m_moveEnabled)
	{
		sf::Vector2f pos = m_view.getCenter();
		pos += (m_moveTarget - pos) * (m_moveSpeed * arg_dt);

		if (std::hypot(pos.x - m_moveTarget.x, pos.y - m_moveTarget.y) < 0.5f)
		{
			pos = m_moveTarget;
			m_moveEnabled = false;
		}
		m_view.setCenter(pos);
	}
}

void CameraManager::SetFollow(const sf::Vector2f& arg_target, float arg_speed)
{
	m_followTarget = arg_target;
	m_followSpeed = arg_speed;
	m_followEnabled = true;
}

void CameraManager::ViewFollow(float arg_dt)
{
	if (m_followEnabled)
	{
		sf::Vector2f pos = m_view.getCenter();
		pos += (m_followTarget - pos) * (m_followSpeed * arg_dt);
		m_view.setCenter(pos);
	}
}

void CameraManager::ViewStopFollow()
{
	m_followEnabled = false;
}

void CameraManager::SetShake(float arg_power, float arg_time)
{
	m_shakePower = arg_power;
	m_shakeTime = arg_time;
	m_originalCenter = m_view.getCenter();
}

void CameraManager::ViewShake(float arg_dt)
{
	if (m_shakeTime > 0.0f)
	{
		m_shakeTime -= arg_dt;

		float offsetX = ((rand() % 2001 - 1000) / 1000.f) * m_shakePower;
		float offsetY = ((rand() % 2001 - 1000) / 1000.f) * m_shakePower;

		m_view.setCenter(m_originalCenter + sf::Vector2f(offsetX, offsetY));

		if (m_shakeTime <= 0.f)
		{
			m_view.setCenter(m_originalCenter);
		}
	}
}
