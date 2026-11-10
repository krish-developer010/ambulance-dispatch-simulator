#ifndef AMBULANCEMANAGER_H
#define AMBULANCEMANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "Ambulance.h"

class AmbulanceManager
{
public:
    bool addAmbulance(const Ambulance& ambulance);
    Ambulance* findAmbulance(const std::string& id);
    const Ambulance* findAmbulance(const std::string& id) const;

    bool setStatus(const std::string& id, AmbulanceStatus status);
    bool updateLocation(const std::string& id, int newLocation);

    std::vector<Ambulance> getAvailableAmbulances() const;
    const std::vector<Ambulance>& getAllAmbulances() const;

private:
    std::vector<Ambulance> ambulances;
    std::unordered_map<std::string, size_t> ambulanceIndex;
};

std::string ambulanceStatusToString(AmbulanceStatus status);

#endif
