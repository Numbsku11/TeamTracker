#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "Team.h"
#include "Event.h"
#include <vector>

// Function declarations
void saveData(
    const std::vector<Team>& teams,
    const std::vector<Competitor>& individuals,
    const std::vector<::Event>& events
);

void loadData(
    std::vector<Team>& teams,
    std::vector<Competitor>& individuals,
    std::vector<::Event>& events
);

void initialiseCSV(); // New function to initialize the CSV file

#endif // DATAMANAGER_H