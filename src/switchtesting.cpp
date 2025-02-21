#include <headers.h> 

// list of all the possible options and enums for a string
enum class StringCodes
{
    eating,
    drinking,
    sleeping,
    exit,
    unknown
};

// matches the strings and returns one of enumed options or an error case 
StringCodes hashString(const std::string& SourceString) 
{
    if (SourceString == "") return StringCodes::eating;
    if (SourceString == "")  return StringCodes::drinking;
    if (SourceString == "")  return StringCodes::sleeping;
    if (SourceString == "") return StringCodes::exit;
    return StringCodes::unknown;
}
// handles the switch case by matching the given command from stringCodes 
void processCommand(const std::string& command) 
{
    switch (hashString(command)) 
    {
        case StringCodes::eating:
            std::cout << "e" << std::endl;
        break;
        
        case StringCodes::drinking:
            std::cout << "b" << std::endl;
        break;
        
        case StringCodes::sleeping:
            std::cout << "c" << std::endl;
        break;
        
        case StringCodes::exit:
            exit(0);
        break; 
        
        default:
            std::cout << "Error, please re-enter option" << std::endl;
    }
}



void main()
{
    while (true)
    {
        std::string option; 
        std::cin >> option;
        
        processCommand(option);
    }
}