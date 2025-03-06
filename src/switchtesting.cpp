#include <headers.h> 
bool debugBool = 0; 

void csvHandle()
{
    
}

void mkdirHandle()
{

}







// list of all the possible options and enums for a string
enum class StringCodes
{
    mkdirtest,
    debug,
    junkPrinter,
    exit,
    clear,
    unknown
};

// matches the strings and returns one of enumed options or an error case 
StringCodes hashString(const std::string& SourceString) 
{
    if (SourceString == "mkdir") return StringCodes::mkdirtest;
    if (SourceString == "debug")  return StringCodes::debug;
    if (SourceString == "clear")  return StringCodes::clear;
    if (SourceString == "junk")  return StringCodes::junkPrinter;
    if (SourceString == "") return StringCodes::exit;
    return StringCodes::unknown;
}
// handles the switch case by matching the given command from stringCodes 
void processCommand(const std::string& command) 
{
    switch (hashString(command)) 
    {
        case StringCodes::mkdirtest:
            if (debugBool == 1)
            {
                std::cout << "debug: directory creation, path & deletion" << std::endl;
            }
            mkdirHandle(); 
        break;
        
        case StringCodes::debug:
            //simple logic to enable or disable the debug flag 
            if (debugBool != 1)
            {
                debugBool = 1;
                std::cout << "debug set to:\t" << debugBool << std::endl;
            }
            
            else 
            {
                debugBool = 0;
                std::cout << "debug set to:\t" << debugBool << std::endl; 
            }
        break;
        
        
        case StringCodes::junkPrinter:
            // test to ensure that the clear func removes everything in the terminal
            std::cout << "Lorem ipsum dolor sit amet, consectetur adipiscing elit.\n Donec commodo ante nec diam venenatis consequat. Mauris tristique lacus id neque faucibus laoreet.\n Etiam urna tortor, cursus vitae eros eu, tristique vulputate dolor.\n Phasellus at metus ipsum. Praesent in orci vitae odio viverra vehicula a id sapien.\n Maecenas sapien est, sodales a erat at, dictum cursus tortor.\n Etiam sagittis est quis augue efficitur, ultricies consectetur metus fermentum. Integer nisi sapien, elementum vel nibh sit amet, interdum ultricies neque.\n Aliquam lobortis consectetur ex vestibulum accumsan. \nDonec urna purus, venenatis sit amet diam vel, accumsan gravida leo. Ut et aliquet lacus."
            << std::endl;
        break;
        
        case StringCodes::exit:
            exit(0);
        break; 
        
        case StringCodes::clear:
            //terminal code to clear everything (working on linux and some later versions of windows)
            printf("\033c"); 
        break;

        default:
            // case for if nothing is recognised from user input 
            std::cout << "Error, please re-enter option" << std::endl;
    }
}


int main()
{
    while (true)
    {
        std::string starterString = ""; 
        std::cin >> starterString;
        
        // takes the string from above and changes every character to be lowercase 
        std::transform(starterString.begin(),starterString.end(),
        starterString.begin() ,[](unsigned char c) {return std::tolower(c);});
    
        if (starterString != "debug" && debugBool == 1)
    {
        std::cout << "debug: lowerstring: \t" << starterString << std::endl;
    }

        processCommand(starterString);
    }
}