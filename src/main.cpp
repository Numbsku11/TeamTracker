#include "Menu.h"
#include "DataManager.h"
#include <vector>
#include <string>
#include <iostream>
#include <csignal> 

#ifdef _WIN32
#include <windows.h> 

#else
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <thread>
#endif

std::vector<Team> teams;
std::vector<Competitor> individuals;
std::vector<Event> events;
bool dataModified = false; 

#ifdef _WIN32
// Windows-specific message handler for program killing
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if 
    (
        signal == CTRL_CLOSE_EVENT || 
        signal == CTRL_LOGOFF_EVENT || 
        signal == CTRL_SHUTDOWN_EVENT
    ) 
    
    {
        std::cout << "\nSaving data before exiting...\n";
        saveData(teams, individuals, events); 
    }
    return TRUE;
}
#else
// Linux-specific signal handler for termination signals
void handleLinuxSignal(int signal) {
    std::cout << "\nSaving data before exiting...\n";
    saveData(teams, individuals, events); // Save data to CSV
    std::exit(signal);
}

// Linux-specific function to handle window close events
void handleXlibEvents() {
    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "[ERROR] Unable to open X display.\n";
        return;
    }

    Window root = DefaultRootWindow(display);
    Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);

    // Create a simple window
    Window window = XCreateSimpleWindow(display, root, 0, 0, 1, 1, 0, 0, 0);
    XSetWMProtocols(display, window, &wmDeleteMessage, 1);

    XMapWindow(display, window);

    // Event loop to listen for WM_DELETE_WINDOW
    XEvent event;
    while (true) {
        XNextEvent(display, &event);
        if (event.type == ClientMessage && static_cast<Atom>(event.xclient.data.l[0]) == wmDeleteMessage) {
            std::cout << "\nSaving data before exiting...\n";
            saveData(teams, individuals, events); // Save data to CSV
            XDestroyWindow(display, window);
            XCloseDisplay(display);
            std::exit(0);
        }
    }
}
#endif

int main() 
{
    // Register platform-specific handlers
    #ifdef _WIN32
    SetConsoleCtrlHandler(ConsoleHandler, TRUE); // Windows-specific handler

    #else
    std::signal(SIGINT, handleLinuxSignal);  // Handle Ctrl+C
    std::signal(SIGTERM, handleLinuxSignal); // Handle termination signals

    // Start a thread to handle Xlib events
    std::thread xlibThread(handleXlibEvents);
    xlibThread.detach();
    #endif

    // Initialize the CSV file
    initialiseCSV();

    // Load existing data
    loadData(teams, individuals, events);

    std::string command;

    do 
    {
        displayMenu();
        std::getline(std::cin, command); // Read the entire line, including spaces

        // Process the command and check if it modifies the data
        if (processCommand(command, teams, individuals, events)) 
        {
            dataModified = true;
        }

    } while (true);

    return 0;
}