#include "ConsoleView.h"
#include <iostream>
#include <cstdlib>

// NDEBUGが定義されていない（＝Debugモード）場合のみコンパイル
#ifndef NDEBUG

void ConsoleView::Add(const std::string& text)
{
    m_views.push_back(text);
}

void ConsoleView::RenderFrame()
{
    if (m_views == m_lastViews)
    {
        m_views.clear();
        return;
    }

#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(hConsole, coord);
#else
    system("clear");
#endif

    for (const auto& line : m_views)
    {
        std::cout << line << std::endl;
    }

    m_lastViews = m_views;
    m_views.clear();
}

void ConsoleView::Reset()
{
    m_views.clear();
    m_lastViews.clear();

#ifdef _WIN32
    // system("cls") は非常に重い処理なのでDebug時でも注意が必要
    system("cls");
#else
    system("clear");
#endif
}

#endif // NDEBUG