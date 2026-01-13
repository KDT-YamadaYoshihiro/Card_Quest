#include "RenderSystem.h"
#include <SFML/Graphics.hpp>

// カメラ適用
void RenderSystem::ApplyCamera()
{
	m_window.setView(CameraManager::GetInstance().GetView());
}
// カメラリセット
void RenderSystem::ResetCamera()
{
	m_window.setView(m_window.getDefaultView());
}
// 描画
void RenderSystem::Draw(const sf::Drawable& drawable)
{
	m_window.draw(drawable);
}

// 描画（状態付き）
void RenderSystem::Draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
	m_window.draw(drawable, states);
}

// 画面クリア
void RenderSystem::Clear(const sf::Color& color)
{
	m_window.clear(color);
}

// 画面表示
void RenderSystem::Display()
{
	m_window.display();
}
