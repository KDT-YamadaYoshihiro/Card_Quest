#include "../Battle/BattleSystem.h"
#include "../Battle/Calculation/Calculation.h"
#include "../Character/Factory/CharacterFactory.h"
#include "../Card/CardFactory/CardFactory.h"
#include "../../View/Font/FontManager.h"
#include "../Battle/SelectTarget/TargetSelect.h"
#include "../Battle/UserController/UserController.h"
#include "../Battle/UserController/ActionData.h"

// フォントのインスタンス取得省略
#define FontMgr FontManager::GetInstance()

// 初期化
BattleSystem::BattleSystem()
	:m_cost(std::make_unique<CostManager>()),
	m_renderer(std::make_unique<CardRenderer>()),
    m_phase(TurnPhase::StartTurn),
    m_turnCount(1),
    m_choiceCardIndex(0)
{
	// エンティティ作成
    CreateEntity();
	// コスト初期化
    m_cost->Init(3);
}

// 初期化
void BattleSystem::Init()
{
}

// 更新
void BattleSystem::Update(sf::RenderWindow& arg_window)
{
    if (CheckWin() || CheckLose()) {
        m_phase = TurnPhase::EndTurn;
        return;
    }

    switch (m_phase) {

    case TurnPhase::StartTurn:

        // ターン開始処理
        StartTurn();

        break;

    case TurnPhase::UserTurn:

        // プレイヤーアップデートを呼ぶ
        PlayerUpdate(arg_window);

        break;

    case TurnPhase::EnemyTurn:

        // エネミーアップデートを呼ぶ
        EnemyUpdate();

        break;

    case TurnPhase::EndTurn:

        // ターン終了処理
        EndTurn();

        break;

    default:
        break;
    }

}

// 描画
void BattleSystem::Render(sf::RenderWindow& window)
{

    // 山札
    m_renderer->DrawDeck(FontMgr.GetFont(), window, {50.0f,300.0f}, CardManager::GetInstance().GetDeckCount());

    // 墓地
    m_renderer->DrawGrave(FontMgr.GetFont(), window,{150.0f, 300.0f}, CardManager::GetInstance().GetCemeteryCount());

    // 手札
    // ベース座標
    float x = 300.0f;
    float baseY = 300.0f;
    // カード情報の取得
    const auto& handCard = CardManager::GetInstance().GetHandCard();
    // 個々のカード情報
    for (size_t i = 0; i < handCard.size(); i++)
    {
        // 座標の変更
        float y = baseY;

        if(static_cast<int>(i) == m_choiceCardIndex)
        {
            // 選択中カードを浮かす
            y -= 30;
        }
        // 描画
        m_renderer->DrawHand(FontMgr.GetFont(), window, { x, y }, *handCard[i]);
        // 一枚ごとにx座標をずらす
        x += 130.0f;
    }


    // カード選択決定ボタン
    

}

// カードの使用
void BattleSystem::OnUseCard(size_t arg_handIndex,size_t arg_targetIndex)
{
    //auto result = CardManager::GetInstance().UseCard(arg_handIndex);
    //auto& owner = m_players[result.ownerID];
    //// ターゲットを選択したとき
    //if (hand[m_choiceCardIndex]->GetCardState().targetType == TargetType::OPPONENT ||
    //    hand[m_choiceCardIndex]->GetCardState().targetType == TargetType::ALLY)
    //{
    //    auto target = TargetSelect::SelectSingle(candidates, arg_window);

    //    ApplyCardAction(result, owner, target);
    //}
    //else
    //{
    //    auto targets = TargetSelect::SelectAll(candidates);

    //    for (auto& t : targets) {
    //        ApplyCardAction(result, owner, t);
    //    }
    //}

}

// カード使用時の影響
void BattleSystem::ApplyAction(const Action& action)
{
    for (auto& target : action.targets)
    {
        if (!target || target->GetStatus().dead)
            continue;

        switch (action.card.actionType)
        {
            case ActionType::ATTCK:
        {
            float dmg = Calculation::GetDamage(
                action.user->GetStatus().atk,
                action.card.power,
                target->GetStatus().def
            );
            target->TakeDamage(dmg);
            break;
        }
        case ActionType::MAGIC:
            // 同様
            break;

        case ActionType::HEAL:
            target->TakeHeal(action.card.power);
            break;

        case ActionType::BUFF:
            target->TakeBuff(action.card.power);
            break;
        }
    }
}

