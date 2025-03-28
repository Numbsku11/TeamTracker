#include "Menu.h"
#include "DataManager.h"
#include <vector>
#include <string>
#include <iostream>

void MainMenu()
{
    using namespace std::string_literals; 
    
    std::string text = 
    R"(
    
    To have infomation added later :3 
    )"; 
}


int main() {

    std::vector<Team> teams;
    std::vector<Competitor> individuals;
    std::vector<Event> events;

    //std::cout << "[DEBUG] Loading data..." << std::endl;

    // Initialize the CSV file
    initialiseCSV();

    // Load existing data
    loadData(teams, individuals, events);

    std::string command;
    bool dataModified = false; // Flag to track if data has been modified

    do {
        displayMenu();
        std::getline(std::cin, command); // Read the entire line, including spaces
        //std::cout << "[DEBUG] Command entered: " << command << std::endl;

        // Process the command and check if it modifies the data
        if (processCommand(command, teams, individuals, events)) {
            dataModified = true;
        }

        // Save data only if it has been modified
        if (dataModified) {
            //std::cout << "[DEBUG] Saving data..." << std::endl;
            saveData(teams, individuals, events);
            dataModified = false; // Reset the flag after saving
        }
    } while (command != "exit");

    return 0;
}