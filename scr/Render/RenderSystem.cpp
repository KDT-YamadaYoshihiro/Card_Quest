#include "RenderSystem.h"
#include <SFML/Graphics.hpp>

void RenderSystem::ApplyCamera()
{
	m_window.setView(CameraManager::Instance().GetView());
}

void RenderSystem::ResetCamera()
{
	m_window.setView(m_window.getDefaultView());
}

void RenderSystem::Draw(const sf::Drawable& drawable)
{
	m_window.draw(drawable);
}

void RenderSystem::Draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
	m_window.draw(drawable, states);
}

void RenderSystem::Clear(const sf::Color& color)
{
	m_window.clear(color);
}

void RenderSystem::Display()
{
	m_window.display();
}
