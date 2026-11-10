#include <cassert>
#include <iostream>

#include "../include/AmbulanceManager.h"
#include "../include/DispatchEngine.h"
#include "../include/EmergencyQueue.h"
#include "../include/Graph.h"
#include "../include/HospitalManager.h"
#include "../include/Reports.h"
#include "../include/RoutingEngine.h"

using namespace std;

int main()
{
    Graph g(6);

    g.addRoad(0, 1, 4);
    g.addRoad(0, 2, 2);
    g.addRoad(1, 2, 1);
    g.addRoad(1, 3, 5);
    g.addRoad(2, 3, 8);
    g.addRoad(2, 4, 10);
    g.addRoad(3, 4, 2);
    g.addRoad(3, 5, 6);
    g.addRoad(4, 5, 3);

    RoutingEngine router;

    vector<int> path = router.shortestPath(g, 0, 5);
    cout << "Shortest path from 0 to 5: ";
    for (int node : path)
    {
        cout << node << " ";
    }
    cout << endl;

    cout << "Distance from 0 to 5: " << router.distanceTo(g, 0, 5) << endl;
    cout << "Reachable from 0 to 5? "
         << (router.isReachable(g, 0, 5) ? "Yes" : "No") << endl;

    g.blockRoad(1, 2);
    cout << "Reachable from 0 to 5 after road block? "
         << (router.isReachable(g, 0, 5) ? "Yes" : "No") << endl;

    EmergencyQueue queue;
    queue.addEmergency(Emergency("E1", 10, 3, 30, "Traffic Accident"));
    queue.addEmergency(Emergency("E2", 20, 5, 7, "Heart Attack"));
    queue.addEmergency(Emergency("E3", 15, 5, 2, "Stroke"));
    queue.addEmergency(Emergency("E4", 25, 1, 4, "Minor Injury"));

    cout << "Emergency processing order:\n";
    while (!queue.empty())
    {
        Emergency e = queue.pop();
        cout << e.id << " severity=" << e.severity
             << " arrival=" << e.arrivalTime << "\n";
    }

    AmbulanceManager ambulanceManager;
    assert(ambulanceManager.addAmbulance(Ambulance("A1", 0, AmbulanceStatus::Available)));
    assert(ambulanceManager.addAmbulance(Ambulance("A2", 5, AmbulanceStatus::Busy)));
    assert(!ambulanceManager.addAmbulance(Ambulance("A1", 1, AmbulanceStatus::Available)));
    assert(!ambulanceManager.addAmbulance(Ambulance("A3", -1, AmbulanceStatus::Available)));

    HospitalManager hospitalManager;
    assert(hospitalManager.addHospital(Hospital("H1", 4, 2)));
    assert(!hospitalManager.addHospital(Hospital("H1", 3, 5)));
    assert(!hospitalManager.addHospital(Hospital("H2", 12, 1, 2)));

    assert(hospitalManager.admitPatient("H1"));
    assert(hospitalManager.admitPatient("H1"));
    assert(!hospitalManager.admitPatient("H1"));

    Graph graph(6);
    graph.addRoad(0, 1, 2);
    graph.addRoad(1, 2, 3);
    graph.addRoad(2, 3, 2);
    graph.addRoad(3, 4, 4);
    graph.addRoad(4, 5, 1);
    graph.addRoad(1, 5, 10);

    RoutingEngine routingEngine2;
    EmergencyQueue emergencyQueue2;

    emergencyQueue2.addEmergency(Emergency("E1", 3, 2, 1, "Minor injury"));
    emergencyQueue2.addEmergency(Emergency("E2", 2, 5, 2, "Critical"));

    DispatchEngine dispatchEngine(
        graph,
        routingEngine2,
        emergencyQueue2,
        ambulanceManager,
        hospitalManager);

    assert(dispatchEngine.dispatchNext());
    assert(emergencyQueue2.size() == 1);

    const vector<DispatchRecord>& history = dispatchEngine.getHistory();
    assert(history.size() == 1);
    assert(history[0].emergency.id == "E2");
    assert(history[0].responseTime == 5);

    assert(Reports::averageResponseTime(dispatchEngine.getHistory()) == 5.0);

    cout << "All tests passed.\n";
    return 0;
}
