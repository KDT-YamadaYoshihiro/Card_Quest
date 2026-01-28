#pragma once
#include <vector>
#include <memory>

class Character;

class PartyBuildContext
{
private:

	std::vector<std::shared_ptr<Character>> m_allCharacters;
	std::vector<std::shared_ptr<Character>> m_party;

	// パーティー上限
	size_t m_maxPartySize = 4;


public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="arg_allChara"></param>
	void Init(const std::vector<std::shared_ptr<Character>>& arg_allChara);

	/// <summary>
	/// 全キャラの取得
	/// </summary>
	/// <returns></returns>
	const std::vector<std::shared_ptr<Character>> GetAllCharacters() const;

	/// <summary>
	/// パーティーメンバーキャラの取得
	/// </summary>
	/// <returns></returns>
	const std::vector < std::shared_ptr<Character>> GetParty() const;

	/// <summary>
	/// 指定キャラをパーティに編成
	/// </summary>
	/// <param name="arg_char"></param>
	/// <returns></returns>
	bool AddToParty(const std::shared_ptr<Character>& arg_char);

	/// <summary>
	/// パーティー編成から除外
	/// </summary>
	/// <param name="arg_char"></param>
	/// <returns></returns>
	bool RmoveParty(const std::shared_ptr<Character>& arg_char);

	/// <summary>
	/// 指定キャラがパーティーに入っているか
	/// </summary>
	/// <param name="arg_char"></param>
	/// <returns></returns>
	bool IsInParty(const std::shared_ptr<Character>& arg_char)const;

private:



};

