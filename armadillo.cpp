#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include "inputLoop.h"

using namespace std;

void printTextFile(const string& filename) {
  if (ifstream file(filename); !file.is_open()) { cerr << "Error: Could not display ascii art'\n"; }
  else {
    string line;
    while (getline(file, line)) { cout << line << endl; }
    file.close();
  }
}

int main() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    printTextFile("../Resources/armadilloASCII.txt");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "##########################################################################################" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
    printTextFile("../Resources/armadilloLogo.txt");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "##########################################################################################" << endl << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout << "     -  press ctrl+q to terminate any active command" << endl << "     -  use 'exit' command to close the shell" << endl << endl;
    return inputLoop();
}




