#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <unordered_map>

struct Event {
    std::string name;
    bool isTeamEvent;
    std::unordered_map<int, int> scores; // teamID or individual ID -> score
};

#endif // EVENT_H