#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include "inputLoop.h"

using namespace std;

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
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    int success = printTextFile("Resources/armadilloASCII.txt");
    if (success == 1) {
        success = printTextFile("../Resources/armadilloASCII.txt");
        if (success == 1) { cerr << "Error: Could not display ascii art'\n"; }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "##########################################################################################" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
    int success2 = printTextFile("Resources/armadilloLogo.txt");
    if (success2 == 1) {
      success2 = printTextFile("../Resources/armadilloLogo.txt");
      if (success2 == 1) { cerr << "Error: Could not display ascii art'\n"; }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "##########################################################################################" << endl << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout << "     -  press ctrl+q to terminate any active command" << endl << "     -  use 'exit' command to close the shell" << endl << endl;
    return inputLoop();
}




