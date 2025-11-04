#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include "inputLoop.h"
#include "TerminalFormatting.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ifstream;

using namespace VT;


int printTextFile(const string& filename) {
  if (ifstream file(filename); !file.is_open()) { return 1; }
  else {
    string line;
    while (getline(file, line)) { cout << line << endl; }
    file.close();
  }
  return 0;
}


int main() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    cout << CLEAR_SCREEN;
    cout << YELLOW;
    int success = printTextFile("Resources/armadilloASCII.txt");
    if (success == 1) {
      success = printTextFile("../Resources/armadilloASCII.txt");
      if (success == 1) { cerr << "Error: Could not display ascii art'\n"; }
    }
    cout << RESET_TEXT;
    cout << "##########################################################################################" << endl;
    cout << BLUE;

    int success2 = printTextFile("Resources/armadilloLogo.txt");
    if (success2 == 1) {
      success2 = printTextFile("../Resources/armadilloLogo.txt");
      if (success2 == 1) { cerr << "Error: Could not display ascii art'\n"; }
    }
    cout << RESET_TEXT;
    cout << "##########################################################################################" << endl << endl;
    cout << WHITE;
    cout << "     -  press ctrl+q to terminate any active command" << endl
         << "     -  use 'exit' command to close the shell" << endl << endl;
    int exitCode = inputLoop();
    cout << CLEAR_SCREEN << CURSOR_TOP_LEFT;

    return exitCode;
}

