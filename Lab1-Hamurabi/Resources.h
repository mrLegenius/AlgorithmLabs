#pragma once
#include <fstream>

struct Resources
{
    int wheat = 2800;
    int acres = 1000;
    int population = 100;

    void Save(std::ofstream& os)
    {
        os.write((char*)&wheat, sizeof(int));
        os.write((char*)&acres, sizeof(int));
        os.write((char*)&population, sizeof(int));
    }

    void Load(std::ifstream& is)
    {
        is.read((char*)&wheat, sizeof(int));
        is.read((char*)&acres, sizeof(int));
        is.read((char*)&population, sizeof(int));
    }
};
