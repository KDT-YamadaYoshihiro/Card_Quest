#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "View/CameraManager/CameraManager.h"


class RenderSystem
{

	sf::RenderWindow& m_window;

public:

	RenderSystem(sf::RenderWindow& arg_window)
		:m_window(arg_window)
	{
		CameraManager::GetInstance().ViewInit(
		    { Constants::WINDOW_CENTER_X, Constants::WINDOW_CENTER_Y },
			{ static_cast<float>(arg_window.getSize().x), static_cast<float>(arg_window.getSize().y) });
	}

	// �`��֘A
	// �J�����K�p
	void ApplyCamera();
	// �J�������Z�b�g
	void ResetCamera();
	// �`��
	void Draw(const sf::Drawable& drawable);
	// �`��i�ʒu�E��]�j
	void Draw(const sf::Drawable& drawable, const sf::RenderStates& states);
	// ��ʃN���A
	void Clear(const sf::Color& color = sf::Color::Black);
	// ��ʕ\��
	void Display();

	// Window�ւ̃A�N�Z�X
	sf::RenderWindow& GetWindow() { return m_window; }
};