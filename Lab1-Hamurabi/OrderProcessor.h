#pragma once
#include "Resources.h"

struct PlayerOrders
{
    int acresToBuy;
    int acresToSell;
    int wheatToFeed;
    int acresToPlant;
};

PlayerOrders ProcessOrders(int tradingPrice, Resources& resources);
