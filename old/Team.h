#ifndef TEAM_H
#define TEAM_H

#include <vector>
#include <string>

struct Competitor {
    std::string name;
    bool isTeamMember = false; // Indicates if the competitor is part of a team
    int teamID = -1;           // -1 if the competitor is an individual
    int score = 0;             // Total score for the competitor
};

struct Team {
    std::string name;
    std::vector<Competitor> members;
    int score = 0;
};

#endif // TEAM_H