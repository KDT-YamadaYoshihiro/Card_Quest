#include "DeckBuildingScene.h"
#include "Scene/SceneManager/SceneManager.h"
#include "Scene/IngameScene/IngameScene.h"
#include "System/DeckBulid/CardBuildPool/CardBuildPool.h"
#include "Entity/Card/CardManager/CardManager.h"
#include "View/Font/FontManager.h"
#include "Scene/PartyBuildScene/PartyBuildScene.h"
#include "UI/BoxButton.h"
#include "System/Constants.h"

DeckBuildingScene::DeckBuildingScene()
    : SceneBase()
{

}

bool DeckBuildingScene::Init(sf::RenderWindow& arg_window)
{
    m_nextButton = std::make_unique<BoxButton>(sf::Vector2f(200.f, 50.f), sf::Vector2f(1000.f, 680.f), FontManager::GetInstance().GetFont(), "BATTLE START");
    if(!m_nextButton)
    {
        ConsoleView::GetInstance().Add("DeckBuildingScene/m_nextButton:nullptr\n");
        return false;
	}

    m_backButton = std::make_unique<BoxButton>(sf::Vector2f(200.f, 50.f), sf::Vector2f(200.f, 680.f), FontManager::GetInstance().GetFont(), "BACK");
    if (!m_backButton)
    {
        ConsoleView::GetInstance().Add("DeckBuildingScene/m_backButton:nullptr\n");
        return false;
    }

    ConsoleView::GetInstance().Add("DeckBuildingScene\n");
    // �J�[�h�v�[���\�z
    CardBuildPool::GetInstance().Build();
    m_deckBuildSystem.Init();

	return true;
}

void DeckBuildingScene::handleEvent(const sf::Event& event)
{
    InPutMouseManager::GetInstance().HandleEvent(event);
}

void DeckBuildingScene::Update(sf::RenderWindow& arg_window, float dt)
{

    // ���C�g�G�t�F�N�g�X�V
    m_lightEffect->Update(dt);

	// ���͍X�V
    auto& input = InPutMouseManager::GetInstance();
    input.Update(arg_window);

	// �}�E�X���W�擾
    sf::Vector2f mousePos = input.GetMousePosition(arg_window);
    float wheel = input.GetWheelDelta();


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

	// �f�b�L�Ґ��V�X�e���X�V
    m_deckBuildSystem.Update(mousePos, input.IsLeftClicked(),input.IsDragging(),input.IsLeftReleased(), wheel);
    // �Ґ������{�^��(�f�b�L������30���ȏ�̎�)
    if (m_nextButton->IsClicked(mousePos, input.IsLeftClicked()))
    {
        if (m_deckBuildSystem.IsComplete())
        {
			// CardManager �Ƀf�b�L���Z�b�g
            CardManager::GetInstance().InitDeck(m_deckBuildSystem.TakeDeck());
            // Console�̃��Z�b�g
            ConsoleView::GetInstance().Reset();
			// �V�[���؂�ւ�
			SceneManager::GetInstance().ChangeScreen<IngameScene>(arg_window);

            return;
        }
        
    }


	// �߂�{�^��
	if (m_backButton->IsClicked(mousePos, input.IsLeftClicked()))
	{
        // CardManager �Ƀf�b�L���Z�b�g
        CardManager::GetInstance().InitDeck(m_deckBuildSystem.TakeDeck());
		// Console�̃��Z�b�g
		ConsoleView::GetInstance().Reset();
		// �V�[���؂�ւ�
		SceneManager::GetInstance().ChangeScreen<PartyBuildScene>(arg_window);

        return;
	}

}

void DeckBuildingScene::Render(sf::RenderWindow& arg_window)
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

    // --- �f�b�L & �v�[���`�� ---
	m_deckBuildSystem.Draw(arg_window, FontManager::GetInstance().GetFont());

    // --- パーティメンバー描画 ---
    auto& session = SceneManager::GetInstance().GetSession();
    const auto& party = session.battleContext->GetPlayers();
    float posX = 850.f;
    for (auto& member : party)
    {
        auto tex = TextureLoader::GetInstance().GetTextureID(member->GetData().iconKey);
        if (!tex)
        {
            return;
        }

        sf::Sprite sprite(*tex);
        sprite.setPosition({ posX, Constants::PADDING_Y * 2 });
        sprite.setOrigin({ sprite.getLocalBounds().size.x, 0.0f });
        sprite.setScale({ -0.8f, 0.8f });
        arg_window.draw(sprite);

        posX += Constants::PADDING_X;
    }

     // --- �����{�^�� ---
    m_nextButton->Draw(arg_window);
	m_backButton->Draw(arg_window);
}

void DeckBuildingScene::End()
{
}
