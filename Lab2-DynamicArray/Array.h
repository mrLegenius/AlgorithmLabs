#pragma once

#include <algorithm>
#include <iostream>

template<typename T>
class Array final
{
private:
    template<bool Const, bool Reverse>
    class Iterator
    {
        using Reference = std::conditional_t<Const, const T&, T&>;
        using ArrayReference = std::conditional_t<Const, const Array&, Array&>;
    public:
        explicit Iterator(ArrayReference array, int position)
                : m_Array(array),
                  m_Position(Reverse ? array.m_Size - 1 - position : position)
        {}

        [[nodiscard]] Reference get() const { return m_Array[m_Position]; }

        void set(const T& value)
        {
            if constexpr (!Const)
            {
                auto& data = const_cast<T&>(m_Array[m_Position]);
                data = value;
            }
            else
            {
                throw std::runtime_error("Trying to set const iterator");
            }
        }

        void next()
        {
            if (!hasNext()) throw std::out_of_range("Iterator out of range");

            if constexpr (Reverse)
                --m_Position;
            else
                ++m_Position;
        }

        [[nodiscard]] bool hasNext() const
        {
            if constexpr (Reverse)
                return m_Position >= 0;
            else
                return m_Position < m_Array.m_Size;
        }

        bool operator==(const Iterator& it) { return (m_Position == it.m_Position); }

        bool operator!=(const Iterator& it) { return (m_Position != it.m_Position); }

        friend Iterator operator+(const Iterator& it, int value)
        {
            Iterator i = it;
            i.m_Position += value;
            return i;
        }

        friend Iterator operator-(const Iterator& it, int value)
        {
            Iterator i = it;
            i.m_Position -= value;
            return i;
        }

        Iterator& operator++()
        {
            if constexpr (Reverse)
                m_Position--;
            else
                m_Position++;

            return *this;
        }

        Iterator& operator++(int)
        {
            if constexpr (Reverse)
                m_Position--;
            else
                m_Position++;

            return *this;
        }

        Iterator& operator+=(int value)
        {
            m_Position += value;
            return *this;
        }

        Iterator& operator-=(int value)
        {
            m_Position -= value;
            return *this;
        }

        Reference operator*() { return m_Array[m_Position]; }

    private:
        ArrayReference m_Array;
        int m_Position = 0;
    };

public:
    Array() : Array(defaultSize)
    {}

    explicit Array(int capacity)
            : m_Capacity(capacity)
    {
        if (m_Capacity)
            m_Data = alloc(m_Capacity);
    }

    Array(const Array& other)
            : m_Capacity(other.m_Capacity),
              m_Size(other.m_Size)
    {
        m_Data = alloc(m_Capacity);

        for (int i = 0; i < m_Size; ++i)
            new(m_Data + i) T{other.m_Data[i]};
    }

    Array(Array&& other) noexcept
    {
        m_Capacity = other.m_Capacity;
        m_Size = other.m_Size;
        m_Data = other.m_Data;

        other.m_Data = nullptr;
        other.m_Size = 0;
        other.m_Capacity = 0;
    }

    ~Array()
    {
        for (int i = 0; i < m_Size; i++)
            (&((T*)m_Data)[i])->~T();

        std::free(m_Data);
    }

    int insert(const T& value)
    {
        return insert(m_Size, value);
    }

    int insert(int index, const T& value)
    {
        if (m_Size == m_Capacity)
            resize();

        std::move(m_Data + index, m_Data + m_Size, m_Data + index + 1);

        new(m_Data + index) T{value};
        m_Size++;

        return index;
    }

    void remove(int index)
    {
        (&((T*)m_Data)[index])->~T();
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
        m_Capacity = other.m_Capacity;
        m_Size = other.m_Size;
        m_Data = other.m_Data;

        other.m_Data = nullptr;
        other.m_Size = 0;
        other.m_Capacity = 0;
        return *this;
    }

    Iterator<false, false> iterator() { return Iterator<false, false>(*this, 0); }
    Iterator<true, false> iterator() const { return Iterator<true, false>(*this, 0); }

    Iterator<false, true> reverseIterator() { return Iterator<false, true>(*this, 0); }
    Iterator<true, true> reverseIterator() const { return Iterator<true, true>(*this, 0); }

    Iterator<false, false> begin() { return Iterator<false, false>(*this, 0); }
    Iterator<false, false> end() { return Iterator<false, false>(*this, m_Size); }

    Iterator<true, false> cbegin() const { return Iterator<true, false>(*this, 0); }
    Iterator<true, false> cend() const { return Iterator<true, false>(*this, m_Size); }

    Iterator<false, true> rbegin() { return Iterator<false, true>(*this, 0); }
    Iterator<false, true> rend() { return Iterator<false, true>(*this, m_Size); }

    Iterator<true, true> crbegin() const { return Iterator<true, true>(*this, 0); }
    Iterator<true, true> crend() const { return Iterator<true, true>(*this, m_Size); }

private:
    static constexpr int defaultSize = 8;
    static constexpr int resizeFactor = 2;
    int m_Size = 0;
    int m_Capacity = 0;
    T* m_Data = nullptr;

    void resize()
    {
        if (m_Capacity == 0)
        {
            m_Capacity = defaultSize;
            m_Data = alloc(m_Capacity);
            return;
        }

        m_Capacity *= resizeFactor;

        T* temp = alloc(m_Capacity);
        for (int i = 0; i < m_Size; ++i)
            new(temp + i) T{m_Data[i]};

        free(m_Data);
        m_Data = temp;
    }

    T* alloc(size_t size) { return (T*)std::malloc(size * sizeof(T)); }
};
