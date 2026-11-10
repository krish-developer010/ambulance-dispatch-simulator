#include "../include/Reports.h"

#include <iomanip>
#include <iostream>
#include <unordered_map>

using namespace std;

double Reports::averageResponseTime(const vector<DispatchRecord>& history)
{
    int totalResponseTime = 0;
    int responseCount = 0;

    for (const DispatchRecord& record : history)
    {
        if (record.responseTime >= 0)
        {
            totalResponseTime += record.responseTime;
            ++responseCount;
        }
    }

    if (responseCount == 0)
    {
        return 0.0;
    }

    return static_cast<double>(totalResponseTime) / responseCount;
}

vector<pair<string, int>> Reports::dispatchCounts(
    const vector<DispatchRecord>& history,
    const AmbulanceManager& ambulanceManager)
{
    unordered_map<string, int> counts;

    for (const Ambulance& ambulance : ambulanceManager.getAllAmbulances())
    {
        counts[ambulance.id] = 0;
    }

    for (const DispatchRecord& record : history)
    {
        ++counts[record.ambulanceId];
    }

    vector<pair<string, int>> result;
    result.reserve(counts.size());

    for (const auto& entry : counts)
    {
        result.push_back(entry);
    }

    return result;
}

void Reports::print(const vector<DispatchRecord>& history,
                   const AmbulanceManager& ambulanceManager)
{
    cout << "\n=== Dispatch Report ===\n";
    cout << "Recorded dispatches: " << history.size() << '\n';

    if (history.empty())
    {
        cout << "Average response time: N/A (no dispatches recorded)\n";
    }
    else
    {
        cout << fixed << setprecision(2);
        cout << "Average response time: "
             << averageResponseTime(history) << " road-weight units\n";
    }

    cout << "\nAmbulance dispatch share:\n";

    vector<pair<string, int>> counts = dispatchCounts(history, ambulanceManager);

    if (counts.empty())
    {
        cout << "No ambulances registered.\n";
    }
    else
    {
        for (const auto& entry : counts)
        {
            double share = 0.0;

            if (!history.empty())
            {
                share = 100.0 * entry.second / history.size();
            }

            cout << "  " << entry.first << ": "
                 << entry.second << " dispatch(es), "
                 << fixed << setprecision(1) << share << "% of recorded "
                 << "dispatches\n";
        }
    }

    cout << "\nDispatch history:\n";

    if (history.empty())
    {
        cout << "No dispatches yet.\n";
        return;
    }

    for (const DispatchRecord& record : history)
    {
        cout << "  Emergency " << record.emergency.id
             << " | ambulance " << record.ambulanceId
             << " | hospital " << record.hospitalId
             << " | response " << record.responseTime
             << " | incident-to-hospital distance "
             << record.incidentToHospitalDistance
             << " | patient delivered: "
             << (record.patientDelivered ? "yes" : "no")
             << '\n';
    }
}
