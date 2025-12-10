#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class CSVLoader
{
protected:

	// ƒJƒ“ƒ}‹æØ‚è‚Å•ªŠ„
    std::vector<std::string> Split(const std::string& line)
    {
		// Œ‹‰ÊŠi”[—p
        std::vector<std::string> result;
        std::stringstream ss(line);
        std::string item;

        while (std::getline(ss, item, ',')) {
            result.push_back(item);
        }
        return result;
    }
};
