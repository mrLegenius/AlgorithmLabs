#include "YearProcessor.h"
#include <random>
#include <iostream>

static constexpr int MinNewComers = 0;
static constexpr int MaxNewComers = 50;
static constexpr float PlagueChance = 0.15f;
static constexpr float MaxRatsCoefficient = 0.07f;
static constexpr int MinHarvestedBushelsPerAcre = 1;
static constexpr int MaxHarvestedBushelsPerAcre = 6;
static constexpr int WheatConsumptionAsFood = 20;

static constexpr int MinTradingPrice = 17;
static constexpr int MaxTradingPrice = 26;

std::random_device rd;
std::mt19937 mt(rd());

void HarvestBushels(const PlayerOrders &orders, Resources &resources, YearInfo &info);
void FeedRats(Resources &resources, YearInfo &info);
void FeedPeople(const PlayerOrders &orders, Resources &resources, YearInfo &info);
void WelcomeNewComers(Resources &resources, YearInfo &info);
void Plague(Resources &resources, YearInfo &info);

YearInfo ProcessYear(const PlayerOrders& orders, Resources& resources)
{
    YearInfo info{};

    HarvestBushels(orders, resources, info);
    FeedRats(resources, info);
    FeedPeople(orders, resources, info);
    WelcomeNewComers(resources, info);
    Plague(resources, info);

    return info;
}

void HarvestBushels(const PlayerOrders &orders, Resources &resources, YearInfo &info)
{
    std::uniform_int_distribution<int> wheatPlantDistribution(MinHarvestedBushelsPerAcre, MaxHarvestedBushelsPerAcre);
    info.harvestedBushelsPerAcre = wheatPlantDistribution(mt);
    info.harvestedBushels = orders.acresToPlant * info.harvestedBushelsPerAcre;

    resources.wheat += info.harvestedBushels;
}

void FeedRats(Resources &resources, YearInfo &info)
{
    std::uniform_real_distribution<float> ratsEatDistribution(0.0f, MaxRatsCoefficient);
    float ratsCoefficient = ratsEatDistribution(mt);
    info.bushelsEatenByRats = resources.wheat * ratsCoefficient;

    resources.wheat -= info.bushelsEatenByRats;
}

void FeedPeople(const PlayerOrders &orders, Resources &resources, YearInfo &info)
{
    int wheatToFeedEveryone = resources.population * WheatConsumptionAsFood;
    if (wheatToFeedEveryone <= orders.wheatToFeed) return;

    int insufficientWheat = wheatToFeedEveryone - orders.wheatToFeed;
    info.starvedPeople = std::ceil(static_cast<float>(insufficientWheat) / WheatConsumptionAsFood);
    info.starvedPercent = static_cast<float>(info.starvedPeople) / resources.population;

    resources.population -= info.starvedPeople;
}

void WelcomeNewComers(Resources &resources, YearInfo &info)
{
    int newComers = info.starvedPeople / 2 + (5 - info.harvestedBushelsPerAcre) * resources.wheat / 600 + 1;
    newComers = std::min(std::max(newComers, MinNewComers), MaxNewComers);
    info.arrivedPeople = newComers;

    resources.population += newComers;
}

void Plague(Resources &resources, YearInfo &info)
{
    std::uniform_real_distribution<float> plagueDistribution(0.0f, 0.999999f);
    info.wasPlague = plagueDistribution(mt) < PlagueChance;
    if (info.wasPlague)
        resources.population /= 2;
}

int ProcessTradingPrice()
{
    std::uniform_int_distribution<int> tradingPriceDistribution(MinTradingPrice, MaxTradingPrice);
    auto tradingPrice = tradingPriceDistribution(mt);

    std::cout << "Land is trading at " << tradingPrice << " bushels in store." << std::endl;

    return tradingPrice;
}
