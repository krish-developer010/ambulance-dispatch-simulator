#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <string>

struct Hospital
{
    std::string id;
    int location;
    int totalBeds;
    int occupiedBeds;

    Hospital(std::string hospitalId = "", int hospitalLocation = -1,
             int numberOfBeds = 0, int bedsOccupied = 0)
        : id(hospitalId),
          location(hospitalLocation),
          totalBeds(numberOfBeds),
          occupiedBeds(bedsOccupied)
    {
    }

    int availableBeds() const
    {
        return totalBeds - occupiedBeds;
    }
};

#endif