// 生成関数
void BattleSystem::CreateEntity()
{

    // プレイヤー、カード作成
    for (int i = 1; i < 5; i++) {

        auto player = CharacterFactory::Instance().CreateCharacter<Player>(i);
        m_players.push_back(player);

        const std::vector<int>& cardIds = player->GetStatus().cardIds;
        auto cards = CardFactory::GetInstance().CreateDeck(cardIds,i);

        if (i == 1) {
            CardManager::GetInstance().InitDeck(std::move(cards));
        }
        else
        {
            CardManager::GetInstance().AddDeckCard(std::move(cards));
        }
    }

	// コントローラー作成
    m_userController = std::make_unique<UserController>();
    

	// エネミー作成
    for (int i = 101; i < 102; i++) {
        auto enemy = CharacterFactory::Instance().CreateCharacter<Enemy>(i);
        m_enemies.push_back(enemy);
	}
}

// カード選択時のキャラクターフォーカス
void BattleSystem::UpdateCardOwnerFocus()
{

    // 全キャラクターのフォーカスを解除
    for (auto& p : m_players)
    {
        p->SetFocused(false);
    }

    // カード未選択なら終了
    if (m_choiceCardIndex < 0)
    {
        return;
    }

    // カード情報の取得
    const auto& hand = CardManager::GetInstance().GetHandCard();

    if (m_choiceCardIndex >= static_cast<int>(hand.size()))
    {
        return;
    }

    // 行動アクションを取得
    auto actionChara = GetActionCharacterFromCard(*hand[m_choiceCardIndex]);
    if (!actionChara)
    {
        return;
    }

    actionChara->SetFocused(true);
}

// カード情報からリンクするキャラクターを取得する
std::shared_ptr<Character> BattleSystem::GetActionCharacterFromCard(const Card& arg_card)
{
    int ownerId = arg_card.GetOwnerId();

    if (ownerId < 0 || ownerId >= static_cast<int>(m_players.size()))
    {
        return nullptr;
    }

    return m_players[ownerId];
}

std::vector<std::shared_ptr<Character>> BattleSystem::MakeTargetCandidates(const std::shared_ptr<Character>& actionChara,TargetType targetType) const
{
	// ターゲット候補
    std::vector<std::shared_ptr<Character>> result;

	// 生存者のみ追加するラムダ
    auto pushAlive = [&](const std::vector<std::shared_ptr<Character>>& list)
        {
            for (const auto& c : list)
            {
                if (c && !c->GetStatus().dead)
                {
                    result.push_back(c);
                }
            }
        };

	// ターゲットタイプごとに候補を追加
    switch (targetType)
    {
		// --- 敵単体・全体 ---
    case TargetType::OPPONENT:
    case TargetType::OPPONENT_ALL:
        if (actionChara->IsPlayer()) {
            pushAlive(m_enemies);
        }
        else
        {
            pushAlive(m_players);
        }

        break;

		// --- 味方単体・全体 ---
    case TargetType::ALLY:
    case TargetType::ALLY_ALL:
        if (actionChara->IsPlayer()) {
            pushAlive(m_players);
        }
        else
        {
            pushAlive(m_enemies);
        }

        break;

		// --- 自身 ---
    case TargetType::SELF:
        if (actionChara && !actionChara->GetStatus().dead) {
            result.push_back(actionChara);
        }
        break;

    default:
        break;
    }

    return result;
}

// ターン開始時
void BattleSystem::StartTurn()
{
    // 手札補充
    CardManager::GetInstance().DeckToHand(5);

    // コスト回復
    m_cost->ResetCost();

	// フェーズをプレイヤーターンに変更
    m_phase = TurnPhase::UserTurn;
}

// プレイヤー更新
void BattleSystem::PlayerUpdate(sf::RenderWindow& window)
{
    auto currentPlayer = m_players[m_currentPlayerIndex];

    m_userController->SetActionCharacter(currentPlayer);

    // ターゲット候補生成
    if (m_userController->HasAction() == false)
    {
        // 選択中カードがある前提で後段で渡される
    }

	// 更新
    m_userController->Update(window);

	// アクションがあるなら実行
    if (m_userController->HasAction())
    {
		// アクション適用
        ApplyAction(m_userController->PopAction());

		// 次のプレイヤーへ
        m_currentPlayerIndex++;
        if (m_currentPlayerIndex >= m_players.size())
        {
            m_currentPlayerIndex = 0;
            m_phase = TurnPhase::EnemyTurn;
        }
    }
}

// エネミー更新
void BattleSystem::EnemyUpdate()
{
    // エネミーはカードをランダムで選択一回だけアクションを起こして終了
    for (auto& e : m_enemies)
    {
        e->Update();
    }

    // 終了時
    // メンバー全員がアクションを行ったら終了
    m_phase = TurnPhase::EndTurn;
}

// ターン終了時
void BattleSystem::EndTurn()
{
	// ターン数を増やす
    m_turnCount++;

    // 生存確認後続行か判定
    for (auto& p : m_players) {
        for (auto& e : m_enemies) {
            if (!p->GetStatus().dead && !e->GetStatus().dead) {

                p->UpdateBuff();
                e->UpdateBuff();

                m_phase = TurnPhase::StartTurn;
            }
        }
    }
}
