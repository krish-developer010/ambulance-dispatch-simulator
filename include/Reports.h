#ifndef REPORTS_H
#define REPORTS_H

#include <utility>
#include <vector>
#include "AmbulanceManager.h"
#include "DispatchEngine.h"

class Reports
{
public:
    static double averageResponseTime(
        const std::vector<DispatchRecord>& history);

    static std::vector<std::pair<std::string, int>> dispatchCounts(
        const std::vector<DispatchRecord>& history,
        const AmbulanceManager& ambulanceManager);

    static void print(
        const std::vector<DispatchRecord>& history,
        const AmbulanceManager& ambulanceManager);
};

#endif
