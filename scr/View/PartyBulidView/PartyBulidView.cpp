#include "PartyBuildView.h"
#include "View/Render/RenderSystem.h"
#include "System/PartyBulid/PartyBuildContext/PartyBuildContext.h"
#include "CSVLoad/TextureLoader/TextureLoader.h"
#include "Entity/Character/Character.h"
#include "View/ConsoleView/ConsoleView.h"
#include "View/ChracterSprite/CharacterSprite.h"
#include "View/Font/FontManager.h"
#include <iostream>


/// <summary>
/// 初期化
/// </summary>
/// <param name="render"></param>
PartyBuildView::PartyBuildView(RenderSystem& arg_render)
	:m_render(arg_render)
{
    // --- パーティエリアの背景設定 ---
    m_partyBackground.setPosition(PARTY_POS);
    m_partyBackground.setSize(PARTY_SIZE);
    m_partyBackground.setFillColor(sf::Color(30, 30, 30, 150)); // 暗めの半透明
    m_partyBackground.setOutlineThickness(2.0f);
    m_partyBackground.setOutlineColor(sf::Color(100, 100, 100)); // グレーの枠線

    // --- ベンチエリアの背景設定 ---
    m_benchBackground.setPosition(BENCH_POS);
    m_benchBackground.setSize(BENCH_SIZE);
    m_benchBackground.setFillColor(sf::Color(30, 30, 30, 150));
    m_benchBackground.setOutlineThickness(2.0f);
    m_benchBackground.setOutlineColor(sf::Color(100, 100, 100));
}

/// <summary>
/// 更新
/// </summary>
/// <param name="context"></param>
void PartyBuildView::Update(const PartyBuildContext& arg_context)
{
	// アイコン（クリック判定用やリスト表示用）の構築
	m_characterIcons.clear();
	sf::Vector2f start{ 120.0f, 415.0f };
	float spacingX = 180.0f;
	int index = 0;

	for (auto& ch : arg_context.GetAllCharacters())
	{
		auto tex = TextureLoader::GetInstance().GetTextureID(ch->GetData().iconKey);
		if (!tex) continue;

		// 座標計算
		sf::Vector2f pos = { start.x + index * spacingX, start.y };

		IconView icon(*tex, pos, { SIZE_X, SIZE_Y });

		// アイコン画像の調整（左向きにする等）
		icon.sprite.setOrigin({ icon.sprite.getLocalBounds().size.x, 0.0f });
		icon.sprite.setScale({ -1.0f, 1.0f });
		icon.sprite.setPosition(icon.rect.position);

		m_characterIcons.push_back(icon);
		++index;
	}

	// ==========================================
	// 2. 現在のパーティのアイコン構築（クリック判定用）
	// ==========================================
	m_partyIcons.clear();

	// 画面上部に配置
	sf::Vector2f partyStart{ 250.0f, 130.0f };
	float partySpacingX = 200.0f;
	index = 0;

	for (auto& ch : arg_context.GetParty())
	{
		// 描画自体はSpriteで行うが、Controllerでのクリック判定用にIconView(Rect)を作っておく
		auto tex = TextureLoader::GetInstance().GetTextureID(ch->GetData().iconKey);
		if (!tex) continue; // テクスチャがなくてもRectだけは作れますが、基本ある前提

		sf::Vector2f pos = { partyStart.x + index * partySpacingX, partyStart.y };

		IconView icon(*tex, pos, { SIZE_X, SIZE_Y });
		m_partyIcons.push_back(icon);

		++index;
	}
}

/// <summary>
/// 描画
/// </summary>
void PartyBuildView::Draw(const PartyBuildContext& arg_context)
{

    // Scene の案内文字
    sf::Text titleText(FontManager::GetInstance().GetFont(), "Party Build");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition({ 50.f, 10.f });
    m_render.Draw(titleText);

    // 案内文字
    sf::Text instructionText(FontManager::GetInstance().GetFont(), "-Select your party members.-");
    instructionText.setCharacterSize(30);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition({ 400.f, 50.f });
    m_render.Draw(instructionText);



    // 背景ボックス
    m_render.Draw(m_partyBackground);
    m_render.Draw(m_benchBackground);

    // 文字列描画
    sf::Text partyLabel(FontManager::GetInstance().GetFont(), "PARTY");
    partyLabel.setFillColor(sf::Color::White);
    partyLabel.setPosition({PARTY_POS.x + 10.0f, PARTY_POS.y + 5.0f});
	m_render.Draw(partyLabel);

	sf::Text benchLabel(FontManager::GetInstance().GetFont(), "BENCH" );
	benchLabel.setFillColor(sf::Color::White);
    benchLabel.setPosition({ BENCH_POS.x + 10.0f, BENCH_POS.y});
	m_render.Draw(benchLabel);

    // キャラクター一覧（ベンチ）の描画
    auto allChars = arg_context.GetAllCharacters();
    sf::Vector2f start{ 120.0f, 415.0f };
    float spacingX = 180.0f;

    for (size_t i = 0; i < allChars.size(); ++i)
    {
        auto& ch = allChars[i];
        auto tex = TextureLoader::GetInstance().GetTextureID(ch->GetData().iconKey);
        if (!tex) continue;

        // その場で位置を確定
        sf::Vector2f pos = { start.x + i * spacingX, start.y };

        sf::Sprite iconSprite(*tex);
        // 原点と反転の設定を固定
        iconSprite.setOrigin({ iconSprite.getLocalBounds().size.x, 0.0f });
        iconSprite.setScale({ -1.0f, 1.0f });
        iconSprite.setPosition(pos);


        // パーティ編成済みのキャラはグレーアウト
        if (arg_context.IsInParty(ch))
        {
            // 暗いグレーかつ半透明に設定 (R, G, B, A)
            // 元の色に対して 100/255 の強さで描画し、透明度を 150 にする
            iconSprite.setColor(sf::Color(100, 100, 100, 255));
        }
        else
        {
            // 通常状態（白を乗算 = 元の色のまま）
            iconSprite.setColor(sf::Color::White);
        }

        m_render.Draw(iconSprite);

    }

    // 選択されたパーティメンバーの描画
    auto party = arg_context.GetParty();
    sf::Vector2f partyStart{ 250.0f, 130.0f };
    float partySpacingX = 200.0f;

    for (size_t i = 0; i < party.size(); ++i)
    {
        auto& ch = party[i];
        if (!ch) continue;

        // Sprite生成（初回のみ）
        if (!m_spriteTable.contains(ch.get()))
        {
            auto sprite = std::make_unique<CharacterSprite>(ch->GetData().textureKey);
            sprite->Init(ch->GetData().textureKey);
            m_spriteTable[ch.get()] = std::move(sprite);
        }

        auto& sprite = m_spriteTable[ch.get()];

        // 重要：描画直前に最新の計算済み座標をセット
        sf::Vector2f pos = { partyStart.x + i * partySpacingX, partyStart.y };
        sprite->SetPosition(pos);

        sprite->SetSpriteWidthMirror();
        sprite->SetState(CharacterAnimState::WAIT);

        // 描画
        sprite->Draw(m_render, ch->GetData(), false);
    }
}


/// <summary>
/// キャラクターアイコンView情報の取得
/// </summary>
/// <returns></returns>
const std::vector<IconView>& PartyBuildView::GetCharacterIcons() const
{
	return m_characterIcons;
}

/// <summary>
/// パーティーキャラアイコンView情報の取得
/// </summary>
/// <returns></returns>
const std::vector<IconView>& PartyBuildView::GetPartyIcons() const
{
	return m_partyIcons;
}

