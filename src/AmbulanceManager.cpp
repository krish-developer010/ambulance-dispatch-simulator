#include "../include/AmbulanceManager.h"

using namespace std;

bool AmbulanceManager::addAmbulance(const Ambulance& ambulance)
{
    if (ambulance.id.empty() || ambulance.location < 0 ||
        ambulanceIndex.find(ambulance.id) != ambulanceIndex.end())
    {
        return false;
    }

    ambulanceIndex[ambulance.id] = ambulances.size();
    ambulances.push_back(ambulance);
    return true;
}

Ambulance* AmbulanceManager::findAmbulance(const string& id)
{
    auto it = ambulanceIndex.find(id);

    if (it == ambulanceIndex.end())
    {
        return nullptr;
    }

    return &ambulances[it->second];
}

const Ambulance* AmbulanceManager::findAmbulance(const string& id) const
{
    auto it = ambulanceIndex.find(id);

    if (it == ambulanceIndex.end())
    {
        return nullptr;
    }

    return &ambulances[it->second];
}

bool AmbulanceManager::setStatus(const string& id, AmbulanceStatus status)
{
    Ambulance* ambulance = findAmbulance(id);

    if (ambulance == nullptr)
    {
        return false;
    }

    ambulance->status = status;
    return true;
}

bool AmbulanceManager::updateLocation(const string& id, int newLocation)
{
    if (newLocation < 0)
    {
        return false;
    }

    Ambulance* ambulance = findAmbulance(id);

    if (ambulance == nullptr)
    {
        return false;
    }

    ambulance->location = newLocation;
    return true;
}

vector<Ambulance> AmbulanceManager::getAvailableAmbulances() const
{
    vector<Ambulance> available;

    for (const Ambulance& ambulance : ambulances)
    {
        if (ambulance.status == AmbulanceStatus::Available)
        {
            available.push_back(ambulance);
        }
    }

    return available;
}

const vector<Ambulance>& AmbulanceManager::getAllAmbulances() const
{
    return ambulances;
}

string ambulanceStatusToString(AmbulanceStatus status)
{
    switch (status)
    {
    case AmbulanceStatus::Available:
        return "Available";
    case AmbulanceStatus::Busy:
        return "Busy";
    case AmbulanceStatus::Returning:
        return "Returning";
    }

    return "Unknown";
}
