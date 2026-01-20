#include "UserController.h"
#include "../../Card/CardManager/CardManager.h"
#include "../../System/InPutManager/InPutManager.h"

//// コンストラクタ
//UserController::UserController()
//	:m_selectedCardIndex(-1),
//    m_decisionButton(40.0f,{ 860.0f, 525.0f }),
//	m_turnEndButton(40.0f, { 1000.0f, 525.0f }),
//	m_requestTurnEnd(false)
//{
//}
//
//// 描画
//void UserController::Draw(sf::RenderWindow& window) const
//{
//	// 決定ボタン描画
//	m_decisionButton.Draw(window);
//	// ターン終了ボタン描画
//	m_turnEndButton.Draw(window);
//}
//
//// カード選択
//std::optional<CardData> UserController::SelectCard(sf::RenderWindow& window, const std::vector<std::unique_ptr<Card>>& arg_hand)
//{
//
//	// 左クリック
//    if (InputManager::GetInstance().IsLeftClicked())
//    {
//        int index = GetClickHandIndex(window);
//        if (index >= 0)
//        {
//			// 選択カード更新
//			m_selectedCardIndex = index;
//            m_selectedCard = arg_hand[index]->GetCardState();
//        }
//	}
//
//	// 決定ボタン
//	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
//	bool leftClick = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
//    if (m_decisionButton.IsClicked(mousePos, leftClick) && m_selectedCard.has_value())
//    {
//		// 選択済みカードを返す
//		return m_selectedCard;
//    }
//
//	// 未選択
//    return std::nullopt;
//}
//
//// ターゲット選択
//std::optional<std::vector<std::shared_ptr<Character>>> UserController::SelectTarget(sf::RenderWindow& window, const std::vector<std::shared_ptr<Character>>& arg_target, const CardData& arg_card, const std::shared_ptr<Character>& arg_actionChara)
//{
//
//	// ターゲット候補保存
//	std::vector<std::shared_ptr<Character>> result;
//    // ターゲット単体
//	std::shared_ptr<Character> target;
//
//	// ターゲットタイプ別処理
//    switch (arg_card.targetType)
//    {
//	case TargetType::OPPONENT:
//	case TargetType::ALLY:
//
//        if (!InputManager::GetInstance().IsLeftClicked())
//        {
//            return std::nullopt;
//        }
//
//		// 単体選択
//		target = TargetSelect::SelectSingle(arg_target, window);
//		// 選択されたら確定
//        if (target)
//        {
//			result.push_back(target);
//            return result;
//        }
//
//        break;
//
//	case TargetType::OPPONENT_ALL:
//	case TargetType::ALLY_ALL:
//
//		// 全体選択は誰かをクリックしたら確定
//        if (InputManager::GetInstance().IsLeftClicked())
//        {
//            result = TargetSelect::SelectAll(arg_target);
//			return result;
//        }
//
//        break;
//
//	case TargetType::SELF:
//
//		// 自分自身を選択
//		result.push_back(TargetSelect::SelectSelf(arg_actionChara));
//        return result;
//
//    default:
//        break;
//    }
//
//	// 未選択
//	return std::nullopt;
//}
//
//// 選択カードindex取得
//int UserController::GetSelectedCardIndex() const
//{
//    return m_selectedCardIndex; 
//}
//
//// ターン終了ボタン用
//bool UserController::IsTurnEndRequested() const
//{
//    return m_requestTurnEnd;
//}
//
//// ターン終了要求リセット
//void UserController::ResetTurnEndRequest()
//{
//    m_requestTurnEnd = false;
//}
//
//// コントローラーのリセット
//void UserController::Reset()
//{
//    m_selectedCard.reset();
//    m_selectedCardIndex = -1;
//}
//
//// クリックされた手札のインデックス取得
//int UserController::GetClickHandIndex(sf::RenderWindow& window) const
//{
//	// カードサイズ
//    constexpr float CARD_W = 120.0f;
//    constexpr float CARD_H = 160.0f;
//
//	// 手札のベース座標
//    float startX = 300.0f;
//    float y = 300.0f;
//
//	// マウス座標取得
//    auto mouse = sf::Mouse::getPosition(window);
//    sf::Vector2f mousePos(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
//
//	// 手札取得
//    const auto& hand = CardManager::GetInstance().GetHandCard();
//
//	// 各カードと当たり判定
//    for (size_t i = 0; i < hand.size(); i++)
//    {
//        sf::FloatRect rect(
//            { startX + i * 130.0f, y },
//            { CARD_W, CARD_H }
//        );
//
//        if (rect.contains(mousePos))
//        {
//            return static_cast<int>(i);
//        }
//    }
//
//    return -1;
//}
//
//// 決定ボタンがクリックされたか
//bool UserController::IsDecisionButtonClicked(sf::RenderWindow& window) const
//{
//    constexpr sf::FloatRect decisionButton(
//        { 800.f, 500.f },   // 座標
//        { 120.f, 50.f }     // サイズ
//    );
//
//    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
//    {
//        auto mouse = sf::Mouse::getPosition(window);
//        sf::Vector2f pos(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
//        return decisionButton.contains(pos);
//    }
//    return false;
//}
//
//// 終了ボタンが押されているか確認
//bool UserController::IsTurnEndButtonClicked(sf::RenderWindow& window) const
//{
//	// マウス座標取得
//    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
//    bool leftClick = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
//	// ターン終了ボタンクリック判定
//    if (m_turnEndButton.IsClicked(mousePos, leftClick))
//    {
//		return true;
//    }
//    else {
//		return false;
//    }
//
//}

/// <summary>
/// 初期化
/// </summary>
/// <param name="arg_context"></param>
/// <returns>成功：失敗</returns>
bool UserController::Init(BattleContext* arg_context)
{
	m_context = arg_context;
	// コンテキストチェック
	if (!m_context)
	{
		std::cout << "UserController/m_context:nullptr" << std::endl;
		return false;
	}
	// リセット
	Reset();

	return true;
}

/// <summary>
/// 更新
/// </summary>
void UserController::Update()
{
	// 選択キャラ確認
    if (!m_selectedActor)
    {
        // UI: キャラ選択
        return;
    }

	// ターゲット確認
    if (m_selectedTargets.empty())
    {
        // UI: ターゲット選択
        return;
    }

}

/// <summary>
/// 行動確定確認
/// </summary>
/// <returns></returns>
bool UserController::IsActionConfirmed()const
{

}

/// <summary>
/// 行動確定取得
/// </summary>
/// <returns></returns>
Action UserController::TakeConfirmedAction()
{

}

/// <summary>
/// リセット
/// </summary>
void UserController::Reset()
{

}
