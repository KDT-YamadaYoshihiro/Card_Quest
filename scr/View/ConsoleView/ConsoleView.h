#pragma once
#include <vector>
#include <string>
#include "System/Singleton/Singleton.h"
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

class ConsoleView : public Singleton<ConsoleView>
{
private:

	friend class Singleton<ConsoleView>;

	/// <summary>
	/// 表示用テキスト一覧
	/// </summary>
	std::vector<std::string> m_views;
	/// <summary>
	/// 表示履歴一覧
	/// </summary>
	std::vector<std::string> m_lastViews;


public:

	// 表示内容の追加
	void Add(const std::string& arg_text);


	// 表示内容の表示
	void RenderFrame();

	// 表示内容のリセット
	void Reset();

};

