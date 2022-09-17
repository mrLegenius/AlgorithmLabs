#include "OrderProcessor.h"

#include <iostream>
#include <limits>
#include <functional>

static constexpr float WheatConsumptionAsSeed = 0.5f;
static constexpr int MaxAcresPerPerson = 10;

void ProcessAcresPurchase(int tradingPrice, Resources &resources, PlayerOrders &orders);
void ProcessAcresSell(int tradingPrice, Resources &resources, PlayerOrders &orders);
void ProcessPopulationFeeding(Resources &resources, PlayerOrders &orders);

void ProcessAcresPlanting(Resources &resources, PlayerOrders &orders);

void ignoreLine()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
int AskSafely(const std::string& message, const std::function<bool(int)>& validation, const std::function<void()>& onValidationFailed)
{
    int value{};
    while (true)
    {
        std::cout << message;
        std::cin >> value;

        if (!std::cin || value < 0)
        {
            std::cin.clear();
            ignoreLine();
            std::cerr << "I can't do what you wish. Get yourself another steward!!!!!" << std::endl;
        }
        else
        {
            ignoreLine();

            bool okay = validation(value);

            if (!okay)
            {
                onValidationFailed();
                continue;
            }

            return value;
        }
    }
}

PlayerOrders ProcessOrders(int tradingPrice, Resources& resources)
{
    PlayerOrders orders{};

    ProcessAcresPurchase(tradingPrice, resources, orders);

    if (orders.acresToBuy == 0)
        ProcessAcresSell(tradingPrice, resources, orders);

    ProcessPopulationFeeding(resources, orders);
    ProcessAcresPlanting(resources, orders);

    std::cout << std::endl;
    return orders;
}

void ProcessAcresPlanting(Resources &resources, PlayerOrders &orders)
{
    orders.acresToPlant = AskSafely(
            "How many acres do you wish to plant with seed? ",
            [&resources](int value)
            {
                return value <= resources.acres
                       && value <= resources.population * MaxAcresPerPerson;
            },
            [&resources]()
            {
                std::cout << "Think again. You have only "
                          << resources.acres
                          << " acres and "
                          << resources.population
                          << " people to tend the fields! Now then,"
                          << std::endl;
            });

    resources.wheat -= orders.acresToPlant * WheatConsumptionAsSeed;
}
void ProcessPopulationFeeding(Resources &resources, PlayerOrders &orders)
{
    orders.wheatToFeed = AskSafely(
            "How many bushels do you wish to feed your people? ",
            [&resources](int value)
            {
                return value <= resources.wheat;
            },
            [&resources]()
            {
                std::cout << "Think again. You have only " << resources.wheat << " bushels. Now then,"
                          << std::endl;
            });

    resources.wheat -= orders.wheatToFeed;
}
void ProcessAcresSell(int tradingPrice, Resources &resources, PlayerOrders &orders)
{
    orders.acresToSell = AskSafely(
            "How many acres do you wish to sell? ",
            [&resources](int value)
            {
                return value <= resources.acres;
            },
            [&resources]()
            {
                std::cout << "Think again. You have only "
                          << resources.acres
                          << " acres. Now then,"
                          << std::endl;
            });

    resources.wheat += orders.acresToSell * tradingPrice;
    resources.acres -= orders.acresToSell;
}
void ProcessAcresPurchase(int tradingPrice, Resources &resources, PlayerOrders &orders)
{
    orders.acresToBuy = AskSafely(
            "How many acres do you wish to buy? ",
            [tradingPrice, &resources](int value)
            {
                int purchasePrice = value * tradingPrice;
                return purchasePrice <= resources.wheat;
            },
            [&resources]()
            {
                std::cout << "Think again. You have only " << resources.wheat
                          << " bushels. Now then," << std::endl;
            });

    resources.wheat -= orders.acresToBuy * tradingPrice;
    resources.acres += orders.acresToBuy;
}
