#include <iostream>
#include <limits>
#include <string>

#include "../include/AmbulanceManager.h"
#include "../include/DispatchEngine.h"
#include "../include/EmergencyQueue.h"
#include "../include/Graph.h"
#include "../include/HospitalManager.h"
#include "../include/Reports.h"
#include "../include/RoutingEngine.h"

using namespace std;

int readInt(const string& prompt)
{
    int value;

    while (true)
    {
        cout << prompt;

        if (cin >> value)
        {
            return value;
        }

        cout << "Please enter a whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readLine(const string& prompt)
{
    string value;
    cout << prompt;
    getline(cin >> ws, value);
    return value;
}

void printRoute(const vector<int>& route)
{
    if (route.empty())
    {
        cout << "No route";
        return;
    }

    for (size_t i = 0; i < route.size(); ++i)
    {
        if (i > 0)
        {
            cout << " -> ";
        }
        cout << route[i];
    }
}

void printDispatchResult(const DispatchEngine& dispatchEngine)
{
    const vector<DispatchRecord>& history = dispatchEngine.getHistory();

    if (history.empty())
    {
        cout << "No dispatch has been recorded.\n";
        return;
    }

    const DispatchRecord& record = history.back();

    cout << "Emergency " << record.emergency.id
         << " assigned to ambulance " << record.ambulanceId << ".\n";

    cout << "Ambulance route: ";
    printRoute(record.ambulanceToIncidentRoute);
    cout << " | distance/time: "
         << record.ambulanceToIncidentDistance << '\n';

    cout << "Hospital route: ";
    printRoute(record.incidentToHospitalRoute);
    cout << " | distance/time: "
         << record.incidentToHospitalDistance << '\n';
}

void printAmbulances(const AmbulanceManager& manager)
{
    const vector<Ambulance>& ambulances = manager.getAllAmbulances();

    cout << "\nAmbulances:\n";
    for (const Ambulance& ambulance : ambulances)
    {
        cout << "  " << ambulance.id
             << " | location " << ambulance.location
             << " | " << ambulanceStatusToString(ambulance.status) << '\n';
    }
}

void printHospitals(const HospitalManager& manager)
{
    const vector<Hospital>& hospitals = manager.getAllHospitals();

    cout << "\nHospitals:\n";
    for (const Hospital& hospital : hospitals)
    {
        cout << "  " << hospital.id
             << " | location " << hospital.location
             << " | available beds " << hospital.availableBeds()
             << "/" << hospital.totalBeds << '\n';
    }
}

void printMenu()
{
    cout << "\n=== Ambulance Dispatch System ===\n"
         << "1. Show city map\n"
         << "2. Register emergency\n"
         << "3. Dispatch highest-priority emergency\n"
         << "4. Show ambulances and hospitals\n"
         << "5. Show reports and dispatch history\n"
         << "6. Block a road\n"
         << "7. Unblock a road\n"
         << "8. Change a road weight\n"
         << "9. Complete patient delivery\n"
         << "10. Finish ambulance return\n"
         << "11. Discharge a patient\n"
         << "0. Exit\n";
}

int main()
{
    Graph graph(6);
    graph.addRoad(0, 1, 2);
    graph.addRoad(1, 2, 3);
    graph.addRoad(2, 3, 2);
    graph.addRoad(3, 4, 4);
    graph.addRoad(4, 5, 1);
    graph.addRoad(1, 5, 10);

    RoutingEngine routingEngine;
    EmergencyQueue emergencyQueue;
    AmbulanceManager ambulanceManager;
    HospitalManager hospitalManager;

    ambulanceManager.addAmbulance(Ambulance("A1", 0, AmbulanceStatus::Available));
    ambulanceManager.addAmbulance(Ambulance("A2", 5, AmbulanceStatus::Available));

    hospitalManager.addHospital(Hospital("H1", 4, 3));
    hospitalManager.addHospital(Hospital("H2", 5, 2));

    DispatchEngine dispatchEngine(graph, routingEngine, emergencyQueue, ambulanceManager, hospitalManager);

    cout << "Sample city loaded: junctions 0-5, 2 ambulances, 2 hospitals.\n";

    bool running = true;

    while (running)
    {
        printMenu();
        int choice = readInt("Choose an option: ");

        switch (choice)
        {
        case 1:
            graph.printGraph();
            break;

        case 2:
        {
            string id = readLine("Emergency ID: ");
            int location = readInt("Incident junction ID: ");
            int severity = readInt("Severity (1-5): ");
            int arrivalTime = readInt("Arrival time: ");
            string type = readLine("Emergency type: ");

            if (!graph.hasVertex(location))
            {
                cout << "That junction does not exist. Emergency not added.\n";
            }
            else if (severity < 1 || severity > 5 || arrivalTime < 0)
            {
                cout << "Severity must be 1-5 and arrival time must be non-negative.\n";
            }
            else
            {
                emergencyQueue.addEmergency(Emergency(id, location, severity, arrivalTime, type));
                cout << "Emergency added to the pending queue.\n";
            }
            break;
        }

        case 3:
            if (dispatchEngine.dispatchNext())
            {
                printDispatchResult(dispatchEngine);
            }
            else if (emergencyQueue.empty())
            {
                cout << "There are no pending emergencies.\n";
            }
            else
            {
                cout << "Dispatch not possible now. The emergency remains pending; "
                     << "check ambulance availability, road connectivity, and "
                     << "hospital beds.\n";
            }
            break;

        case 4:
            printAmbulances(ambulanceManager);
            printHospitals(hospitalManager);
            break;

        case 5:
            Reports::print(dispatchEngine.getHistory(), ambulanceManager);
            break;

        case 6:
        {
            int from = readInt("Road endpoint 1: ");
            int to = readInt("Road endpoint 2: ");

            if (!graph.hasRoad(from, to))
            {
                cout << "Road not found.\n";
            }
            else
            {
                graph.blockRoad(from, to);
                cout << "Road blocked. Future routes will avoid it.\n";
            }
            break;
        }

        case 7:
        {
            int from = readInt("Road endpoint 1: ");
            int to = readInt("Road endpoint 2: ");

            if (!graph.hasRoad(from, to))
            {
                cout << "Road not found.\n";
            }
            else
            {
                graph.unblockRoad(from, to);
                cout << "Road unblocked.\n";
            }
            break;
        }

        case 8:
        {
            int from = readInt("Road endpoint 1: ");
            int to = readInt("Road endpoint 2: ");
            int weight = readInt("New positive road weight: ");

            if (!graph.hasRoad(from, to))
            {
                cout << "Road not found.\n";
            }
            else if (weight <= 0)
            {
                cout << "Road weight must be positive.\n";
            }
            else
            {
                graph.updateRoadWeight(from, to, weight);
                cout << "Road weight updated. Future routes will use the new weight.\n";
            }
            break;
        }

        case 9:
        {
            string emergencyId = readLine("Delivered emergency ID: ");

            if (dispatchEngine.completeTrip(emergencyId))
            {
                cout << "Patient delivery recorded; ambulance is now returning.\n";
            }
            else
            {
                cout << "Could not complete that trip. Check the emergency ID and its current status.\n";
            }
            break;
        }

        case 10:
        {
            string ambulanceId = readLine("Returning ambulance ID: ");
            int location = readInt("Ambulance's return location junction: ");

            if (dispatchEngine.finishReturn(ambulanceId, location))
            {
                cout << "Ambulance is now available at junction " << location << ".\n";
            }
            else
            {
                cout << "Could not finish the return. Check the ambulance status and junction ID.\n";
            }
            break;
        }

        case 11:
        {
            string hospitalId = readLine("Hospital ID: ");

            if (hospitalManager.dischargePatient(hospitalId))
            {
                cout << "Patient discharged; one bed is now available.\n";
            }
            else
            {
                cout << "Could not discharge a patient. Check the hospital ID and occupied-bed count.\n";
            }
            break;
        }

        case 0:
            running = false;
            cout << "Exiting the simulator.\n";
            break;

        default:
            cout << "Unknown option. Choose a listed menu number.\n";
            break;
        }
    }

    return 0;
}
