#ifndef TEAM_H
#define TEAM_H

#include "Competitor.h"
#include <vector>
#include <string>

struct Team {
    std::string name;
    std::vector<Competitor> members;
    int score = 0;
};

#endif // TEAM_H