#include <ftxui/component/component.hpp> // For Renderer
#include <ftxui/component/screen_interactive.hpp> // For ScreenInteractive
#include <ftxui/dom/elements.hpp> // For table rendering
#include <vector>
#include <string>
#include <iostream>

using namespace ftxui;

int main() {
    std::vector<std::vector<std::string>> data = {
        {"ID", "Name", "Age", "Occupation"},
        {"1", "Alice", "25", "Engineer"},
        {"2", "Bob", "30", "Designer"},
        {"3", "Charlie", "28", "Doctor"},
        {"4", "David", "35", "Teacher"}
    };

    Elements rows;
    for (const auto& row : data) {
        Elements cols;
        for (const auto& cell : row) {
            cols.push_back(text(cell) | border);
        }
        rows.push_back(hbox(cols));
    }

    auto table = vbox(rows) | border;

    auto screen = ScreenInteractive::FitComponent();
    screen.Loop(Renderer([&] { return table; }));

    return 0;
}
