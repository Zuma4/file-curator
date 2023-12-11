#include "cmd/cmd.h"
#include "types/types.h"
#include "file_management/file_management.h"
#include "unit_testing/unit_testing.h"
#include <iostream>
#include <vector>

int main() {

    setup();
    Cookies cookies;
    try {cookies = loadCookies();}
    catch(const char* e) {std::cout << "couldn't load cookies";}
    
    while(true)
    {
        std::vector<std::string> start_options{"load file save", "explore file", "organize", "quit"};
        int start_option = choose(start_options);

        if (start_option == 1)
            start_option_1();
        else if (start_option == 2)
            start_option_2();
        else if (start_option == 3)
            start_option_3(cookies);
        else
            break;
    }

    saveCookies(cookies);

    return 0;
}

// TODO: make functions throw proper error objects
