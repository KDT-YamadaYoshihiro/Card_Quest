#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class CSVLoader
{
protected:
    std::vector<std::string> Split(const std::string& line)
    {
        std::vector<std::string> result;
        std::stringstream ss(line);
        std::string item;

        while (std::getline(ss, item, ',')) {
            result.push_back(item);
        }
        return result;
    }
};
