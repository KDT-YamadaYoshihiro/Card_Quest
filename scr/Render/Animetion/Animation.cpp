#include "Animation.h"
#include "../RenderSystem.h"

void Animation::AddFrame(const sf::Texture& arg_texture)
{
	m_frames.push_back(arg_texture);
	if (m_frames.size() == 1)
	{
		m_sprite.setTexture(m_frames[0]);
	}
}

void Animation::SetPosition(const sf::Vector2f& arg_pos)
{
	m_sprite.setPosition(arg_pos);
}

void Animation::Update(float arg_dt)
{
	// フレームが無ければ何もしない
	if (m_frames.empty())
	{
		return;
	}

	// タイマー更新
	m_timer += arg_dt;
	// フレーム切り替え
	if (m_timer >= m_frameTiem)
	{
		m_timer -= m_frameTiem;
		m_index = (m_index + 1) % m_frames.size();
		m_sprite.setTexture(m_frames[m_index]);
	}
}

void Animation::Draw(RenderSystem& render)
{
	render.Draw(m_sprite);
}
