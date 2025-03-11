#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <vector>
#include <string>

using namespace ftxui;

int main() {
    std::vector<std::vector<std::string>> data = {
        {"ID", "Name", "Age", "Occupation"},
        {"1", "Alice", "25", "Engineer"},
        {"2", "Bob", "30", "Designer"},
        {"3", "Charlie", "28", "Doctor"},
        {"4", "David", "35", "Teacher"}
    };

    Elements rows;cmd /c chcp 65001>nul && C:\msys64\ucrt64\bin\gcc.exe -fdiagnostics-color=always -g "G:\VSC Project\Teamtracker\src\UI.cpp" -o "G:\VSC Project\Teamtracker\src\UI.exe"
    G:\VSC Project\Teamtracker\src\UI.cpp:1:10: fatal error: ftxui/component/screen_interactive.hpp: No such file or directory
        1 | #include <ftxui/component/screen_interactive.hpp>
          |          ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    compilation terminated.
    
    Build finished with error(s).
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