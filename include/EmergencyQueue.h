#ifndef EMERGENCYQUEUE_H
#define EMERGENCYQUEUE_H

#include <vector>
#include "Emergency.h"
#include "MinHeap.h"

class EmergencyQueue
{
public:
    EmergencyQueue();

    void addEmergency(const Emergency& emergency);
    Emergency peek() const;
    Emergency pop();
    bool empty() const;
    int size() const;
    std::vector<Emergency> toVector() const;

private:
    MinHeap<Emergency> heap;
};

#endif
