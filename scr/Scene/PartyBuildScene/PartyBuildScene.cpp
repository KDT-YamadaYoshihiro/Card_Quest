#include "PartyBuildScene.h"
#include "Entity/Character/Factory/CharacterFactory.h"
#include "System/Battle/BattleContex/BattleContext.h"
#include "Scene/SceneManager/SceneManager.h"
#include "Scene/DeckBuildingScene/DeckBuildingScene.h"
#include "System/InPutManager/InPutMouseManager.h"
#include "View/Font/FontManager.h"
#include "Scene/StageBuildScene/StageBulidScene.h"

/// <summary>
/// ������
/// </summary>
PartyBuildScene::PartyBuildScene()
	:SceneBase()
{

    ConsoleView::GetInstance().Add("PartyBuildScene\n");
}

/// <summary>
/// ������
/// </summary>
/// <param name="arg_window"></param>
bool PartyBuildScene::Init(sf::RenderWindow& arg_window)
{
    m_render = std::make_unique<RenderSystem>(arg_window);
    if (!m_render)
    {
        ConsoleView::GetInstance().Add("PartyBuildScene/m_render:nullptr\n");
        return false;
	}

    std::vector<std::shared_ptr<Character>> allChars;
    for (int i = 1; i <= MAX_PARTY_SIZE; ++i)
    {
        allChars.push_back(CharacterFactory::GetInstance().CreateCharacter<PlayerCharacter>(i, 1));
    }

    m_context.Init(allChars);

    if(m_context.GetParty().empty())
    {
        ConsoleView::GetInstance().Add("PartyBuildScene/m_context.GetParty():empty\n");
	}

    m_view = std::make_unique<PartyBuildView>(*m_render);
    if (!m_view)
    {
        ConsoleView::GetInstance().Add("PartyBuildScene/m_view:nullptr\n");
        return false;
    }

    m_controller = std::make_unique<PartyBuildController>(m_context, *m_view);
    if (!m_controller)
    {
        ConsoleView::GetInstance().Add("PartyBuildScene/m_controller:nullptr\n");
        return false;
    }

	// �{�^��
    m_nextButton = std::make_unique<BoxButton>(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), sf::Vector2f(1000.f, 680.f), FontManager::GetInstance().GetFont(), "NEXT");
    if(!m_nextButton)
    {
        ConsoleView::GetInstance().Add("PartyBuildScene/m_nextButton:nullptr\n");
        return false;
	}

    m_backButton = std::make_unique<BoxButton>(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), sf::Vector2f(200.f, 680.f), FontManager::GetInstance().GetFont(), "BACK");
    if (!m_backButton)
    {
        ConsoleView::GetInstance().Add("PartyBuildScene/m_backButton:nullptr\n");
        return false;
    }

	return true;
}

/// <summary>
/// �C�x���g����
/// </summary>
/// <param name="event"></param>
void PartyBuildScene::handleEvent(const sf::Event& event)
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="arg_window"></param>
void PartyBuildScene::Update(sf::RenderWindow& arg_window, float dt)
{
    m_view->Update(m_context);
    m_controller->Update(m_render->GetWindow());
    
    // ���͍X�V
    auto& input = InPutMouseManager::GetInstance();
    input.Update(arg_window);

    // �}�E�X���W�擾
    sf::Vector2f mousePos = input.GetMousePosition(arg_window);
    float wheel = input.GetWheelDelta();

    // ���C�g�G�t�F�N�g�X�V
    m_lightEffect->Update(dt);


	// �{�^���̃J�[�\������������F��ς���
	// �����{�^��
    if(m_nextButton->IsHovered(mousePos))
    {
        m_nextButton->SetColor(sf::Color::Yellow);
    }
    else
    {
        m_nextButton->SetColor(sf::Color::White);
    }
	// �߂�{�^��
    if(m_backButton->IsHovered(mousePos))
    {
        m_backButton->SetColor(sf::Color::Yellow);
    }
    else
    {
        m_backButton->SetColor(sf::Color::White);
	}


	// �߂�{�^������
    if (m_backButton->IsClicked(mousePos, input.IsLeftClicked()))
    {
        // Console�̃��Z�b�g
        ConsoleView::GetInstance().Reset();
        SceneManager::GetInstance().ChangeScreen<StageBulidScene>(arg_window);
        return;
	}
	// ���փ{�^������
    if (m_nextButton->IsClicked(mousePos, input.IsLeftClicked()))
    {
        if (!m_context.GetParty().empty()) {
            StartDeckBulid(arg_window);
			return;
        }
    }
}

/// <summary>
/// �`��
/// </summary>
/// <param name="arg_window"></param>
void PartyBuildScene::Render(sf::RenderWindow& arg_window)
{
    // �w�i
    auto tex = TextureLoader::GetInstance().GetTextureID("bg");
    if (tex)
    {
        sf::Sprite sprite(*tex);
        sprite.setPosition({ 0.0f,0.0f });
        sprite.setScale({ 0.7f,0.7f });
        arg_window.draw(sprite);
    }
    
	// ���C�g�G�t�F�N�g�`��
    m_lightEffect->Draw(arg_window);
	// �p�[�e�B�[�Ґ���ʕ`��
    m_view->Draw(m_context);
	// �{�^���`��
    m_nextButton->Draw(arg_window);
    m_backButton->Draw(arg_window);

}

/// <summary>
/// �I������
/// </summary>
void PartyBuildScene::End()
{
}

/// <summary>
/// �f�b�L�Ґ���ʂɈڍs
/// </summary>
void PartyBuildScene::StartDeckBulid(sf::RenderWindow& arg_window)
{
    // �p�[�e�B�[�����Z�b�g
    auto& session = SceneManager::GetInstance().GetSession();

    session.battleContext->SetPlayers(m_context.GetParty());

    // Console�̃��Z�b�g
    ConsoleView::GetInstance().Reset();

    // �V�[���̐؂�ւ�
    SceneManager::GetInstance().ChangeScreen<DeckBuildingScene>(arg_window);
}
