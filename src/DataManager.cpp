#include "DataManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

void saveData
(
    const std::vector<Team>& teams, 
    const std::vector<Competitor>& individuals, 
    const std::vector<Event>& events
) 
{
    std::ofstream outFile("data.csv");
    
    if (!outFile) 
    {
        std::cerr << "Error: Could not open file for saving!" << std::endl;
        return;
    }

    // Save teams
    outFile << "Teams\n";
    for (const auto& team : teams) 
    {
        outFile << team.name << "," << team.score << "\n";
        
        for (const auto& member : team.members) 
        {
            outFile << "Member," << member.name << "," << member.isTeamMember << "," << member.teamID << "\n";
        }
    }

    // Save individuals
    outFile << "Individuals\n";
    for (const auto& individual : individuals) 
    {
        outFile << individual.name << "," << individual.isTeamMember << "," << individual.teamID << "\n";
    }

    // Save events
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

void loadData
(
    std::vector<Team>& teams, 
    std::vector<Competitor>& individuals, 
    std::vector<Event>& events
) 
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
                if (teams.empty()) 
                {
                    std::cerr << "Error: Found 'Member' without a preceding team!" << std::endl;
                    continue;
                }

                Competitor member;
                std::getline(ss, token, ','); // Skip "Member"
                std::getline(ss, member.name, ',');
                ss >> member.isTeamMember;
                ss.ignore(1); // Skip comma
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
            ss.ignore(1); // Skip comma
            ss >> individual.teamID;
            individuals.push_back(individual);
        } 
        
        else if (section == "Events") 
        {
            if (line.find("Score") == 0) 
            {
                if (events.empty()) 
                {
                    std::cerr << "Error: Found 'Score' without a preceding event!" << std::endl;
                    continue;
                }

                int id, score;
                std::getline(ss, token, ','); // Skip "Score"
                ss >> id;
                ss.ignore(1); // Skip comma
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