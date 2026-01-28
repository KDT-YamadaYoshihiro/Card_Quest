#include "PartyBuildContext.h"
#include "Entity/Character/Character.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="arg_allChara"></param>
void PartyBuildContext::Init(const std::vector<std::shared_ptr<Character>>& arg_allChara)
{
	m_allCharacters = arg_allChara;
	m_party.clear();
}

/// <summary>
/// 全キャラの取得
/// </summary>
/// <returns></returns>
const std::vector<std::shared_ptr<Character>> PartyBuildContext::GetAllCharacters() const
{
	return m_allCharacters;
}

/// <summary>
/// パーティーメンバーキャラの取得
/// </summary>
/// <returns></returns>
const std::vector<std::shared_ptr<Character>> PartyBuildContext::GetParty() const
{
	return m_party;
}

/// <summary>
/// 指定キャラをパーティに編成
/// </summary>
/// <param name="arg_char"></param>
/// <returns></returns>
bool PartyBuildContext::AddToParty(const std::shared_ptr<Character>& arg_char)
{
	// すでにパーティメンバーにいないか
	if (IsInParty(arg_char))
	{
		return false;
	}
	// 編成枠に空きがあるか
	if (m_party.size() >= m_maxPartySize)
	{
		return false;
	}

	m_party.push_back(arg_char);
	return true;
}

/// <summary>
/// パーティー編成から除外
/// </summary>
/// <param name="arg_char"></param>
/// <returns></returns>
bool PartyBuildContext::RmoveParty(const std::shared_ptr<Character>& arg_char)
{
	auto it = std::find(m_party.begin(), m_party.end(), arg_char);
	if (it == m_party.end())
	{
		return false;
	}

	m_party.erase(it);
	return true;
}

/// <summary>
/// 指定キャラがパーティーに入っているか
/// </summary>
/// <param name="arg_char"></param>
/// <returns></returns>
bool PartyBuildContext::IsInParty(const std::shared_ptr<Character>& arg_char)const
{
	return std::find(m_party.begin(), m_party.end(), arg_char) != m_party.end();
}
