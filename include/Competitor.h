#ifndef COMPETITOR_H
#define COMPETITOR_H

#include <string>

struct Competitor {
    std::string name;
    bool isTeamMember;
    int teamID; // -1 if individual
};

#endif // COMPETITOR_H