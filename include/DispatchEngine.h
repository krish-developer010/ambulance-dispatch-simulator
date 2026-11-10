#ifndef DISPATCHENGINE_H
#define DISPATCHENGINE_H

#include <string>
#include <vector>

#include "AmbulanceManager.h"
#include "EmergencyQueue.h"
#include "Graph.h"
#include "HospitalManager.h"
#include "RoutingEngine.h"

struct DispatchRecord
{
    Emergency emergency;
    std::string ambulanceId;
    std::string hospitalId;

    std::vector<int> ambulanceToIncidentRoute;
    std::vector<int> incidentToHospitalRoute;

    int ambulanceToIncidentDistance;
    int incidentToHospitalDistance;
    int responseTime;

    bool patientDelivered;
    bool ambulanceReturned;

    DispatchRecord()
        : ambulanceToIncidentDistance(-1),
          incidentToHospitalDistance(-1),
          responseTime(-1),
          patientDelivered(false),
          ambulanceReturned(false)
    {
    }
};

class DispatchEngine
{
public:
    DispatchEngine(Graph& graph,
                   RoutingEngine& routingEngine,
                   EmergencyQueue& emergencyQueue,
                   AmbulanceManager& ambulanceManager,
                   HospitalManager& hospitalManager);

    bool dispatchNext();
    bool completeTrip(const std::string& emergencyId);
    bool finishReturn(const std::string& ambulanceId, int returnLocation);
    const std::vector<DispatchRecord>& getHistory() const;

private:
    Graph& graph;
    RoutingEngine& routingEngine;
    EmergencyQueue& emergencyQueue;
    AmbulanceManager& ambulanceManager;
    HospitalManager& hospitalManager;

    std::vector<DispatchRecord> history;
};

#endif
