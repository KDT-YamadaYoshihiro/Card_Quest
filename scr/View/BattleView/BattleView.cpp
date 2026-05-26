#include "BattleView.h"
#include "CSVLoad/TextureLoader/TextureLoader.h"
#include "View/Font/FontManager.h"
#include "Entity/Card/CardManager/CardManager.h"
#include "System/Battle/Cost/CostManager.h"
#include "CSVLoad/StageLoader/StageLoader.h"
#include "GameMain/WindowSetting.h"
#include "System/Constants.h"

// ���W�n
namespace BattleViewConstants
{
    // ハンド配置定数 (Constants.h と重複するためコメントアウト)
    constexpr sf::Vector2f HAND_START{ 200.f, 520.f };
    constexpr float HAND_SPACING = Constants::HAND_SPACING;
    constexpr float SELECT_OFFSET_Y = Constants::SELECT_OFFSET_Y;
}

/// <summary>
/// ������
/// </summary>
/// <param name="context"></param>
BattleView::BattleView(BattleContext& context, RenderSystem& render)
    : m_context(context),
    m_render(render),
    m_font(FontManager::GetInstance().GetFont()),
	m_clearBannerText(m_font, "")
{
    m_cardRenderer = std::make_shared<CardRenderer>();
}

/// <summary>
/// �t�F�[�Y�̐ݒ�
/// </summary>
/// <param name="phase"></param>
void BattleView::SetPhase(BattleViewPhase phase)
{
    m_phase  = phase;
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="dt"></param>
void BattleView::Update(float dt)
{
    UpdateCamera(dt);

    for (auto& p : m_popups)
    {
        namespace BattleViewConstants
        {
            // ハンド配置定数 (Constants.h と重複するためコメントアウト)
            constexpr sf::Vector2f HAND_START{ 200.f, 520.f };
            constexpr float HAND_SPACING = Constants::HAND_SPACING;
            constexpr float SELECT_OFFSET_Y = Constants::SELECT_OFFSET_Y;
        }

        /// <summary>
        /// ������
        /// </summary>
        /// <param name="context"></param>
        BattleView::BattleView(BattleContext& context, RenderSystem& render)
            : m_context(context),
            m_render(render),
            m_font(FontManager::GetInstance().GetFont()),
            m_clearBannerText(m_font, "")
        {
            m_cardRenderer = std::make_shared<CardRenderer>();
        }

        /// <summary>
        /// �t�F�[�Y�̐ݒ�
        /// </summary>
        /// <param name="phase"></param>
        void BattleView::SetPhase(BattleViewPhase phase)
        {
            m_phase  = phase;
        }

        /// <summary>
        /// �X�V
        /// </summary>
        /// <param name="dt"></param>
        void BattleView::Update(float dt)
        {
            UpdateCamera(dt);

            for (auto& p : m_popups)
            {
                p.lifeTime -= dt;
                p.position.y -= Constants::SELECT_OFFSET_Y * dt; // 選択時のオフセット分移動
            }

            m_popups.erase(
                std::remove_if(m_popups.begin(), m_popups.end(),
                    [](const DamagePopup& p) { return p.lifeTime <= 0.f; }),
                m_popups.end());
        }

        /// <summary>
        /// �A�N�V�����L�����ݒ�
        /// </summary>
        /// <param name="index"></param>
        void BattleView::SetSelectedActor(const std::shared_ptr<Character>& actor)
        {
            m_selectedActor = actor;
        }

        /// <summary>
        /// �I���J�[�h�̃C���f�b�N�X�ݒ�
        /// </summary>
        /// <param name="arg_index"></param>
        void BattleView::SetSelectedCard(int arg_index)
        {
            m_selectedCardId = index;
        }

        /// <summary>
        /// �^�[�Q�b�g���̃C���f�b�N�X�ݒ�
        /// </summary>
        void BattleView::SetTargetIndices(const std::vector<std::shared_ptr<Character>>& arg_target)
        {
            m_targets = arg_target;
        }


        /// <summary>
        /// �R�X�g�̃C���f�b�N�X�ݒ�
        /// </summary>
        /// <param name="value"></param>
        void BattleView::ShowCostGain(int value)
        {
            m_costGain = value;
        }

        /// <summary>
        /// ��񃊃Z�b�g
        /// </summary>
        void BattleView::ResetTransientView()
        {
            ClearTargets();
            ClearCostGain();
            m_selectedCardId = -1;
        }

        /// <summary>
        /// ���ʗʂ̐ݒ�
        /// </summary>
        /// <param name="arg_pos"></param>
        /// <param name="arg_value"></param>
        /// <param name="arg_isHeal"></param>
        void BattleView::AddDamagePopup(const sf::Vector2f& arg_pos, int arg_value, bool arg_isHeal)
        {
            DamagePopup p;
            p.position = arg_pos;
            p.value = arg_value;
            p.isHeal = arg_isHeal;
            p.lifeTime = 1.0f;

            m_popups.push_back(p);
        }

        /// <summary>
        /// �`��
        /// </summary>
        /// <param name="window"></param>
        void BattleView::Render(sf::RenderWindow& arg_window)
        {
            // �J�����@�\\ON
            //m_render.ApplyCamera();

            // �L�����N�^�[�`��
            DrawCharacters();

            // �^�[�Q�b�g�t�H�[�J�X�̕`��
            if (!m_context.GetFocusTargets().empty())
            {
                DrawFocus(arg_window);
            }

            // �_���[�W��񕜗ʂ̕\\��
            for (auto& p : m_popups)
            {
                sf::Text text(FontManager::GetInstance().GetFont(), "");
                text.setFont(FontManager::GetInstance().GetFont()); // �����̃t�H���g�Ǘ�
                text.setString((p.isHeal ? "+" : "-") + std::to_string(p.value));
                text.setCharacterSize(24);
                text.setPosition(p.position);
                text.setFillColor(p.isHeal ? sf::Color::Green : sf::Color::Red);

                arg_window.draw(text);
            }
            // �^�[�Q�b�g�̃T�[�N��
            DrawFocus(arg_window);

            // �J�����@�\\OFF
            //m_render.ResetCamera();
            // �J�[�h
            DrawCards(arg_window);
            // �s����
            DrawCost(arg_window);
            // �����s����
            if (m_context.GetFocusDraw()) {
                DrawCostGain(arg_window);
            }
            // �o�[�i�[
            DrawTurnBanner(arg_window);
            // �X�e�[�W��
            DrawStageName(arg_window);
        }

        /// <summary>
        /// �L�����N�^�[�`��
        /// </summary>
        /// <param name="window"></param>
        void BattleView::DrawCharacters()
        {

            auto drawList = [&](const auto& list)
                {
                    for (auto& c : list)
                    {
                        if (!c) continue;

                        // ===== Sprite�����i����̂݁j=====
                        if (!m_spriteTable.contains(c.get()))
                        {
                            auto sprite = std::make_unique<CharacterSprite>(c->GetData().textureKey);
                            sprite->Init(c->GetData().textureKey);
                            m_spriteTable[c.get()] = std::move(sprite);
                        }

                        auto& sprite = m_spriteTable[c.get()];

                        // ===== ��Ԍ��� =====
                        CharacterAnimState state = CharacterAnimState::WAIT;

                        // Animation
                        if (c->IsDead())
                        {
                            sprite->SetState(CharacterAnimState::DEAD);
                        }
                        else {
                            sprite->SetState(c->GetAnimState());
                        }

                        // ���o���W���擾���ăZ�b�g
                        sprite->SetPosition(c->GetVisualPosition());

                        if (c->GetFaction() == Faction::Player)
                        {
                            sprite->SetSpriteWidthMirror();
                        }

                        sprite->Draw(m_render,c->GetData(),true);
                    }
                };

            drawList(m_context.GetPlayers());
            drawList(m_context.GetEnemies());
        }

        /// <summary>
        /// �J�[�h�`��
        /// </summary>
        /// <param name="window"></param>
        void BattleView::DrawCards(sf::RenderWindow& arg_window)
        {
            // ===== �R�D =====
            m_cardRenderer->DrawDeck(m_font, m_render.GetWindow(), DECK_POS, CardManager::GetInstance().GetDeckCount());


            // ===== ��D =====
            sf::Vector2f pos = HAND_START;

            // ��D�S�̂ł̒ʂ��ԍ����J�E���g����ϐ�
            int globalCardIdx = 0;

            // �I�𒆂̃O���[�o���C���f�b�N�X���擾
            int selectedCardGlobalIdx = m_context.GetSelectedCardIndex();

            for (auto& p : m_context.GetAlivePlayers())
            {
                const int cardCount = p->GetCardCount();

                for (int i = 0; i < cardCount; ++i)
                {
                    sf::Vector2f drawPos = pos;

                    // ���[�J���� i �ł͂Ȃ��A��D�S�̂ł̒ʂ��ԍ��Ŕ�r����
                    if (globalCardIdx == selectedCardGlobalIdx)
                    {
                        drawPos.y -= SELECT_OFFSET_Y;
                    }

                    // �J�[�h�f�[�^�擾
                    const CardData& data = p->GetCardData(i);

                    // �`��
                    m_cardRenderer->DrawSingleCard(m_font, m_render.GetWindow(), drawPos, data, p->GetData().iconKey);

                    // ���̃J�[�h�֍��W�ƒʂ��ԍ���i�߂�
                    pos.x += HAND_SPACING;
                    globalCardIdx++;
                }
            }
        }

        /// <summary>
        /// �^�[�Q�b�g�t�H�[�J�X�i�~��\\���j
        /// </summary>
        /// <param name="window"></param>
        void BattleView::DrawFocus(sf::RenderWindow& arg_window)
        {

            const auto& focusTargets = m_context.GetFocusTargets();

            for (const auto& target : focusTargets)
            {
                if (!target)
                {
                    continue;
                }

        	auto tex = TextureLoader::GetInstance().GetTextureID("targetFrame");
        	sf::Sprite sprite(*tex);
        	sprite.setOrigin(sf::Vector2f(tex->getSize().x / 2.f, tex->getSize().y / 2.f));
        	// �L�����N�^�[�̒��S���W���擾���Đݒ�
        	sf::Vector2f centerPos = GetCharacterCenter(target);
        	sprite.setScale({ 0.1f,0.1f });
        	sprite.setPosition({ centerPos.x, centerPos.y + 10.f });
        	arg_window.draw(sprite);

            }

        }

        /// <summary>
        /// �R�X�g�\\��
        /// </summary>
        /// <param name="arg_window"></param>
        void BattleView::DrawCost(sf::RenderWindow& arg_window)
        {
            auto tex = TextureLoader::GetInstance().GetTextureID("CostFrame");
            sf::Sprite sprite(*tex);
            sprite.setPosition(sf::Vector2f(550.0f, 100.0f));
            sprite.setScale({0.13f, 0.08f});
            arg_window.draw(sprite);

            sf::Text text(m_font, "");
            int cost = CostManager::GetInstance().GetCurrentCost();
            text.setString({"AP / " +  std::to_string(cost)});
            auto pos = sprite.getPosition();
            text.setPosition({ pos.x + 55 , pos.y + 30 });
            arg_window.draw(text);
        }


        /// <summary>
        /// �ǉ��R�X�g�\\��
        /// </summary>
        /// <param name="window"></param>
        void BattleView::DrawCostGain(sf::RenderWindow& arg_window)
        {
        	// �t�H�[�J�X�\\�����L���ȏꍇ�̂ݕ`��
            if (!m_context.GetFocusDraw()) {
                return;
            }

            int val = m_context.GetPredictedCost();

            sf::Text gainText(m_font,"");
            gainText.setFont(m_font);
            gainText.setCharacterSize(36);
            gainText.setOutlineColor(sf::Color::Black);
            gainText.setOutlineThickness(2.f);

            if (val == 0)
            {
                // --- �ω��Ȃ��F���F�� "0" �\\�� ---
                gainText.setFillColor(sf::Color::White);
                gainText.setString(" + 0");
            }
        	else if (val > 0)
        	{
                // --- �R�X�g�l���F�ΐF�� "+" �\\�� ---
        		gainText.setFillColor(sf::Color::Green);
        		gainText.setString(" + " + std::to_string(val));
        	}
            else
            {
                // --- �R�X�g����F�ԐF�� "-" �\\�� ---
                gainText.setFillColor(sf::Color::Red);
                gainText.setString(" - " + std::to_string(std::abs(val)));
            }

            // �\\���ʒu�F���݂�AP�\\���̉E���ɔz�u
            // (DrawCost�̍��W 50.f, 650.f �ɑ΂��Ē���)
            gainText.setPosition(sf::Vector2f(750.0f, 130.0f));

            arg_window.draw(gainText);
        }

        /// <summary>
        /// �X�e�[�W���̕\\��
        /// </summary>
        /// <param name="arg_window"></param>
        void BattleView::DrawStageName(sf::RenderWindow& arg_window)
        {

            // �X�e�[�WID�̎擾
            int stageId = m_context.GetStageId();

            const auto& stageData = StageLoader::GetInstance().GetStageData(stageId);

            if (!stageData)
            {
                return;
            }
            // �X�e�[�W��
            sf::Text stageText(m_font, "");
            // UTF8�ɑΉ�������
            sf::String unicodeString = sf::String::fromUtf8(stageData->name.begin(), stageData->name.end());
            stageText.setString(unicodeString);
            stageText.setCharacterSize(30);
            stageText.setFillColor(sf::Color::White);
            stageText.setOutlineColor(sf::Color::Black); 
            stageText.setOutlineThickness(2.f);
            stageText.setPosition({ 20.0f, 50.0f });

            // ���n
            sf::FloatRect textBounds = stageText.getGlobalBounds();
            sf::RectangleShape bgBox;

            // �{�b�N�X�̃T�C�Y�F�e�L�X�g�͈̔͂�菭���傫������i�p�f�B���O�j
            float paddingX = Constants::PADDING_X;
            float paddingY = Constants::PADDING_Y;
            bgBox.setSize({ textBounds.size.x + paddingX * 2, textBounds.size.y + paddingY * 2 });

            // �{�b�N�X�̐F�ݒ�i�������̐ɂ���Ɖ�ʂɓ���݂₷���ł��j
            bgBox.setFillColor(sf::Color(0, 50, 150, 180)); // �Â߂̐A��������
            bgBox.setOutlineColor(sf::Color::Cyan);        // �g���ɖ��邢��
            bgBox.setOutlineThickness(2.f);

            // �{�b�N�X�̈ʒu�F�e�L�X�g���ނ悤�ɔz�u
            bgBox.setPosition({ textBounds.position.x - paddingX, textBounds.position.y - paddingY });

            // 4. �`��i��Ƀ{�b�N�X�A���̌�Ƀe�L�X�g���d�˂�j
            arg_window.draw(bgBox);
            arg_window.draw(stageText);
        }

        /// <summary>
        /// �^�[���̃o�i�[�\\��
        /// </summary>
        /// <param name="window"></param>
        void BattleView::DrawTurnBanner(sf::RenderWindow& window)
        {
            // BattleSystem.h �� TurnPhase ��`�ƍ��킹��
            // 0: StartTurn, 1: UserTurn, 2: EnemyTurn ... 
            int phase = m_context.GetTurnPhase();

            std::string turnStr = "";
            sf::Color textColor = sf::Color::White;

            if (phase == 1) { // UserTurn
                turnStr = "PLAYER TURN";
                textColor = sf::Color::Cyan; // �n
            }
            else if (phase == 2) { // EnemyTurn
                turnStr = "ENEMY TURN";
                textColor = sf::Color(255, 100, 100); // �Ԍn
            }
            else {
                return; // ����ȊO�̃t�F�[�Y�iStart���j�ł͕\\�����Ȃ��A�܂��͕ʂ̕\\��
            }

            sf::Text text(m_font, "");
            text.setString(turnStr);
            text.setCharacterSize(50); // �傫�߂ɕ\\��
            text.setFillColor(textColor);
            text.setOutlineColor(sf::Color::Black);
            text.setOutlineThickness(4.f);

            // ��ʒ����ɔz�u
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin({ textRect.position.x + textRect.size.x / 2.0f,textRect.position.y + textRect.size.y / 2.0f });

            // �E�B���h�E�T�C�Y���擾���Ē�����
            sf::Vector2u windowSize = WindowSetting::GetInstance().GetWindowSize();
            text.setPosition({ windowSize.x / 2.0f, 50.f }); 

            window.draw(text);
        }

        /// <summary>
        /// �^�[�Q�b�g���̃��Z�b�g
        /// </summary>
        void BattleView::ClearTargets()
        {
            m_targets.clear();
        }

        /// <summary>
        /// �s���{���̑����l���Z�b�g
        /// </summary>
        void BattleView::ClearCostGain()
        {
            m_costGain = 0;
        }

        /// <summary>
        /// �L�����N�^�[�̒��S���W�̎擾
        /// </summary>
        /// <param name="c"></param>
        /// <returns></returns>
        sf::Vector2f BattleView::GetCharacterCenter(const std::shared_ptr<Character>& c)
        {
            constexpr float CHAR_W = Constants::CHAR_W;
            constexpr float CHAR_H = Constants::CHAR_H;

            // �L�����N�^�[�̍�����W���擾
            sf::Vector2f pos = c->GetPosition();

            // ��`�T�C�Y�iCHAR_W, CHAR_H�j�̔����𑫂��Ē��S���v�Z
            return { pos.x + (CHAR_W * 0.5f), pos.y + (CHAR_H * 0.5f) };
        }

        /// <summary>
        /// �J�����X�V
        /// </summary>
        /// <param name="dt"></param>
        void BattleView::UpdateCamera(float dt)
        {

            auto& camera = CameraManager::GetInstance();

            switch (m_phase)
void BattleView::UpdateCamera(float dt)
{

    auto& camera = CameraManager::GetInstance();

    switch (m_phase)
    {
    case BattleViewPhase::SelectPlayer:
    case BattleViewPhase::SelectCard:
    {
        // �S�v���C���[�����܂钆�S�_���v�Z
        const auto& players = m_context.GetPlayers();
        if (players.empty()) break;

        sf::Vector2f averageCenter{ 0.f, 0.f };
        for (auto& p : players)
        {
            averageCenter += GetCharacterCenter(p);
        }
        averageCenter /= static_cast<float>(players.size());

        camera.SetMove(averageCenter, 5.0f);
        camera.SetZoom(1.0f, 2.0f);
        camera.ViewStopFollow();
        break;
    }

    case BattleViewPhase::SelectTarget:
    {
        if (m_targets.empty())
        {
            break;
        }

        sf::Vector2f targetCenter{ 0.f, 0.f };
        for (auto& t : m_targets)
        {
            targetCenter += GetCharacterCenter(t);
        }

        targetCenter /= static_cast<float>(m_targets.size());

        // �^�[�Q�b�g�I�����͏����Y�[���C�����Ē��ڂ�����
        camera.SetMove(targetCenter, 6.0f);
        camera.SetZoom(0.7f, 3.0f);
        break;
    }
    case BattleViewPhase::Default:
        // �����ʒu�ɖ߂�
        camera.SetMove({ 640.0f, 360.0f }, 4.0f);
        camera.SetZoom(1.0f, 2.0f);
        break;
    }

    camera.ViewUpdate(dt);
}

/// <summary>
/// �_���[�W�|�b�v�A�b�v�ʒu�v�Z
/// </summary>
/// <param name="c"></param>
/// <returns></returns>
sf::Vector2f BattleView::CalcDamagePopupPos(const std::shared_ptr<Character>& c)
{
    auto center = GetCharacterCenter(c);
    center.y -= 40.f; // ����ɕ�������
    return center;
}

/// <summary>
/// �Q�[���N���A�o�i�[�̕`��
/// </summary>
/// <param name="arg_window"></param>
void BattleView::DrawClearBanner(sf::RenderWindow& arg_window)
{

	m_clearBannerText = sf::Text(m_font, "GAME CLEAR!");

	// �o�i�[������ݒ�
	m_clearBannerText.setString("STAGE CLEAR!");
	m_clearBannerText.setCharacterSize(60);
	m_clearBannerText.setFillColor(sf::Color::Green);
	m_clearBannerText.setOutlineColor(sf::Color::Black);
	m_clearBannerText.setOutlineThickness(4.f);
	// ��ʒ����ɔz�u
	sf::FloatRect textRect = m_clearBannerText.getLocalBounds();
	m_clearBannerText.setOrigin({ textRect.position.x + textRect.size.x / 2.0f,textRect.position.y + textRect.size.y / 2.0f });
	// �E�B���h�E�T�C�Y���擾���Ē�����
	sf::Vector2u windowSize = WindowSetting::GetInstance().GetWindowSize();
	m_clearBannerText.setPosition({ windowSize.x / 2.0f, windowSize.y / 2.0f });
	arg_window.draw(m_clearBannerText);


}

/// <summary>
/// �Q�[���I�[�o�[�o�i�[�̕`��
/// </summary>
/// <param name="arg_window"></param>
void BattleView::DrawGameOverBanner(sf::RenderWindow& arg_window)
{

    m_clearBannerText = sf::Text(m_font, "GAME OVER");


	// �o�i�[������ݒ�
	m_clearBannerText.setString("GAME OVER");
	m_clearBannerText.setCharacterSize(60);
	m_clearBannerText.setFillColor(sf::Color::Red);
	m_clearBannerText.setOutlineColor(sf::Color::Black);
	m_clearBannerText.setOutlineThickness(4.f);
	// ��ʒ����ɔz�u
	sf::FloatRect textRect = m_clearBannerText.getLocalBounds();
	m_clearBannerText.setOrigin({ textRect.position.x + textRect.size.x / 2.0f,textRect.position.y + textRect.size.y / 2.0f });
	// �E�B���h�E�T�C�Y���擾���Ē�����
	sf::Vector2u windowSize = WindowSetting::GetInstance().GetWindowSize();
	m_clearBannerText.setPosition({ windowSize.x / 2.0f, windowSize.y / 2.0f });
	arg_window.draw(m_clearBannerText);

}
