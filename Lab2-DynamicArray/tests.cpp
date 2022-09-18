#include "gtest/gtest.h"
#include "Array.h"

TEST(ArrayTest, DefaultConstructor)
{
    Array<int> a;
    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(a.capacity(), 8);
}

TEST(ArrayTest, CapacityConstructor)
{
    Array<int> a{20};

    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(a.capacity(), 20);
}

TEST(ArrayTest, CopyConstructor)
{
    Array<int> a{};
    a.insert(5);
    a.insert(6);
    a.insert(7);

    Array<int> b{a};

    ASSERT_EQ(a.size(), b.size());
    ASSERT_EQ(a.capacity(), b.capacity());
    for (int i = 0; i < a.size(); ++i)
    {
        EXPECT_EQ(a[i], b[i]);
    }
}

TEST(ArrayTest, MoveConstructor)
{
    Array<int> a{};
    a.insert(5);
    a.insert(6);
    a.insert(7);

    Array<int> b = std::move(a);

    ASSERT_EQ(b.size(), 3);
    EXPECT_EQ(b[0], 5);
    EXPECT_EQ(b[1], 6);
    EXPECT_EQ(b[2], 7);
}

TEST(ArrayTest, InsertToEnd)
{
    Array<int> a{};

    a.insert(5);

    ASSERT_EQ(a.size(), 1);
    EXPECT_EQ(a[0], 5);
}

TEST(ArrayTest, InsertToMiddle)
{
    Array<int> a{};

    a.insert(1);
    a.insert(2);
    a.insert(3);
    a.insert(1, 4);

    ASSERT_EQ(a.size(), 4);
    EXPECT_EQ(a[1], 4);
}

TEST(ArrayTest, RemoveAtBegin)
{
    Array<int> a{};
    a.insert(1);
    a.insert(2);
    a.insert(3);

    a.remove(0);

    ASSERT_EQ(a.size(), 2);
    EXPECT_EQ(a[0], 2);
    EXPECT_EQ(a[1], 3);
}

TEST(ArrayTest, RemoveAtMiddle)
{
    Array<int> a{};
    a.insert(1);
    a.insert(2);
    a.insert(3);

    a.remove(1);

    ASSERT_EQ(a.size(), 2);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 3);
}

TEST(ArrayTest, RemoveAtEnd)
{
    Array<int> a{};
    a.insert(1);
    a.insert(2);
    a.insert(3);

    a.remove(a.size() - 1);

    ASSERT_EQ(a.size(), 2);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
}

TEST(ArrayTest, Iterator)
{
    Array<int> a{};
    a.insert(1);
    a.insert(2);
    a.insert(3);

    int c = 1;

    for (auto it = a.iterator(); it.hasNext(); it.next())
        EXPECT_EQ(it.get(), c++);
}

TEST(ArrayTest, ReverseIterator)
{
    Array<int> a{};
    a.insert(1);
    a.insert(2);
    a.insert(3);

    int c = 3;

    for (auto it = a.reverseIterator(); it.hasNext(); it.next())
        EXPECT_EQ(it.get(), c--);
}