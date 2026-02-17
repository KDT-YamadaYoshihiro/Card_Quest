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
    const auto* texture = TextureLoader::GetInstance().GetTextureID(arg_key);
    if (!texture) return;

    const EffectData& data = EffectDataLoder::GetInstance().GetConfig(arg_key);

    sf::Vector2f finalPos = arg_pos;
    sf::Vector2f finalScale = { 1.0f, 1.0f };

    switch (data.positionType) {
    case PositionType::WIndowScenter:
        // 画面中央（解像度に合わせて調整）
        finalPos = sf::Vector2f(1920.0f / 2.0f, 1080.0f / 2.0f);
        break;

    case PositionType::PlayerSide:
    case PositionType::EnemySize:
        // 全体演出：渡された基準座標（一番左のキャラ等）を使用し、サイズを大きくする
        finalPos = arg_pos;
        finalScale = { 2.5f, 2.5f }; // 全体を覆う程度の倍率（任意調整）
        break;

    case PositionType::PlayerChara:
    case PositionType::EnemyChara:
    default:
        // 単体演出：渡された座標そのまま
        finalPos = arg_pos;
        break;
    }

    auto newEffect = std::make_unique<EffectAnimation>(data, *texture);
    // スケールを適用して再生
    newEffect->Play(finalPos, finalScale);

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
