#pragma once

#include "Resources.h"
#include "OrderProcessor.h"
#include <fstream>

struct YearInfo
{
    float starvedPercent = 0.0f;
    int starvedPeople = 0;
    int arrivedPeople = 5;
    int harvestedBushels = 3000;
    int harvestedBushelsPerAcre = 3;
    int bushelsEatenByRats = 10;
    bool wasPlague = false;

    void Save(std::ofstream& os)
    {
        os.write((char*)&starvedPercent, sizeof(float));
        os.write((char*)&starvedPeople, sizeof(int));
        os.write((char*)&arrivedPeople, sizeof(int));
        os.write((char*)&harvestedBushels, sizeof(int));
        os.write((char*)&harvestedBushelsPerAcre, sizeof(int));
        os.write((char*)&bushelsEatenByRats, sizeof(int));
        os.write((char*)&wasPlague, sizeof(bool));
    }

    void Load(std::ifstream& is)
    {
        is.read((char*)&starvedPercent, sizeof(float));
        is.read((char*)&starvedPeople, sizeof(int));
        is.read((char*)&arrivedPeople, sizeof(int));
        is.read((char*)&harvestedBushels, sizeof(int));
        is.read((char*)&harvestedBushelsPerAcre, sizeof(int));
        is.read((char*)&bushelsEatenByRats, sizeof(int));
        is.read((char*)&wasPlague, sizeof(bool));
    }
};

YearInfo ProcessYear(const PlayerOrders& orders, Resources& resources);
int ProcessTradingPrice();
