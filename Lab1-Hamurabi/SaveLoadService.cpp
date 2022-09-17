#include "SaveLoadService.h"
#include <fstream>
#include <iostream>

std::string path = "Save";

bool TrySave(int& year, float& averageStarvedPercent, YearInfo& info, Resources& resources)
{
    std::cout << "Do you want to take a rest and quit? y or n" << std::endl;
    char answer;
    std::cin >> answer;

    if (answer == 'y' || answer == 'Y')
    {
        std::ofstream output {};
        output.open(path, std::ios::trunc | std::ios::binary);
        if (output.is_open())
        {
            output.write((char*)&year, sizeof(int));
            output.write((char*)&averageStarvedPercent, sizeof(float));
            info.Save(output);
            resources.Save(output);

            output.close();
            return true;
        }
    }

    return false;
}
void TryLoad(int& year, float& averageStarvedPercent, YearInfo& info, Resources& resources)
{
    std::ifstream input {};
    input.open(path, std::ios::binary);
    if (input.is_open())
    {
        std::cout << "Do you want to continue last game? y or n" << std::endl;

        char answer;
        std::cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {
            input.read((char*)&year, sizeof(int));
            input.read((char*)&averageStarvedPercent, sizeof(float));
            info.Load(input);
            resources.Load(input);
        }

        input.close();
    }
}
