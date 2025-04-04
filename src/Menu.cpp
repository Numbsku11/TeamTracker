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
    assignScore, 
    help,
    unknown
};

// Normalize input
std::string normaliseInput(const std::string& input) 
{
    std::string normalized;
    normalized.reserve(input.size());

    for (char c : input) {
        if (!std::isspace(c)) {
            normalized += std::tolower(c);
        }
    }

    return normalized;
}

// Map commands to enums
StringCodes hashString(const std::string& SourceString) 
{
    std::string normalizedInput = normaliseInput(SourceString);

    // Debug output
    //std::cout << "Debug: Normalized input: " << normalizedInput << std::endl;

    static const std::unordered_map <std::string, StringCodes> commandMap = 
    {
        {"listteams", StringCodes::listTeams},
        {"listindividuals", StringCodes::listIndividuals},
        {"listevents", StringCodes::listEvents},
        {"listleaderboard", StringCodes::listLeaderboard},
        {"createteam", StringCodes::createTeam},
        {"createindividual", StringCodes::createIndividual},
        {"createevent", StringCodes::createEvent},
        {"addscores", StringCodes::assignScore},
        {"help", StringCodes::help}
    };

    auto it = commandMap.find(normalizedInput);
    return (it != commandMap.end()) ? it->second : StringCodes::unknown;
}

