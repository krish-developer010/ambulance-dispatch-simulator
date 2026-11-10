#ifndef EMERGENCY_H
#define EMERGENCY_H

#include <string>

struct Emergency
{
    std::string id;
    int location;
    int severity;
    int arrivalTime;
    std::string type;

    Emergency()
        : id(""), location(-1), severity(0), arrivalTime(0), type("")
    {
    }

    Emergency(std::string emergencyId, int emergencyLocation, int emergencySeverity,
              int emergencyArrivalTime, std::string emergencyType)
        : id(emergencyId),
          location(emergencyLocation),
          severity(emergencySeverity),
          arrivalTime(emergencyArrivalTime),
          type(emergencyType)
    {
    }

    bool operator<(const Emergency& other) const
    {
        if (severity != other.severity)
        {
            return severity > other.severity;
        }

        return arrivalTime < other.arrivalTime;
    }
};

#endif
