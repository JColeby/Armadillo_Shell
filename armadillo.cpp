#include <string>
#include <iostream>
#include <fstream>

#include "inputLoop.h"

using namespace std;

int main() {
    // Code to display ascii art
    string filename = "../Resources/armadilloASCII.txt";
    if (ifstream file(filename); !file.is_open()) { cerr << "Error: Could not display ascii art'\n"; }
    else {
        string line;
        while (getline(file, line)) { cout << line << '\n'; }
        file.close();
    }

    return inputLoop();
}


