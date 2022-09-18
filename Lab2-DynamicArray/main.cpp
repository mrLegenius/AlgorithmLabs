#include <iostream>
#include "Array.h"

int main()
{
    Array<int> a;

    for (int i = 0; i < 4; ++i)
        a.insert(i + 1);

    for (auto it = a.begin(); it != a.end(); ++it)
        it.set(it.get() * 2);

    std::cout << "Reversed A: " << std::endl;
    for (auto it = a.crbegin(); it != a.crend(); ++it)
        std::cout << it.get() << " ";
    std::cout << std::endl << std::endl;

    for (int i = 0; i < 4; ++i)
        a.insert(2, i + 1000);

    a.remove(0);
    a.remove(2);

    Array<int> b = a;
    std::cout << "Array A: " << std::endl;
    std::cout << "Size: " << a.size() << std::endl;
    std::cout << "Capacity: " << a.capacity() << std::endl;

    for (auto it = a.iterator(); it.hasNext(); it.next())
        std::cout << it.get() << " ";
    std::cout << std::endl << std::endl;

    std::cout << "Reversed B: " << std::endl;
    std::cout << "Size: " << b.size() << std::endl;
    std::cout << "Capacity: " << b.capacity() << std::endl;
    for (auto it = b.reverseIterator(); it.hasNext(); it.next())
        std::cout << it.get() << " ";
    std::cout << std::endl << std::endl;

    while (!getchar());
    return 0;
}
