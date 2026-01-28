#include "TargetSelect.h"
#include "Entity/Character/Character.h"
#include "Entity/Card/CardDate.h"
#include <iostream>

/// <summary>
/// 単体選択
/// </summary>
/// <param name="arg_targetChara">ターゲット候補</param>
/// <param name="arg_window"></param>
/// <returns></returns>
std::shared_ptr<Character> TargetSelect::SelectSingle(const std::vector<std::shared_ptr<Character>>& arg_targetChara, sf::RenderWindow& arg_window)
{

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		return nullptr;
	}

	// mouse座標の取得
	auto mousePos = sf::Mouse::getPosition(arg_window);
	sf::Vector2f mouseWoeldPos(static_cast<float>(mousePos.x),static_cast<float>(mousePos.y));

	for (const auto& target : arg_targetChara)
	{

		if (!target || target->GetData().dead)
		{
			continue;
		}

		//if (target->GetHitCircle().contains(mouseWoeldPos))
		//{
		//	// ログ
		//	std::cout << target->GetData().name << "を選択" << std::endl;
		//	return target;
		//}

	}

	return nullptr;

}


/// <summary>
/// 全体選択
/// </summary>
/// <param name="arg_targetChara">ターゲットキャラ</param>
/// <returns></returns>
std::vector<std::shared_ptr<Character>> TargetSelect::SelectAll(const std::vector<std::shared_ptr<Character>>& arg_targetChara)
{
	
	std::vector<std::shared_ptr<Character>> result;

	for (const auto& target : arg_targetChara)
	{
		result.push_back(target);
	}

	return result;

}

/// <summary>
/// 自分自身
/// </summary>
/// <param name="arg_actionChara">行動者</param>
/// <returns></returns>
std::shared_ptr<Character> TargetSelect::SelectSelf(const std::shared_ptr<Character>& arg_actionChara)
{
	// 念のためnullptrでないか、生きているか判定する
	if (arg_actionChara && !arg_actionChara->GetData().dead)
	{
		// 自分自身なのでそのまま引数を返す。
		return arg_actionChara;
	}

	return nullptr;
}
