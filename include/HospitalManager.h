#ifndef HOSPITALMANAGER_H
#define HOSPITALMANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "Hospital.h"

class HospitalManager
{
public:
    bool addHospital(const Hospital& hospital);
    Hospital* findHospital(const std::string& id);
    const Hospital* findHospital(const std::string& id) const;

    bool admitPatient(const std::string& id);
    bool dischargePatient(const std::string& id);

    const std::vector<Hospital>& getAllHospitals() const;

private:
    std::vector<Hospital> hospitals;
    std::unordered_map<std::string, size_t> hospitalIndex;
};

#endif
