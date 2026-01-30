#include "TextureLoader.h"
#include <iostream>

bool TextureLoader::LoadTextures(const std::string& path)
{

    // ファイルの確認
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "指定したファイルが見つかりません : " << path << std::endl;
        return false;
    }

    std::string line;
    bool first = true;

    // データ読み込み
    while (std::getline(file, line))
    {
        if (first) { first = false; continue; } // ヘッダスキップ

        auto cols = Split(line);
        if (cols.size() < 2)
            continue;

        std::string id = cols[0];
        std::string filePath = cols[1];

        sf::Texture tex;
        if (!tex.loadFromFile(filePath))
        {
            std::cerr << "ファイルの読み込み失敗 : " << filePath << std::endl;
            continue;
        }

        m_textures[id] = std::move(tex);

    }

    std::cout << "ファイルの読み込み成功" << std::endl;
    return true;
}
