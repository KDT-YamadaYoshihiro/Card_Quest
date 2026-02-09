#include "MenuWindow.h"
#include "View/Font/FontManager.h"
#include "System/InPutManager/InPutMouseManager.h"

MenuWindow::MenuWindow()
    : m_isOpen(false), 
    m_result(MenuResult::None),
    m_menuTitle(FontManager::GetInstance().GetFont(),"")
{
}

bool MenuWindow::Init()
{
    // 共通フォントの取得
    const sf::Font& font = FontManager::GetInstance().GetFont();

    // 1. メニューを開くためのボタン（画面右上に配置する例）
    m_menuOpenButton = std::make_unique<BoxButton>(
        sf::Vector2f(120.f, 40.f), sf::Vector2f(1150.f, 30.f), font, "MENU");

    // 2. メニュー背景（画面全体を薄暗くする半透明の矩形）
    // ウィンドウサイズ 1280x720 を想定。環境に合わせて調整してください。
    m_overlay.setSize(sf::Vector2f(1280.f, 720.f));
    m_overlay.setFillColor(sf::Color(0, 0, 0, 180));

    // 3. メニュータイトル
    m_menuTitle.setFont(font);
    m_menuTitle.setString("PAUSE");
    m_menuTitle.setCharacterSize(60);
    m_menuTitle.setFillColor(sf::Color::White);
    // 中央揃え
    sf::FloatRect textRect = m_menuTitle.getLocalBounds();
    m_menuTitle.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
    m_menuTitle.setPosition({ 640.f, 150.f });

    // 4. メニュー内ボタンの生成（中央に縦に並べる）
    float centerX = 640.f;
    m_continueBtn = std::make_unique<BoxButton>(sf::Vector2f(300.f, 60.f), sf::Vector2f(centerX, 300.f), font, "Continue");
    m_retryBtn = std::make_unique<BoxButton>(sf::Vector2f(300.f, 60.f), sf::Vector2f(centerX, 400.f), font, "Retry");
    m_giveUpBtn = std::make_unique<BoxButton>(sf::Vector2f(300.f, 60.f), sf::Vector2f(centerX, 500.f), font, "Give Up");

    return true;
}

void MenuWindow::Update(sf::RenderWindow& window)
{

    static bool isEscPrev = false; // 前回のフレームで押されていたか
    bool isEscCurrent = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape);

    if (!isEscCurrent && isEscPrev)
    {
        if (!m_isOpen)
        {
            m_isOpen = true;
            m_result = MenuResult::None;
        }
        else
        {
            // メニューが開いている時にEscを離すと「続行」扱いにする
            m_isOpen = false;
            m_result = MenuResult::Cntinue;
        }
    }
    isEscPrev = isEscCurrent;

    // マウス入力判定
    if (InPutMouseManager::GetInstance().IsLeftClickConfirmed())
    {
        HandleInput(window);
    }
}

void MenuWindow::HandleInput(sf::RenderWindow& window)
{
    // マウス座標の取得（InPutMouseManagerの機能を使用）
    auto& input = InPutMouseManager::GetInstance();
    sf::Vector2f mousePos = input.GetMousePosition(window);

    if (!m_isOpen)
    {
        // メニューが閉じているとき：オープンボタンの判定
        if (m_menuOpenButton->IsClicked(mousePos, input.IsLeftClicked()))
        {
            m_isOpen = true;
            m_result = MenuResult::None; // 選択状態をリセット
        }
    }
    else
    {
        // メニューが開いているとき：各メニュー項目の判定
        if (m_continueBtn->IsClicked(mousePos,input.IsLeftClicked()))
        {
            m_isOpen = false;
            m_result = MenuResult::Cntinue;
        }
        else if (m_retryBtn->IsClicked(mousePos,input.IsLeftClicked()))
        {
            m_isOpen = false; // 必要に応じて閉じる
            m_result = MenuResult::Retry;
        }
        else if (m_giveUpBtn->IsClicked(mousePos,input.IsLeftClicked()))
        {
            m_isOpen = false;
            m_result = MenuResult::GiveUp;
        }
    }
}

void MenuWindow::Render(sf::RenderWindow& window)
{
    // メニューを開くためのボタンは常に（またはゲームプレイ中）描画
    m_menuOpenButton->Draw(window);

    // メニューが開いている場合、オーバーレイとメニュー内ボタンを描画
    if (m_isOpen)
    {
        window.draw(m_overlay);
        window.draw(m_menuTitle);

        m_continueBtn->Draw(window);
        m_retryBtn->Draw(window);
        m_giveUpBtn->Draw(window);
    }
}

bool MenuWindow::IsOpen() const
{
    return m_isOpen;
}

MenuResult MenuWindow::GetResult() const
{
    return m_result;
}

void MenuWindow::Reset()
{
    // メニューを閉じた状態に戻す
    m_isOpen = false;

    // 選択結果を「なし（選択中）」の状態に戻す
    m_result = MenuResult::None;
}
