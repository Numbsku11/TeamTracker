#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <map>

struct Event {
    std::string name;
    bool isTeamEvent;
    std::map<int, int> scores; // teamID or individual ID -> score
};

#endif // EVENT_H