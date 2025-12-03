#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include "inputLoop.h"
#include "TerminalFormatting.h"
#include "Resources/ardoASCII.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ifstream;

using namespace VT;


int main() {
    // this allows us to make fancy terminal output by enabling ENABLE_VIRTUAL_TERMINAL_PROCESSING
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    // printing out ascii art and shell information
    system("cls");
    cout << YELLOW << armadilloASCII << RESET_TEXT;
    cout << "##########################################################################################";
    cout << BLUE << armadilloLogo << RESET_TEXT;
    cout << "##########################################################################################" << endl << endl;
    cout << WHITE;
    cout << "     -  run 'help' for additional information about Armadillo" << endl << endl;

    int exitCode = inputLoop(); // going to the main loop

    system("cls");  // clears the screen
    return exitCode;
}

