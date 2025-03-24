#include <ftxui/component/component.hpp>       // for Tab, Renderer, etc.
#include <ftxui/component/screen_interactive.hpp> // for ScreenInteractive
#include <ftxui/dom/elements.hpp>              // for text, hbox, vbox, etc.
#include <iostream>
#include <string>
#include <vector>

using namespace ftxui;

// Function to create the "Teams" tab
Component CreateTeamsTab(std::vector<std::string>& teams, int& selected_team_index) {
    auto team_menu = Menu(&teams, &selected_team_index, MenuOption::Vertical());
    return Renderer(team_menu, [&] {
        return vbox({
            text("Teams:"),
            teams.empty() ? text("No teams available.") | dim : team_menu->Render(),
            separator(),
            teams.empty() || selected_team_index >= teams.size()
                ? text("")
                : text("Selected Team: " + teams[selected_team_index]),
        });
    });
}

// Function to create the "Events" tab
Component CreateEventsTab(std::vector<std::string>& events, int& selected_event_index) {
    auto event_menu = Menu(&events, &selected_event_index, MenuOption::Vertical());
    return Renderer(event_menu, [&] {
        return vbox({
            text("Events:"),
            events.empty() ? text("No events available.") | dim : event_menu->Render(),
            separator(),
            events.empty() || selected_event_index >= events.size()
                ? text("")
                : text("Selected Event: " + events[selected_event_index]),
        });
    });
}

// Function to create the "Create" tab
Component CreateCreateTab(std::vector<std::string>& teams, std::vector<std::string>& events) {
    std::string new_team_name;
    std::string new_event_name;

    auto team_input = Input(&new_team_name, "Enter team name");
    auto event_input = Input(&new_event_name, "Enter event name");
    auto add_team_button = Button("Add Team", [&] {
        if (!new_team_name.empty()) {
            teams.push_back(new_team_name);
            new_team_name.clear();
        }
    });
    auto add_event_button = Button("Add Event", [&] {
        if (!new_event_name.empty()) {
            events.push_back(new_event_name);
            new_event_name.clear();
        }
    });

    auto create_tab = Container::Vertical({
        team_input,
        event_input,
        Container::Horizontal({
            add_team_button,
            add_event_button,
        }),
    });

    return Renderer(create_tab, [&] {
        return vbox({
            text("Create Teams and Events") | bold | center,
            separator(),
            hbox({
                text("New Team: "),
                team_input->Render(),
            }),
            hbox({
                text("New Event: "),
                event_input->Render(),
            }),
            hbox({
                add_team_button->Render(),
                add_event_button->Render(),
            }) | center,
        });
    });
}

int main() {
    auto screen = ScreenInteractive::TerminalOutput();

    // State variables
    std::vector<std::string> teams;
    std::vector<std::string> events;
    int selected_team_index = 0;
    int selected_event_index = 0;
    int selected_tab = 0; // 0 for Teams, 1 for Events, 2 for Create

    // Create tabs
    auto teams_tab = CreateTeamsTab(teams, selected_team_index);
    auto events_tab = CreateEventsTab(events, selected_event_index);
    auto create_tab = CreateCreateTab(teams, events);

    // Tab system
    std::vector<std::string> tab_titles = {"Teams", "Events", "Create"};
    auto tab_content = Container::Tab({
        teams_tab,
        events_tab,
        create_tab,
    }, &selected_tab);

    // Add key navigation for tabs
    auto tab_navigation = CatchEvent(tab_content, [&](Event event) {
        if (event == Event::ArrowLeft) {
            selected_tab = (selected_tab - 1 + tab_titles.size()) % tab_titles.size();
            return true;
        }
        if (event == Event::ArrowRight) {
            selected_tab = (selected_tab + 1) % tab_titles.size();
            return true;
        }
        return false;
    });

    // Main renderer
    auto tab_renderer = Renderer(tab_navigation, [&] {
        return vbox({
            text("Team Tracker") | bold | center,
            separator(),
            hbox({
                text("Tabs: "),
                text(tab_titles[0]) | (selected_tab == 0 ? bold : dim),
                text(" | "),
                text(tab_titles[1]) | (selected_tab == 1 ? bold : dim),
                text(" | "),
                text(tab_titles[2]) | (selected_tab == 2 ? bold : dim),
            }) | center,
            separator(),
            tab_content->Render(),
        });
    });

    // Run the screen loop
    screen.Loop(tab_renderer);

    return 0;
}