#include "TextureManager.h"
#include <iostream>

bool TextureManager::Load(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "TextureLoaderが見つかりません " << path << std::endl;
        return false;
    }

    std::string line;
    bool first = true;

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
            std::cerr << "Failed to load texture: " << filePath << std::endl;
            continue;
        }

        m_textures[id] = std::move(tex);
    }
}
