#include "GUI.h"
#include "DataManager.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <string>
#include <vector>
#include <iostream>

using namespace ftxui;

void launchGUI(
    std::vector<Team>& teams,
    std::vector<Competitor>& individuals,
    std::vector<::Event>& events // Explicitly qualify Event
) {
    auto screen = ScreenInteractive::Fullscreen();

    // Menu options
    std::vector<std::string> menu_entries = {
        "List Teams", "List Individuals", "List Events", "Create Team",
        "Create Individual", "Create Event", "Exit"
    };

    int selected = 0;
    auto menu = Menu(&menu_entries, &selected);

    // Main container
    auto container = Container::Vertical({
        menu,
    });

    // Renderer for the menu
    auto renderer = Renderer(container, [&] {
        return vbox({
            text("Team Tracker") | bold | center,
            separator(),
            menu->Render() | center,
        });
    });

    // Event loop
    screen.Loop(renderer);

    // Handle menu selection
    switch (selected) {
        case 0: // List Teams
            for (const auto& team : teams) {
                std::cout << "Team: " << team.name << "\n";
            }
            break;
        case 1: // List Individuals
            for (const auto& individual : individuals) {
                std::cout << "Individual: " << individual.name << "\n";
            }
            break;
        case 2: // List Events
            for (const auto& event : events) {
                std::cout << "Event: " << event.name << "\n";
            }
            break;
        case 3: { // Create Team
            std::string teamName;
            auto teamNameInput = Input(&teamName, "Enter team name");

            std::vector<std::string> memberNames(5);
            std::vector<Component> memberInputs;
            for (int i = 0; i < 5; ++i) {
                memberInputs.push_back(Input(&memberNames[i], "Member " + std::to_string(i + 1)));
            }

            auto formContainer = Container::Vertical({teamNameInput});
            formContainer->Add(Container::Vertical(memberInputs));

            auto formRenderer = Renderer(formContainer, [&] {
                Elements memberElements;
                for (auto& input : memberInputs) {
                    memberElements.push_back(input->Render());
                }

                return vbox(
                    text("Create Team") | bold | center,
                    separator(),
                    text("Team Name:"),
                    teamNameInput->Render(),
                    separator(),
                    vbox(std::move(memberElements)),
                    separator(),
                    Button("Submit", [&] {
                        Team newTeam(teamName);
                        int teamID = teams.size();
                        for (const auto& name : memberNames) {
                            newTeam.members.push_back({name, true, teamID});
                        }
                        teams.push_back(newTeam);
                        screen.ExitLoopClosure()();
                    }) | center
                );
            });

            screen.Loop(formRenderer);
            break;
        }
        case 4: { // Create Individual
            std::string name;
            auto nameInput = Input(&name, "Enter individual competitor name");

            auto formContainer = Container::Vertical({nameInput});

            auto formRenderer = Renderer(formContainer, [&] {
                return vbox(
                    text("Create Individual") | bold | center,
                    separator(),
                    text("Name:"),
                    nameInput->Render(),
                    separator(),
                    Button("Submit", [&] {
                        individuals.push_back({name, false, -1});
                        screen.ExitLoopClosure()();
                    }) | center
                );
            });

            screen.Loop(formRenderer);
            break;
        }
        case 5: { // Create Event
            std::string name;
            bool isTeamEvent = false;
            auto nameInput = Input(&name, "Enter event name");
            auto teamEventCheckbox = Checkbox("Is this a team event?", &isTeamEvent);

            auto formContainer = Container::Vertical({nameInput, teamEventCheckbox});

            auto formRenderer = Renderer(formContainer, [&] {
                return vbox(
                    text("Create Event") | bold | center,
                    separator(),
                    text("Event Name:"),
                    nameInput->Render(),
                    separator(),
                    teamEventCheckbox->Render(),
                    separator(),
                    Button("Submit", [&] {
                        events.push_back({name, isTeamEvent, {}});
                        screen.ExitLoopClosure()();
                    }) | center
                );
            });

            screen.Loop(formRenderer);
            break;
        }
    }
}
