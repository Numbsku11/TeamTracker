#include "DataManager.h"
#include "Team.h"
#include "Event.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

void saveData(
    const std::vector<Team>& teams, 
    const std::vector<Competitor>& individuals, 
    const std::vector<Event>& events
) {
    fs::path dataDir = fs::current_path() / "data";
    fs::path finalFilePath = dataDir / "data.csv";

    if (!fs::exists(dataDir)) {
        fs::create_directories(dataDir);
    }

    std::ofstream outFile(finalFilePath, std::ios::out | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "[ERROR] Failed to open file for writing: " << finalFilePath << std::endl;
        return;
    }

    // Save teams
    outFile << "Teams\n";
    for (const auto& team : teams) {
        outFile << team.name << "," << team.score << "\n";
        for (const auto& member : team.members) {
            outFile << "Member," << member.name << "," << member.isTeamMember << "," << member.teamID << "," << member.score << "\n";
        }
    }

    // Save individuals
    outFile << "Individuals\n";
    for (const auto& individual : individuals) {
        outFile << individual.name << "," << individual.isTeamMember << "," << individual.teamID << "," << individual.score << "\n";
    }

    // Save events
    outFile << "Events\n";
    for (const auto& event : events) {
        outFile << event.name << "," << event.isTeamEvent << "\n";
        for (const auto& [id, score] : event.scores) {
            outFile << "Score," << id << "," << score << "\n";
        }
    }

    outFile.close();
}

void loadData(
    std::vector<Team>& teams, 
    std::vector<Competitor>& individuals, 
    std::vector<Event>& events
) {
    fs::path dataDir = fs::current_path() / "data";
    fs::path filePath = dataDir / "data.csv";

    if (!fs::exists(filePath)) {
        std::cerr << "[DEBUG] No saved data found. Starting fresh!" << std::endl;
        return;
    }

    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        std::cerr << "[ERROR] Failed to open file for reading: " << filePath << std::endl;
        return;
    }

    std::string line, section;
    while (std::getline(inFile, line)) {
        if (line == "Teams") {
            section = "Teams";
            continue;
        } else if (line == "Individuals") {
            section = "Individuals";
            continue;
        } else if (line == "Events") {
            section = "Events";
            continue;
        }

        std::istringstream ss(line);
        std::string token;

        if (section == "Teams") {
            if (line.find("Member") == 0) {
                Competitor member;
                std::getline(ss, token, ','); // Skip "Member"
                std::getline(ss, member.name, ',');
                std::getline(ss, token, ',');
                member.isTeamMember = std::stoi(token);
                std::getline(ss, token, ',');
                member.teamID = std::stoi(token);
                std::getline(ss, token, ',');
                member.score = std::stoi(token);
                teams.back().members.push_back(member);
            } else {
                Team team;
                std::getline(ss, team.name, ',');
                std::getline(ss, token, ',');
                team.score = std::stoi(token);
                teams.push_back(team);
            }
        } else if (section == "Individuals") {
            Competitor individual;
            std::getline(ss, individual.name, ',');
            std::getline(ss, token, ',');
            individual.isTeamMember = std::stoi(token);
            std::getline(ss, token, ',');
            individual.teamID = std::stoi(token);
            std::getline(ss, token, ',');
            individual.score = std::stoi(token);
            individuals.push_back(individual);
        } else if (section == "Events") {
            if (line.find("Score") == 0) {
                int id, score;
                std::getline(ss, token, ','); // Skip "Score"
                std::getline(ss, token, ',');
                id = std::stoi(token);
                std::getline(ss, token, ',');
                score = std::stoi(token);
                events.back().scores[id] = score;
            } else {
                Event event;
                std::getline(ss, event.name, ',');
                std::getline(ss, token, ',');
                event.isTeamEvent = std::stoi(token);
                events.push_back(event);
            }
        }
    }

    inFile.close();
}

void initializeCSV() {
    std::cout << "[DEBUG] Initializing CSV file..." << std::endl;

    // Define the directory and file paths
    fs::path dataDir = fs::current_path() / "data";
    fs::path finalFilePath = dataDir / "data.csv";

    // Ensure the data directory exists
    if (!fs::exists(dataDir)) {
        try {
            fs::create_directories(dataDir);
            std::cout << "[DEBUG] Data directory created: " << dataDir << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Failed to create data directory: " << e.what() << std::endl;
            return;
        }
    }

    // Check if the CSV file already exists
    if (!fs::exists(finalFilePath)) {
        try {
            std::ofstream outFile(finalFilePath, std::ios::out | std::ios::trunc);
            if (!outFile.is_open()) {
                throw std::ios_base::failure("Failed to create CSV file.");
            }
            std::cout << "[DEBUG] CSV file created successfully: " << finalFilePath << std::endl;

            // Write headers to the CSV file
            outFile << "Teams\n";
            outFile << "Individuals\n";
            outFile << "Events\n";

            outFile.close();
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Exception occurred while initializing CSV file: " << e.what() << std::endl;
        }
    } else {
        std::cout << "[DEBUG] CSV file already exists: " << finalFilePath << std::endl;
    }
}
