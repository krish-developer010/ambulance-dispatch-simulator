#include "../include/HospitalManager.h"

using namespace std;

bool HospitalManager::addHospital(const Hospital& hospital)
{
    if (hospital.id.empty() ||
        hospital.location < 0 ||
        hospital.totalBeds < 0 ||
        hospital.occupiedBeds < 0 ||
        hospital.occupiedBeds > hospital.totalBeds ||
        hospitalIndex.find(hospital.id) != hospitalIndex.end())
    {
        return false;
    }

    hospitalIndex[hospital.id] = hospitals.size();
    hospitals.push_back(hospital);
    return true;
}

Hospital* HospitalManager::findHospital(const string& id)
{
    auto it = hospitalIndex.find(id);

    if (it == hospitalIndex.end())
    {
        return nullptr;
    }

    return &hospitals[it->second];
}

const Hospital* HospitalManager::findHospital(const string& id) const
{
    auto it = hospitalIndex.find(id);

    if (it == hospitalIndex.end())
    {
        return nullptr;
    }

    return &hospitals[it->second];
}

bool HospitalManager::admitPatient(const string& id)
{
    Hospital* hospital = findHospital(id);

    if (hospital == nullptr || hospital->availableBeds() == 0)
    {
        return false;
    }

    ++hospital->occupiedBeds;
    return true;
}

bool HospitalManager::dischargePatient(const string& id)
{
    Hospital* hospital = findHospital(id);

    if (hospital == nullptr || hospital->occupiedBeds == 0)
    {
        return false;
    }

    --hospital->occupiedBeds;
    return true;
}

const vector<Hospital>& HospitalManager::getAllHospitals() const
{
    return hospitals;
}
