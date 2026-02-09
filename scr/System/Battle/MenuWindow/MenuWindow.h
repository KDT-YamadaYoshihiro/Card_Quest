#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "UI/BoxButton.h"
#include "View/Render/RenderSystem.h"

// メニューからの遷移
enum class MenuResult
{
	Cntinue,	// 続行
	Retry,		// 再挑戦	
	GiveUp,		// あきらめる
	None,		// 選択中
};

class MenuWindow
{
private:

	// メニューボタン
	std::unique_ptr<BoxButton> m_menuOpenButton;

	// メニュー画面内ボタン
	std::unique_ptr<BoxButton> m_continueBtn;
	std::unique_ptr<BoxButton> m_retryBtn;
	std::unique_ptr<BoxButton> m_giveUpBtn;

	// 背景（半透明のボックス)
	sf::RectangleShape m_overlay;
	sf::Text m_menuTitle;

	// 表示フラグ
	MenuResult m_result = MenuResult::None;
	bool m_isOpen = false;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	MenuWindow();
	virtual ~MenuWindow() = default;

	/// <summary>
	/// 生成、初期化
	/// </summary>
	/// <returns></returns>
	bool Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="arg_window"></param>
	void Update(sf::RenderWindow& arg_window);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="arg_window"></param>
	void Render(sf::RenderWindow& arg_window);

	/// <summary>
	/// メニューオープンフラグの取得
	/// </summary>
	/// <returns></returns>
	bool IsOpen()const;
	
	/// <summary>
	/// 遷移先の取得
	/// </summary>
	/// <returns></returns>
	MenuResult GetResult() const;

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

private:

	/// <summary>
	/// クリック判定
	/// </summary>
	/// <param name="arg_window"></param>
	void HandleInput(sf::RenderWindow& arg_window);

};

