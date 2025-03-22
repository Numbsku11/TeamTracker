#include <headers.h>

struct Competitor {
    std::string name;
    bool isTeamMember;
    int teamID; // -1 if individual
};

struct Team {
    std::string name;
    std::vector<Competitor> members;
    int score = 0;
};

struct Event {
    std::string name;
    bool isTeamEvent;
    std::map<int, int> scores; // teamID or individual ID -> score
};

std::vector<Team> teams;
std::vector<Competitor> individuals;
std::vector<Event> events;

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

void registerTeam() 
{
    if (teams.size() >= 4) 
    {
        std::cout << "Maximum number of teams reached!" << std::endl;
        return;
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

    Team newTeam{teamName, {}, 0};
    int teamID = teams.size();

    for (const auto &name : memberNames) 
    {
        newTeam.members.push_back({name, true, teamID});
    }

    teams.push_back(newTeam);
}

void registerIndividual() 
{
    if (individuals.size() >= 20) 
    {
        std::cout << "Maximum number of individual competitors reached!" << std::endl;
        return;
    }

    std::string name;
    std::cout << "Enter individual competitor name: ";
    std::cin >> name;
    individuals.push_back({name, false, -1});
}

void addEvent() 
{
    std::string name;
    bool isTeamEvent;
    std::cout << "Enter event name: ";
    std::cin >> name;
    std::cout << "Is this a team event? (1 for yes, 0 for no): ";
    std::cin >> isTeamEvent;
    events.push_back({name, isTeamEvent, {}});
}

void recordScore() 
{
    int eventIndex;
    std::cout << "Enter event index: ";
    std::cin >> eventIndex;
    if (eventIndex < 0 || eventIndex >= events.size()) 
    {
        std::cout << "Invalid event index!" << std::endl;
        return;
    }

    int competitorID, score;
    std::cout << "Enter competitor/team ID: ";
    std::cin >> competitorID;
    std::cout << "Enter score: ";
    std::cin >> score;
    events[eventIndex].scores[competitorID] = score;

    if (events[eventIndex].isTeamEvent && competitorID < teams.size()) 
    {
        teams[competitorID].score += score;
    }
}

void displayLeaderboard() 
{
    std::cout << "Team Leaderboard:" << std::endl;

    for (const auto &team : teams) 
    {
        std::cout << team.name << " - Score: " << team.score << std::endl;
    }

    std::cout << "\nIndividual Leaderboard:" << std::endl;
    for (const auto &event : events) 
    {
        if (!event.isTeamEvent) 
        {
            for (const auto &[id, score] : event.scores) 
            {
                std::cout << individuals[id].name << " - Score: " << score << std::endl;
            }
        }
    }
}

void displayTeamMembers() 
{
    if (teams.empty()) 
    {
        std::cout << "No teams have been registered yet!" << std::endl;
        return;
    }

    for (const auto& team : teams) 
    {
        std::cout << "Team: " << team.name << std::endl;
        std::cout << "Members:" << std::endl;
        for (const auto& member : team.members) 
        {
            std::cout << "- " << member.name << std::endl;
        }
        std::cout << std::endl;
    }
}

void saveData() 
{
    std::ofstream outFile("data.csv");
    if (!outFile) 
    {
        std::cerr << "Error: Could not open file for saving!" << std::endl;
        return;
    }

    outFile << "Teams\n";
    for (const auto& team : teams) 
    {
        outFile << team.name << "," << team.score << "\n";
        for (const auto& member : team.members) 
        {
            outFile << "Member," << member.name << "," << member.isTeamMember << "," << member.teamID << "\n";
        }
    }

    outFile << "Individuals\n";
    for (const auto& individual : individuals) 
    {
        outFile << individual.name << "," << individual.isTeamMember << "," << individual.teamID << "\n";
    }

    outFile << "Events\n";
    for (const auto& event : events) 
    {
        outFile << event.name << "," << event.isTeamEvent << "\n";
        for (const auto& [id, score] : event.scores) 
        {
            outFile << "Score," << id << "," << score << "\n";
        }
    }

    outFile.close();
}

void loadData() 
{
    std::ifstream inFile("data.csv");
    if (!inFile) 
    {
        std::cerr << "No saved data found. Starting fresh!" << std::endl;
        return;
    }

    std::string line, section;
    while (std::getline(inFile, line)) 
    {
        if (line == "Teams") 
        {
            section = "Teams";
            continue;
        } 
        else if (line == "Individuals") 
        {
            section = "Individuals";
            continue;
        } 
        else if (line == "Events") 
        {
            section = "Events";
            continue;
        }

        std::istringstream ss(line);
        std::string token;
        if (section == "Teams") 
        {
            if (line.find("Member") == 0) 
            {
                Competitor member;
                std::getline(ss, token, ',');
                std::getline(ss, member.name, ',');
                ss >> member.isTeamMember;
                ss.ignore(1);
                ss >> member.teamID;
                teams.back().members.push_back(member);
            } 
            else 
            {
                Team team;
                std::getline(ss, team.name, ',');
                ss >> team.score;
                teams.push_back(team);
            }
        } 
        else if (section == "Individuals") 
        {
            Competitor individual;
            std::getline(ss, individual.name, ',');
            ss >> individual.isTeamMember;
            ss.ignore(1);
            ss >> individual.teamID;
            individuals.push_back(individual);
        } 
        else if (section == "Events") 
        {
            if (line.find("Score") == 0) 
            {
                int id, score;
                std::getline(ss, token, ',');
                ss >> id;
                ss.ignore(1);
                ss >> score;
                events.back().scores[id] = score;
            } 
            else 
            {
                Event event;
                std::getline(ss, event.name, ',');
                ss >> event.isTeamEvent;
                events.push_back(event);
            }
        }
    }

    inFile.close();
}

void displayMenu() 
{
    std::cout << "\nMenu Options:\n";
    std::cout << "Participants:\n";
    std::cout << "  listTeams\n";
    std::cout << "  listIndividuals\n";
    std::cout << "\nEvents:\n";
    std::cout << "  listEvents\n";
    std::cout << "  listLeaderboard\n";
    std::cout << "\nCreate:\n";
    std::cout << "  createTeam\n";
    std::cout << "  createIndividual\n";
    std::cout << "  createEvent\n";
    std::cout << "\nOther:\n";
    std::cout << "  exit\n";
    std::cout << "Enter your command: ";
}

StringCodes hashString(const std::string& SourceString) 
{
    static const std::unordered_map<std::string, StringCodes> commandMap = {
        {"listTeams", StringCodes::listTeams},
        {"listIndividuals", StringCodes::listIndividuals},
        {"listEvents", StringCodes::listEvents},
        {"listLeaderboard", StringCodes::listLeaderboard},
        {"createTeam", StringCodes::createTeam},
        {"createIndividual", StringCodes::createIndividual},
        {"createEvent", StringCodes::createEvent},
        {"exit", StringCodes::exit}
    };

    auto it = commandMap.find(SourceString);
    return (it != commandMap.end()) ? it->second : StringCodes::unknown;
}

void clearScreen() 
{
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

void processCommand(const std::string& command) 
{
    switch (hashString(command)) 
    {
        case StringCodes::listTeams:
            clearScreen();
            displayTeamMembers();
            std::cout << "Press enter to continue..." << std::endl;
            std::cin.ignore();
            std::cin.get();
            break;

        case StringCodes::listIndividuals:
            clearScreen();
            if (individuals.empty()) 
            {
                std::cout << "No individuals have been registered yet!" << std::endl;
            } 
            else 
            {
                std::cout << "Individual Participants:\n";
                for (const auto& individual : individuals) 
                {
                    std::cout << "- " << individual.name << std::endl;
                }
            }
            std::cout << "Press enter to continue..." << std::endl;
            std::cin.ignore();
            std::cin.get();
            break;

        case StringCodes::listEvents:
            clearScreen();
            if (events.empty()) 
            {
                std::cout << "No events have been created yet!" << std::endl;
            } 
            else 
            {
                std::cout << "Events:\n";
                for (size_t i = 0; i < events.size(); ++i) 
                {
                    std::cout << i << ": " << events[i].name
                              << (events[i].isTeamEvent ? " (Team Event)" : " (Individual Event)") << std::endl;
                }
            }
            std::cout << "Press enter to continue..." << std::endl;
            std::cin.ignore();
            std::cin.get();
            break;

        case StringCodes::listLeaderboard:
            clearScreen();
            displayLeaderboard();
            std::cout << "Press enter to continue..." << std::endl;
            std::cin.ignore();
            std::cin.get();
            break;

        case StringCodes::createTeam:
            registerTeam();
            break;

        case StringCodes::createIndividual:
            registerIndividual();
            break;

        case StringCodes::createEvent:
            addEvent();
            break;

        case StringCodes::exit:
            std::cout << "Exiting program..." << std::endl;
            exit(0);

        default:
            std::cout << "Error, please re-enter a valid command." << std::endl;
    }
}

int main() 
{
    loadData();

    std::string command;
    do 
    {
        displayMenu();
        std::cin >> command;
        processCommand(command);
        saveData();
    } while (command != "exit");

    return 0;
}
