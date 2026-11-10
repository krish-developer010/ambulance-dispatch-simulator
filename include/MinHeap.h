#ifndef MINHEAP_H
#define MINHEAP_H

#include <vector>
#include <functional>
#include <stdexcept>
#include <utility>

template <typename T>
class MinHeap
{
private:
    std::vector<T> heap;
    std::function<bool(const T&, const T&)> compare;

    void heapifyUp(int index)
    {
        while (index > 0)
        {
            int parent = (index - 1) / 2;

            if (compare(heap[index], heap[parent]))
            {
                std::swap(heap[index], heap[parent]);
                index = parent;
            }
            else
            {
                break;
            }
        }
    }

    void heapifyDown(int index)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < heap.size() && compare(heap[left], heap[smallest]))
        {
            smallest = left;
        }

        if (right < heap.size() && compare(heap[right], heap[smallest]))
        {
            smallest = right;
        }

        if (smallest != index)
        {
            std::swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    MinHeap(std::function<bool(const T&, const T&)> customCompare =
                [](const T& a, const T& b)
                {
                    return a < b;
                })
        : compare(customCompare)
    {
    }

    void insert(const T& value)
    {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    bool empty() const
    {
        return heap.empty();
    }

    int size() const
    {
        return static_cast<int>(heap.size());
    }

    T peek() const
    {
        if (empty())
        {
            throw std::out_of_range("MinHeap is empty.");
        }

        return heap[0];
    }

    T extractMin()
    {
        if (empty())
        {
            throw std::out_of_range("MinHeap is empty.");
        }

        T minimum = heap[0];

        heap[0] = heap.back();
        heap.pop_back();

        if (!empty())
        {
            heapifyDown(0);
        }

        return minimum;
    }

    void clear()
    {
        heap.clear();
    }

    void removeAt(int index)
    {
        if (index < 0 || index >= size())
        {
            throw std::out_of_range("Invalid heap index.");
        }

        heap[index] = heap.back();
        heap.pop_back();

        if (index < heap.size())
        {
            if (index > 0 && compare(heap[index], heap[(index - 1) / 2]))
            {
                heapifyUp(index);
            }
            else
            {
                heapifyDown(index);
            }
        }
    }

    std::vector<T> toVector() const
    {
        return heap;
    }
};

#endif
