#ifndef TEAM_H
#define TEAM_H

#include <vector>
#include <string>

struct Competitor {
    std::string name;
    bool isTeamMember;
    int teamID;
    int score;

    Competitor() : name(""), isTeamMember(false), teamID(-1), score(0) {} // Default constructor
    Competitor(const std::string& name, bool isTeamMember = false, int teamID = -1, int score = 0)
        : name(name), isTeamMember(isTeamMember), teamID(teamID), score(score) {}
};

struct Team {
    std::string name;
    std::vector<Competitor> members;
    int score;

    Team() : name(""), score(0) {} // Default constructor
    Team(const std::string& name, int score = 0) : name(name), score(score) {}
};

#endif // TEAM_H