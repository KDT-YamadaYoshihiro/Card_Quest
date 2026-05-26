#pragma once
#include <vector>
#include <memory>
#include "System/Singleton/Singleton.h"
#include "View/Effect/EffectAnimation/EffectAnimation.h"

class EffectManager : public Singleton<EffectManager>
{
private:

	friend class Singleton<EffectManager>;
	// �G�t�F�N�g�̃��X�g
	std::vector<std::unique_ptr<EffectAnimation>> m_effects;

public:

	/// <summary>
	/// �G�t�F�N�g���쐬���čĐ�
	/// </summary>
	/// <param name="arg_data"></param>
	/// <param name="arg_texture"></param>
	/// <param name="arg_pos"></param>
	void CreateEffect(const std::string& arg_key, sf::Vector2f arg_pos);

	/// <summary>
	/// �S�G�t�F�N�g�̍X�V
	/// </summary>
	/// <param name="arg_dt"></param>
	void Update(float arg_dt);

	/// <summary>
	/// �S�G�t�F�N�g�̕`��
	/// </summary>
	/// <param name="arg_window"></param>
	void Draw(sf::RenderWindow& arg_window);

	/// <summary>
	/// �G�t�F�N�g�̍Đ��������f
	/// </summary>
	/// <returns></returns>
	bool GetPlay()const;

};

