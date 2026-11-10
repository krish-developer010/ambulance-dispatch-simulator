#ifndef AMBULANCE_H
#define AMBULANCE_H

#include <string>

enum class AmbulanceStatus
{
    Available,
    Busy,
    Returning
};

struct Ambulance
{
    std::string id;
    int location;
    AmbulanceStatus status;

    Ambulance(std::string ambulanceId = "", int currentLocation = -1,
              AmbulanceStatus currentStatus = AmbulanceStatus::Available)
        : id(ambulanceId),
          location(currentLocation),
          status(currentStatus)
    {
    }
};

#endif