// Clear screen utility
void clearScreen() 
{
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

int openHelpFile()
{
    const char* HelpFile = "help.txt";
    
#ifdef _WIN32
    std::string NotePad = "notepad " + std::string(HelpFile);    
    
    int RunTime = system(NotePad.c_str()); 

#else
    std::string Nano = "nano " + std::string(HelpFile);    
    
    int RunTime = system(Nano.c_str()); 

#endif
    
    if (RunTime == 0)
    {
        std::cout << "Help file opened";
    }

    else
    {
        std::cout << "unable to open help file";
    }
    return 0;
}


void displayMenu() 
{
    
    std::cout << "\nTeam Tracker\n";
    std::cout << "---------------------\n";
    std::cout << "Menu Options:\n\n";
    std::cout << "\nCreate:\n";
    std::cout << "  Create Team\n";
    std::cout << "  Create Individual\n";
    std::cout << "  Create Event\n";
    std::cout << "\nCompetitors and Events:\n";
    std::cout << "  Add Scores \n";    
    std::cout << "  List Events\n";
    std::cout << "  List Leaderboard\n";
    std::cout << "  List Teams\n";
    std::cout << "  List Individuals\n";
    std::cout << "\nOther:\n";
    std::cout << "  Help\n\n";
    std::cout << "Enter your command:\t";
}

void menuHold() 
{
    std::cout << "\nPress Enter to return to the menu...";
    
    // Clear any leftover input in the buffer
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    
    // Wait for the user to press Enter
    while (std::cin.get() != '\n') {
        // Do nothing, just wait for Enter
    }
    clearScreen();
}

// Process commands
bool processCommand
(
    const std::string& command, 
    std::vector<Team>& teams, 
    std::vector<Competitor>& individuals, 
    std::vector<Event>& events
) 

{
    bool dataModified = false; // Track if data was modified

    switch (hashString(command))
    {
        case StringCodes::listTeams:
            clearScreen();
            if (teams.empty()) {
                std::cout << "No teams have been registered yet!" << std::endl;
            } 
            
            else 
            {
                for (const auto& team : teams) 
                {
                    std::cout << "Team: " << team.name << "\nMembers:\n";
                    
                    for (const auto& member : team.members) 
                    {
                        std::cout << "- " << member.name << std::endl;
                    }
                }
            }
            menuHold();
            break;

        case StringCodes::listIndividuals:
        {

            clearScreen();
            
            if (individuals.empty()) 
            {
                std::cout << "No individuals have been registered yet!" << std::endl;
            } else 
            
            {
                std::cout << "Individual Participants:\n";
                for (const auto& individual : individuals) 
                {
                    std::cout << "- " << individual.name << std::endl;
                }
            }
            
            menuHold();
            break;
        }

        case StringCodes::listEvents:
        {

            clearScreen();
            
            if (events.empty()) 
            {
                std::cout << "No events have been created yet!" << std::endl;
            } 
            
            else 
            {
                std::cout << "\n--------------------------\n";
                std::cout << "Team events:\n";
                for (const auto& event : events) 
                {
                    if (event.isTeamEvent) 
                    {
                        std::cout << "\t" << event.name << " (Team Event)\n";
                    }
                }
                
                std::cout << "\n--------------------------\n";
                std::cout << "Individual events:\n";
                for (const auto& event : events) 
                {
                    if (!event.isTeamEvent) 
                    {
                        std::cout << "\t" << event.name << " (Individual Event)\n";
                    }
                }
            }
            
            menuHold();
            break;
        }

        case StringCodes::listLeaderboard: 
        {
            clearScreen();

            // Team Leaderboard
            std::cout << "\n--------------------------\n";
            std::cout << "Team Leaderboard:\n";
            for (const auto& team : teams) 
            {
                int totalScore = 0;
                int eventsParticipated = 0;

                for (const auto& event : events) {
                    if (event.isTeamEvent && event.scores.find(&team - &teams[0]) != event.scores.end()) 
                    {
                        totalScore += event.scores.at(&team - &teams[0]);
                        eventsParticipated++;
                    }
                }

                std::cout << "\t" << team.name << " - Total Score: " << totalScore
                << " (Events Participated: " << eventsParticipated << ")\n";
            }

            std::cout << "\n--------------------------\n";

            // Individual Leaderboard
            std::cout << "Individual Leaderboard:\n";
            for (const auto& individual : individuals) 
            {
                int totalScore = 0;
                int eventsParticipated = 0;

                for (const auto& event : events) 
                {
                    if (!event.isTeamEvent && event.scores.find(&individual - &individuals[0]) != event.scores.end()) 
                    {
                        totalScore += event.scores.at(&individual - &individuals[0]);
                        eventsParticipated++;
                    }
                }

                std::cout << "\t" << individual.name << " - Total Score: " << totalScore
                << " (Events Participated: " << eventsParticipated << ")\n";
            }
            
            std::cout << "\n--------------------------\n";
            menuHold();
            break;
        }

        case StringCodes::createTeam: 
        {
            clearScreen();
            
            if (teams.size() >= 4) 
            {
                std::cout << "Maximum number of teams reached!" << std::endl;
                break;
            }
            
            std::string teamName;
            std::cout << "Enter team name: ";
            std::cin >> teamName;
            std::vector<std::string> memberNames;
            
            for (int i = 0; i < 5; i++) 
            {
                std::string memberName;
                std::cout << "Enter member " << i + 1 << " name: ";
                std::cin >> memberName;
                memberNames.push_back(memberName);
            }
            
            Team newTeam(teamName, 0); // Correct constructor usage
            int teamID = teams.size();
            
            for (const auto& name : memberNames) 
            {
                newTeam.members.push_back({name, true, teamID});
            }
            teams.push_back(newTeam);

            std::cout << "\nTeam created successfully:\n";
            std::cout << "Team Name: " << newTeam.name << "\nMembers:\n";
            
            for (const auto& member : newTeam.members) 
            {
                std::cout << "- " << member.name << std::endl;
            }

            dataModified = true; // Data was modified
            menuHold();
            break;
        }

        case StringCodes::createIndividual: 
        {
            clearScreen();
            
            if (individuals.size() >= 20) 
            {
                std::cout << "Maximum number of individual competitors reached!" << std::endl;
                menuHold();
                break;
            }
            
            std::string name;
            std::cout << "Enter individual competitor name: ";
            std::cin >> name;
            individuals.push_back({name, false, -1});

            std::cout << "\nIndividual competitor created successfully:\n";
            std::cout << "Name: " << name << std::endl;

            dataModified = true; // Data was modified
            menuHold();
            break;
        }

        case StringCodes::createEvent: 
        {
            clearScreen();
            std::string name;
            bool isTeamEvent;
            
            std::cout << "Enter event name: ";
            std::cin >> name;
            std::cout << "Is this a team event? (1 for yes, 0 for no): ";
            std::cin >> isTeamEvent;
            events.push_back({name, isTeamEvent, {}});

            std::cout << "\nEvent created successfully:\n";
            std::cout << "Event Name: " << name << "\nType: " 
                    << (isTeamEvent ? "Team Event" : "Individual Event") << std::endl;

            dataModified = true; // Data was modified
            menuHold();
            break;
        }

        case StringCodes::assignScore: 
        {
            clearScreen();
            
            if (events.empty()) 
            {
                std::cout << "No events have been created yet!" << std::endl;
                menuHold();
                break;
            }

            // Display events
            std::cout << "Available Events:\n";
            for (size_t i = 0; i < events.size(); ++i) 
            {
                std::cout << i << ": " << events[i].name
                << (events[i].isTeamEvent ? " (Team Event)" : " (Individual Event)") << std::endl;
            }

            // Select an event
            int eventIndex;
            std::cout << "Enter the event index: ";
            std::cin >> eventIndex;
            
            if (eventIndex < 0 || eventIndex >= events.size()) 
            {
                std::cout << "Invalid event index!" << std::endl;
                menuHold();
                break;
            }

            Event& selectedEvent = events[eventIndex];

            // Assign positions and award points
            if (selectedEvent.isTeamEvent) 
            {
            
                // Input positions for each team
                std::vector<std::pair<int, int>> teamPositions; // Pair of teamID and position
                std::cout << "Enter positions for each team (1 for first, 2 for second, etc.):\n";
                
                for (size_t i = 0; i < teams.size(); ++i) 
                {
                    int position;
                    std::cout << "Team " << teams[i].name << ": ";
                    std::cin >> position;
                    teamPositions.emplace_back(i, position);
                }

                // Sort teams by their positions
                std::sort
                (teamPositions.begin(), 
                teamPositions.end(), 
                [](const auto& a, const auto& b) 
                
                {
                    return a.second < b.second; // Sort in ascending order of positions
                });

                // Assign points based on position
                std::vector <int> points = {20, 15, 10, 5}; // Points for top 4 positions
                
                for (size_t i = 0; i < teamPositions.size() && i < points.size(); ++i) 
                {
                    int teamID = teamPositions[i].first;
                    teams[teamID].score += points[i]; // Add points to the team's total score
                    selectedEvent.scores[teamID] = points[i]; // Store the points in the event
                    std::cout << "Team " << teams[teamID].name << " awarded " << points[i] << " points.\n";
                }

            } 
            
            else 
            {
                // Input positions for each individual
                std::vector<std::pair<int, int>> individualPositions; // Pair of individualID and position
                std::cout << "Enter positions for each individual (1 for first, 2 for second, etc.):\n";
                
                for (size_t i = 0; i < individuals.size(); ++i) 
                {
                    int position;
                    std::cout << "Individual " << individuals[i].name << ": ";
                    std::cin >> position;
                    individualPositions.emplace_back(i, position);
                }

                // Sort individuals by their positions
                std::sort
                (
                    individualPositions.begin(), 
                    individualPositions.end(), 
                    [](const auto& a, const auto& b) 
                
                {
                    return a.second < b.second; // Sort in ascending order of positions
                });

                // Assign points based on position
                for (size_t i = 0; i > individualPositions.size() && i > 20; ++i) 
                {
                    int individualID = individualPositions[i].first;
                    int points = 20 - i; // Points decrease from 20 to 1
                    
                    individuals[individualID].score += points; // Add points to the individual's total score
                    selectedEvent.scores[individualID] = points; // Store the points in the event
                    std::cout << "Individual " << individuals[individualID].name 
                    << " awarded " << points << " points.\n";
                }
            }

            dataModified = true; // Mark data as modified
            menuHold();
            break;
        }

        case StringCodes::help:
        {
            clearScreen();
            openHelpFile(); 
            menuHold(); 
            
            break;
        }

        default:
        {
            std::cout << "Error, please re-enter a valid command." << std::endl;
            menuHold();
            clearScreen();
            break;
        }
    }

    return dataModified; // Return whether data was modified
}