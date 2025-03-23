#include "Menu.h"
#include "DataManager.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>    
#include <limits>    
// Define the enums for commands

enum class StringCodes {
    listTeams,
    listIndividuals,
    listEvents,
    listLeaderboard,
    createTeam,
    createIndividual,
    createEvent,
    exit,
    unknown
};

// Normalize input
std::string normalizeInput(const std::string& input) 
{
    // Debug: Print input before normalization
    //std::cout << "Debug: Input before normalization: \"" << input << "\"" << std::endl;

    // Trim leading and trailing spaces
    size_t start = input.find_first_not_of(" \t");
    size_t end = input.find_last_not_of(" \t");
    std::string trimmed = (start == std::string::npos) ? "" : input.substr(start, end - start + 1);

    // Remove all spaces from the string
    trimmed.erase(std::remove(trimmed.begin(), trimmed.end(), ' '), trimmed.end());

    // Convert to lowercase
    std::transform(trimmed.begin(), trimmed.end(), trimmed.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    // Debug: Print output after normalization
    //std::cout << "Debug: Output after normalization: \"" << trimmed << "\"" << std::endl;

    return trimmed;
}

// Map commands to enums
StringCodes hashString(const std::string& SourceString) {
    std::string normalizedInput = normalizeInput(SourceString);

    // Debug output
    //std::cout << "Debug: Normalized input: " << normalizedInput << std::endl;

    static const std::unordered_map<std::string, StringCodes> commandMap = {
        {"listteams", StringCodes::listTeams},
        {"listindividuals", StringCodes::listIndividuals},
        {"listevents", StringCodes::listEvents},
        {"listleaderboard", StringCodes::listLeaderboard},
        {"createteam", StringCodes::createTeam},
        {"createindividual", StringCodes::createIndividual},
        {"createevent", StringCodes::createEvent},
        {"exit", StringCodes::exit}
    };

    auto it = commandMap.find(normalizedInput);
    return (it != commandMap.end()) ? it->second : StringCodes::unknown;
}

// Clear screen utility
void clearScreen() {
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

void displayMenu() {
    
    std::cout << "\nTeam Tracker\n";
    std::cout << "---------------------\n";
    std::cout << "Menu Options:\n\n";
    std::cout << "Participants:\n";
    std::cout << "  list Teams\n";
    std::cout << "  list Individuals\n";
    std::cout << "\nEvents:\n";
    std::cout << "  list Events\n";
    std::cout << "  list Leaderboard\n";
    std::cout << "\nCreate:\n";
    std::cout << "  create Team\n";
    std::cout << "  create Individual\n";
    std::cout << "  create Event\n";
    std::cout << "\nOther:\n";
    std::cout << "  exit\n";
    std::cout << "Enter your command: ";
}

void menuHold() {
    std::cout << "\nHit Enter to return to the menu...";
    std::cin.ignore(std::cin.rdbuf()->in_avail(), '\n'); // Clear any leftover input in the buffer
    std::cin.get(); // Wait for the user to press Enter
}

// Process commands
void processCommand(const std::string& command, std::vector<Team>& teams, std::vector<Competitor>& individuals, std::vector<Event>& events) {
    switch (hashString(command)) {
        case StringCodes::listTeams:
            clearScreen();
            if (teams.empty()) {
                std::cout << "No teams have been registered yet!" << std::endl;
            } else {
                for (const auto& team : teams) {
                    std::cout << "Team: " << team.name << "\nMembers:\n";
                    for (const auto& member : team.members) {
                        std::cout << "- " << member.name << std::endl;
                    }
                }
            }
            menuHold();
            break;

        case StringCodes::listIndividuals:
            clearScreen();
            if (individuals.empty()) {
                std::cout << "No individuals have been registered yet!" << std::endl;
            } else {
                std::cout << "Individual Participants:\n";
                for (const auto& individual : individuals) {
                    std::cout << "- " << individual.name << std::endl;
                }
            }
            menuHold();
            break;

        case StringCodes::listEvents:
            clearScreen();
            if (events.empty()) {
                std::cout << "No events have been created yet!" << std::endl;
            } else {
                std::cout << "Events:\n";
                for (size_t i = 0; i < events.size(); ++i) {
                    std::cout << i << ": " << events[i].name
                            << (events[i].isTeamEvent ? " (Team Event)" : " (Individual Event)") << std::endl;
                }
            }
            menuHold();
            break;

        case StringCodes::listLeaderboard:
            clearScreen();
            std::cout << "Team Leaderboard:\n";
            for (const auto& team : teams) {
                std::cout << team.name << " - Score: " << team.score << std::endl;
            }
            std::cout << "\nIndividual Leaderboard:\n";
            for (const auto& event : events) {
                if (!event.isTeamEvent) {
                    for (const auto& [id, score] : event.scores) {
                        std::cout << individuals[id].name << " - Score: " << score << std::endl;
                    }
                }
            }
            menuHold();
            break;

        case StringCodes::createTeam: {
            clearScreen();
            if (teams.size() >= 4) {
                std::cout << "Maximum number of teams reached!" << std::endl;
                break;
            }
            std::string teamName;
            std::cout << "Enter team name: ";
            std::cin >> teamName;
            std::vector<std::string> memberNames;
            for (int i = 0; i < 5; i++) {
                std::string memberName;
                std::cout << "Enter member " << i + 1 << " name: ";
                std::cin >> memberName;
                memberNames.push_back(memberName);
            }
            Team newTeam{teamName, {}, 0};
            int teamID = teams.size();
            for (const auto& name : memberNames) {
                newTeam.members.push_back({name, true, teamID});
            }
            teams.push_back(newTeam);

            // Output the most recently created team
            std::cout << "\nTeam created successfully:\n";
            std::cout << "Team Name: " << newTeam.name << "\nMembers:\n";
            for (const auto& member : newTeam.members) {
                std::cout << "- " << member.name << std::endl;
            }

            menuHold();
            break;
        }

        case StringCodes::createIndividual: {
            clearScreen();
            if (individuals.size() >= 20) {
                std::cout << "Maximum number of individual competitors reached!" << std::endl;
                menuHold();
                break;
            }
            std::string name;
            std::cout << "Enter individual competitor name: ";
            std::cin >> name;
            individuals.push_back({name, false, -1});

            // Output the most recently created individual
            std::cout << "\nIndividual competitor created successfully:\n";
            std::cout << "Name: " << name << std::endl;

            menuHold();
            break;
        }

        case StringCodes::createEvent: {
            clearScreen();
            std::string name;
            bool isTeamEvent;
            std::cout << "Enter event name: ";
            std::cin >> name;
            std::cout << "Is this a team event? (1 for yes, 0 for no): ";
            std::cin >> isTeamEvent;
            events.push_back({name, isTeamEvent, {}});

            // Output the most recently created event
            std::cout << "\nEvent created successfully:\n";
            std::cout << "Event Name: " << name << "\nType: " 
                    << (isTeamEvent ? "Team Event" : "Individual Event") << std::endl;

            menuHold();
            break;
        }

        case StringCodes::exit:
            clearScreen();
            std::cout << "Exiting program..." << std::endl;
            break;

        default:
            std::cout << "Error, please re-enter a valid command." << std::endl;
            menuHold();
            clearScreen();
            break;
    }
}