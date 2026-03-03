#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

void cleanFile(const fs::path &inputPath, const fs::path &outputPath)
{
    std::ifstream read(inputPath);
    std::ofstream write(outputPath);

    if (!read.is_open() || !write.is_open())
        return;

    std::string line;
    while (std::getline(read, line))
    {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

        if (line.empty() || line.find_first_not_of(" \t\n") == std::string::npos)
        {
            continue;
        }

        size_t commaCount = std::count(line.begin(), line.end(), ',');

        if (commaCount == 4)
        {
            write << line << "\n";
        }
    }
}

int main()
{
    std::string inputDir = "Z:\\File\\code\\SomTum\\data";
    std::string outputDir = "Z:\\File\\code\\SomTum\\data_clean";

    if (!fs::exists(outputDir))
    {
        fs::create_directories(outputDir);
    }

    for (const auto &entry : fs::directory_iterator(inputDir))
    {
        if (entry.is_regular_file())
        {
            fs::path filePath = entry.path();

            fs::path destination = fs::path(outputDir) / filePath.filename();

            cleanFile(filePath, destination);
        }
    }

    return 0;
}