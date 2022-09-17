#include <iostream>

#include "OrderProcessor.h"
#include "Resources.h"
#include "YearProcessor.h"
#include "SaveLoadService.h"

void PrintYearInfo(YearInfo);
void ProcessResult(float averageStarvedPercent, float acresPerPerson);
void PrintWorstResult();
void FinishGame();

static constexpr int TotalYears = 10;
static constexpr int LastYear = TotalYears + 1;
static constexpr float DeadPercentLose = 0.45f;

int year = 1;
Resources resources;

int main()
{
    YearInfo info{};
    float averageStarvedPercent = 0.0f;

    TryLoad(year, averageStarvedPercent, info, resources);

    PrintYearInfo(info);

    for (; year <= TotalYears;)
    {
        auto tradingPrice = ProcessTradingPrice();

        auto orders = ProcessOrders(tradingPrice, resources);
        info = ProcessYear(orders, resources);
        averageStarvedPercent += info.starvedPercent;

        year++;
        PrintYearInfo(info);

        if (info.starvedPercent > DeadPercentLose)
        {
            std::cout << "You starved " << info.starvedPeople << " in one year!!!" << std::endl;
            PrintWorstResult();
            FinishGame();
        }

        if (year != LastYear && TrySave(year, averageStarvedPercent, info, resources))
        {
            FinishGame();
        }
    }

    averageStarvedPercent /= TotalYears;
    float acresPerPerson = static_cast<float>(resources.acres) / resources.population;
    ProcessResult(averageStarvedPercent, acresPerPerson);

    FinishGame();
}

void PrintYearInfo(YearInfo info)
{
    std::cout << "I beg to report you," << std::endl;

    auto starvedPeopleString = info.starvedPeople ? std::to_string(info.starvedPeople) : "no";
    std::cout << "In year " << year << ", " << starvedPeopleString << " people starved and ";

    auto arrivedPeopleString = info.arrivedPeople ? std::to_string(info.arrivedPeople) : "no one";
    std::cout << arrivedPeopleString << " came to the city." << std::endl;

    if (info.wasPlague)
        std::cout << "A horrible plague struck! Half the people died." << std::endl;

    std::cout << "So the population is now " << resources.population << "." << std::endl;
    std::cout << "The city now owns " << resources.acres << " acres." << std::endl;

    auto harvestedBushelsString = info.harvestedBushels ? std::to_string(info.harvestedBushels) : "no";
    std::cout << "You harvested " << harvestedBushelsString << " bushels. ";
    if (info.harvestedBushels)
        std::cout << info.harvestedBushelsPerAcre << " bushels per acre.";
    std::cout << std::endl;

    auto ratsAteBushelsString = info.bushelsEatenByRats ? std::to_string(info.bushelsEatenByRats) : "no";
    std::cout << "The rats ate " << ratsAteBushelsString << " bushels." << std::endl;
    std::cout << "You now have " << resources.wheat << " bushels in the store." << std::endl;

    std::cout << std::endl;
}

void ProcessResult(float averageStarvedPercent, float acresPerPerson)
{
    if (averageStarvedPercent > 0.33f && acresPerPerson < 7)
    {
        PrintWorstResult();
    }
    else if (averageStarvedPercent > 0.10f && acresPerPerson < 9)
    {
        std::cout << "Your heavy-handed performance shacks of hero and Ivan IV." << std::endl;
        std::cout << "The people (remaining) find you an unpleasant ruler, and," << std::endl;
        std::cout << "Frankly, hate your huts!!" << std::endl;
    }
    else if (averageStarvedPercent > 0.03f && acresPerPerson < 10)
    {
        std::cout << "Your performance could have been somewhat better, but" << std::endl;
        std::cout << "really wasn't too bad at all. " << static_cast<int>(resources.population * 0.8f);
        std::cout << "people" << std::endl;
        std::cout << "dearly like to see you assassinated but we all have our" << std::endl;
        std::cout << "trivial problems" << std::endl;
    }
    else
    {
        std::cout << "A fantastic performance!!! Charlemagne, Disraeli, and " << std::endl;
        std::cout << "Jefferson combined could not have done better!" << std::endl;
    }
}

void PrintWorstResult()
{
    std::cout << "Due to this extreme mismanagement" << std::endl;
    std::cout << "you have not only been impeached and throw out of office" << std::endl;
    std::cout << "but you have also been declared national fink!!!!" << std::endl;
}

void FinishGame()
{
    std::cout << std::endl << "So long for now." << std::endl;
    while (!getchar());
    exit(0);
}
