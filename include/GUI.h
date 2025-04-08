#ifndef GUI_H
#define GUI_H

#include <vector>
#include "Team.h"
#include "Event.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

// Function to launch the GUI
void launchGUI(
    std::vector<Team>& teams,
    std::vector<Competitor>& individuals,
    std::vector<::Event>& events
);

#endif // GUI_H
