#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

// 前方宣言
class Character;

class TargetSelect
{

public:
	// 単体選択
	static std::shared_ptr<Character> SelectSingle(const std::vector<std::shared_ptr<Character>>& arg_targetChara,sf::RenderWindow& arg_window);

	// 全体選択(即決)
	static std::vector<std::shared_ptr<Character>> SelectAll(const std::vector<std::shared_ptr<Character>>& arg_targetChara);

	// 自分自身
	static std::shared_ptr<Character> SelectSelf(const std::shared_ptr<Character>& arg_actionChara);
};

