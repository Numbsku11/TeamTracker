#include <iostream>


int teamName()
{
    return 0;
}
int main()
{
    char selectionOption[] = "holding";
    std:: cout << "1. team name\n" 
                    << "2. team members\n"
                    << "3. events\n"
                    << "4. individuals\n"
                    << "5. list teams\n"
                    << "6. list individuals\n"
                    << "7. list team events\n"
                    << "8. list individual events";

    std::cin >> selectionOption;
    if (selectionOption == "team name")
    {
        teamName();
    }
    else if (selectionOption =="team members" )
    {
        
    }
    
}