#pragma once

#include <algorithm>
#include <iostream>

template<typename T>
class Array final
{
public:
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Array() : Array(defaultSize)
    {}

    explicit Array(int capacity)
            : m_Capacity(capacity)
    {
        if (m_Capacity)
            m_Data = (T*)malloc(m_Capacity * sizeof(T));
    }

    Array(const Array& other)
            : m_Capacity(other.m_Capacity),
              m_Size(other.m_Size)
    {
        if (m_Capacity)
            m_Data = (T*)malloc(m_Capacity * sizeof(T));

        std::copy_n(other.m_Data, m_Capacity, m_Data);
    }

    Array(Array&& other) noexcept
            : Array()
    {
        swap(*this, other);
    }

    ~Array()
    { free(m_Data); }

    int insert(const T& value)
    {
        if (m_Size == m_Capacity)
            resize();

        m_Data[m_Size] = value;
        m_Size++;

        return m_Size - 1;
    }

    int insert(int index, const T& value)
    {
        if (m_Size == m_Capacity)
            resize();

        std::move(m_Data + index, m_Data + m_Size, m_Data + index + 1);

        m_Data[index] = value;
        m_Size++;

        return index;
    }

    void remove(int index)
    {
        std::move(m_Data + index + 1, m_Data + m_Size, m_Data + index);
        m_Size--;
    }

    [[nodiscard]] int size() const
    { return m_Size; }

    [[nodiscard]] int capacity() const
    { return m_Capacity; }

    const T& operator[](int index) const
    { return m_Data[index]; };

    T& operator[](int index)
    { return m_Data[index]; };

    Array& operator=(Array other)
    {
        swap(*this, other);
        return *this;
    }

    iterator begin() noexcept
    {
        return m_Data;
    }

    iterator end() noexcept
    {
        return m_Data + m_Size;
    }

    const_iterator cbegin() const noexcept
    {
        return m_Data;
    }

    const_iterator cend() const noexcept
    {
        return m_Data + m_Size;
    }

    reverse_iterator rbegin() noexcept
    {
        return std::reverse_iterator(end());
    }

    reverse_iterator rend() noexcept
    {
        return std::reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return std::reverse_iterator(end());
    }

    const_reverse_iterator crend() const noexcept
    {
        return std::reverse_iterator(begin());
    }

private:
    static constexpr int defaultSize = 8;
    static constexpr int resizeFactor = 2;
    int m_Size = 0;
    int m_Capacity = 0;
    T* m_Data = nullptr;

    friend void swap(Array& first, Array& second)
    {
        using std::swap;

        swap(first.m_Size, second.m_Size);
        swap(first.m_Capacity, second.m_Capacity);
        swap(first.m_Data, second.m_Data);
    };

    void resize()
    {
        if (m_Capacity == 0)
        {
            m_Capacity = defaultSize;
            m_Data = (T*)malloc(m_Capacity * sizeof(T));
            return;
        }

        m_Capacity *= resizeFactor;

        T* temp = (T*)malloc(m_Capacity * sizeof(T));

        std::move(m_Data, m_Data + m_Size, temp);

        free(m_Data);
        m_Data = temp;
    }
};
