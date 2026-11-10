#include "../include/EmergencyQueue.h"

using namespace std;

EmergencyQueue::EmergencyQueue()
    : heap([](const Emergency& a, const Emergency& b)
           {
               if (a.severity != b.severity)
               {
                   return a.severity > b.severity;
               }

               return a.arrivalTime < b.arrivalTime;
           })
{
}

void EmergencyQueue::addEmergency(const Emergency& emergency)
{
    heap.insert(emergency);
}

Emergency EmergencyQueue::peek() const
{
    return heap.peek();
}

Emergency EmergencyQueue::pop()
{
    return heap.extractMin();
}

bool EmergencyQueue::empty() const
{
    return heap.empty();
}

int EmergencyQueue::size() const
{
    return heap.size();
}

vector<Emergency> EmergencyQueue::toVector() const
{
    return heap.toVector();
}
