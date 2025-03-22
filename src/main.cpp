#include "Menu.h"
#include "DataManager.h"
#include <vector>
#include <iostream>

int main() {

    std::vector<Team> teams;
    std::vector<Competitor> individuals;
    std::vector<Event> events;

    loadData(teams, individuals, events);

    std::string command;
    do {
        displayMenu();
        std::cin >> command;
        processCommand(command, teams, individuals, events);

        saveData(teams, individuals, events);
    } while (command != "exit");

    return 0;
}