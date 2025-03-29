#include "Menu.h"
#include "DataManager.h"
#include <vector>
#include <string>
#include <iostream>
#include <csignal> // For signal handling

std::vector<Team> teams;
std::vector<Competitor> individuals;
std::vector<Event> events;
bool dataModified = false; // Flag to track if data has been modified

void handleSignal(int signal) {
    std::cout << "\n[INFO] Signal received (" << signal << "). Saving data before exiting...\n";
    if (dataModified) {
        saveData(teams, individuals, events);
    }
    std::exit(signal);
}

void MainMenu() {
    using namespace std::string_literals;

    std::string text =
        R"(

    )";

    std::cout << text << "\n\nPress enter to continue" << std::endl;
    menuHold();
}

int main() {
    // Register signal handlers
    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);

    MainMenu();

    // Initialize the CSV file
    initialiseCSV();

    // Load existing data
    loadData(teams, individuals, events);

    std::string command;

    do {
        displayMenu();
        std::getline(std::cin, command); // Read the entire line, including spaces

        // Process the command and check if it modifies the data
        if (processCommand(command, teams, individuals, events)) {
            dataModified = true;
        }

    } while (command != "exit");

    if (dataModified) {
        saveData(teams, individuals, events);
    }

    return 0;
}