#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include "Team.h"
#include "Competitor.h"
#include "Event.h"

void displayMenu();
void processCommand
(
    const std::string& command, 
    std::vector<Team>& teams, 
    std::vector<Competitor>& individuals, 
    std::vector<Event>& events
);
void clearScreen(); // Add this declaration

#endif // MENU_H