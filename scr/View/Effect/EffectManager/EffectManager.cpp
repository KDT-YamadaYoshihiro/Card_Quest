#include "EffectManager.h"
#include "CSVLoad/TextureLoader/TextureLoader.h"
#include "View/ConsoleView/ConsoleView.h"
#include "GameMain/WindowSetting.h"

/// <summary>
/// エフェクトを作成して再生
/// </summary>
/// <param name="arg_data"></param>
/// <param name="arg_texture"></param>
/// <param name="arg_pos"></param>
void EffectManager::CreateEffect(const std::string& arg_key, sf::Vector2f arg_pos)
{
    // 各ローダーからデータを取得
    const auto* texture = TextureLoader::GetInstance().GetTextureID(arg_key);
    if (!texture)
    {
        return;
    }

    const EffectData& data = EffectDataLoder::GetInstance().GetConfig(arg_key);
    if(!data.key.empty())
    {
        ConsoleView::GetInstance().Add("Effect Created: " + data.key);
	}

    // PositionType に基づく座標決定
    sf::Vector2f finalPos;
    switch (data.positionType) {
    case PositionType::Target:
        finalPos = arg_pos;
        break;

    case PositionType::PlayerSide:
        // 味方側の基準座標（例）
        finalPos = { windowSize.x * 0.25f, windowSize.y * 0.7f };
        break;

    case PositionType::EnemySide:
        // 敵側の基準座標（例）
        finalPos = { windowSize.x * 0.75f, windowSize.y * 0.3f };
        break;

    case PositionType::WindowCenter:
        // ウィンドウの中央
        finalPos = { windowSize.x / 2.0f, windowSize.y / 2.0f };
        break;
    }

    // インスタンス生成と再生
    auto newEffect = std::make_unique<EffectAnimation>(data, *texture);
	// エフェクトの中心を基準に位置を調整
    newEffect->Play(finalPos);
	// リストに追加
    m_effects.push_back(std::move(newEffect));
}

/// <summary>
/// 全エフェクトの更新
/// </summary>
/// <param name="arg_dt"></param>
void EffectManager::Update(float arg_dt)
{
    // 再生が終わったものを削除しながら更新
    auto it = m_effects.begin();
    while (it != m_effects.end()) {
        (*it)->Update(arg_dt);

        if (!(*it)->IsPlaying()) {
            it = m_effects.erase(it); // 再生終了したらリストから除外
        }
        else {
            ++it;
        }
    }
}

/// <summary>
/// 全エフェクトの描画
/// </summary>
/// <param name="arg_window"></param>
void EffectManager::Draw(sf::RenderWindow& arg_window)
{
    for (auto& effect : m_effects) {
        effect->Draw(arg_window);
    }
}
