#include <iostream>
#include "Sort.h"
#include "Array.h"

int main()
{
    const int size = 10;

    srand(time(nullptr));
    int a[size];
    for (int& i : a)
        i = rand() % 100;

    std::cout << "Initial array: " << std::endl;
    for (int& i : a)
        std::cout << i << " ";
    std::cout << std::endl;

    sort(a, a + size, [](int a, int b)
    { return a < b; });

    std::cout << "Sorted array: " << std::endl;
    for (int& i : a)
        std::cout << i << " ";
    std::cout << std::endl;

    std::cout << std::endl;

struct test
{
    int a;
    float b;
};
    Array<test> array;
    for (int i = 0; i < size; ++i)
    {
        array.insert(test{rand() % 100, rand() % 100 + .5f});
    }

    std::cout << "Initial array: " << std::endl;
    for (auto& i : array)
        std::cout << "(" << i.a << ", " << i.b << ") ";

    std::cout << std::endl;
    sort(array.begin(), array.end(), [](auto a, auto b){ return a.a < b.a; });
    sort(array.begin(), array.end(), [](auto a, auto b){ return a.b < b.b; });

    std::cout << "Sorted array: " << std::endl;
    for (auto& i : array)
        std::cout << "(" << i.a << ", " << i.b << ") ";
    std::cout << std::endl;

    while (!getchar());
    return 0;
}
