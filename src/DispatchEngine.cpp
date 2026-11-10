#include "../include/DispatchEngine.h"

#include <limits>

using namespace std;

DispatchEngine::DispatchEngine(Graph& graph,
                               RoutingEngine& routingEngine,
                               EmergencyQueue& emergencyQueue,
                               AmbulanceManager& ambulanceManager,
                               HospitalManager& hospitalManager)
    : graph(graph),
      routingEngine(routingEngine),
      emergencyQueue(emergencyQueue),
      ambulanceManager(ambulanceManager),
      hospitalManager(hospitalManager)
{
}

bool DispatchEngine::dispatchNext()
{
    if (emergencyQueue.empty())
    {
        return false;
    }

    Emergency emergency = emergencyQueue.peek();

    const Ambulance* selectedAmbulance = nullptr;
    vector<int> selectedAmbulanceRoute;
    int bestAmbulanceDistance = numeric_limits<int>::max();

    vector<Ambulance> availableAmbulances = ambulanceManager.getAvailableAmbulances();

    for (const Ambulance& ambulance : availableAmbulances)
    {
        if (!graph.hasVertex(ambulance.location) || !graph.hasVertex(emergency.location))
        {
            continue;
        }

        int distance = routingEngine.distanceTo(graph, ambulance.location, emergency.location);

        if (distance < 0)
        {
            continue;
        }

        if (distance < bestAmbulanceDistance ||
            (distance == bestAmbulanceDistance &&
             (selectedAmbulance == nullptr || ambulance.id < selectedAmbulance->id)))
        {
            vector<int> route = routingEngine.shortestPath(graph, ambulance.location, emergency.location);

            if (!route.empty())
            {
                selectedAmbulance = ambulanceManager.findAmbulance(ambulance.id);
                selectedAmbulanceRoute = route;
                bestAmbulanceDistance = distance;
            }
        }
    }

    if (selectedAmbulance == nullptr)
    {
        return false;
    }

    const Hospital* selectedHospital = nullptr;
    vector<int> selectedHospitalRoute;
    int bestHospitalDistance = numeric_limits<int>::max();

    const vector<Hospital>& hospitals = hospitalManager.getAllHospitals();

    for (const Hospital& hospital : hospitals)
    {
        if (hospital.availableBeds() <= 0 || !graph.hasVertex(hospital.location))
        {
            continue;
        }

        int distance = routingEngine.distanceTo(graph, emergency.location, hospital.location);

        if (distance < 0)
        {
            continue;
        }

        if (distance < bestHospitalDistance ||
            (distance == bestHospitalDistance &&
             (selectedHospital == nullptr || hospital.id < selectedHospital->id)))
        {
            vector<int> route = routingEngine.shortestPath(graph, emergency.location, hospital.location);

            if (!route.empty())
            {
                selectedHospital = hospitalManager.findHospital(hospital.id);
                selectedHospitalRoute = route;
                bestHospitalDistance = distance;
            }
        }
    }

    if (selectedHospital == nullptr)
    {
        return false;
    }

    if (!hospitalManager.admitPatient(selectedHospital->id))
    {
        return false;
    }

    if (!ambulanceManager.setStatus(selectedAmbulance->id, AmbulanceStatus::Busy))
    {
        hospitalManager.dischargePatient(selectedHospital->id);
        return false;
    }

    DispatchRecord record;
    record.emergency = emergency;
    record.ambulanceId = selectedAmbulance->id;
    record.hospitalId = selectedHospital->id;
    record.ambulanceToIncidentRoute = selectedAmbulanceRoute;
    record.incidentToHospitalRoute = selectedHospitalRoute;
    record.ambulanceToIncidentDistance = bestAmbulanceDistance;
    record.incidentToHospitalDistance = bestHospitalDistance;
    record.responseTime = bestAmbulanceDistance;

    emergencyQueue.pop();
    history.push_back(record);

    return true;
}

bool DispatchEngine::completeTrip(const string& emergencyId)
{
    for (int i = static_cast<int>(history.size()) - 1; i >= 0; --i)
    {
        DispatchRecord& record = history[i];

        if (record.emergency.id != emergencyId || record.patientDelivered)
        {
            continue;
        }

        Ambulance* ambulance = ambulanceManager.findAmbulance(record.ambulanceId);

        if (ambulance == nullptr)
        {
            return false;
        }

        if (!ambulanceManager.updateLocation(record.ambulanceId,
                hospitalManager.findHospital(record.hospitalId)->location))
        {
            return false;
        }

        if (!ambulanceManager.setStatus(record.ambulanceId, AmbulanceStatus::Returning))
        {
            return false;
        }

        record.patientDelivered = true;
        return true;
    }

    return false;
}

bool DispatchEngine::finishReturn(const string& ambulanceId, int returnLocation)
{
    if (!graph.hasVertex(returnLocation))
    {
        return false;
    }

    for (int i = static_cast<int>(history.size()) - 1; i >= 0; --i)
    {
        DispatchRecord& record = history[i];

        if (record.ambulanceId != ambulanceId || !record.patientDelivered || record.ambulanceReturned)
        {
            continue;
        }

        Ambulance* ambulance = ambulanceManager.findAmbulance(ambulanceId);

        if (ambulance == nullptr || ambulance->status != AmbulanceStatus::Returning)
        {
            return false;
        }

        if (!ambulanceManager.updateLocation(ambulanceId, returnLocation) ||
            !ambulanceManager.setStatus(ambulanceId, AmbulanceStatus::Available))
        {
            return false;
        }

        record.ambulanceReturned = true;
        return true;
    }

    return false;
}

const vector<DispatchRecord>& DispatchEngine::getHistory() const
{
    return history;
}
