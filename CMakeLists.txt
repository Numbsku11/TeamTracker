cmake_minimum_required(VERSION 3.10)

# Project name and languages
project(Teamtracker LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent module
include(FetchContent)

# Fetch FTXUI library
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v6.0.2
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Add the executable
add_executable(Teamtracker
    src/main.cpp
    src/DataManager.cpp
    src/Menu.cpp
    src/GUI.cpp
)

# Include directories
target_include_directories(Teamtracker PRIVATE
    include
)

# Link the FTXUI library and other dependencies
find_package(Threads REQUIRED)

target_link_libraries(Teamtracker PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
    Threads::Threads
)

# Add resources.rc for application icon
if(WIN32)
    target_sources(Teamtracker PRIVATE Icon/resources.rc)
endif()