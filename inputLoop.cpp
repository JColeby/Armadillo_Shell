#include "inputLoop.h"

using std::cin;
using std::thread;


int inputLoop() {
    while (true) {
      if (displayCurrentDirectory() == -1) { return -1; }
      string input;
      getline(cin, input);
      if (input == "exit") { return 0; } // exits the shell
      if (input == "clear") { system("cls"); continue; }
      runningCommand = true;
      thread t(inputHandler, input);
      while (runningCommand) {
        bool ctrlDown = GetAsyncKeyState(VK_CONTROL) & 0x8000; // 0x8000 means the key is currently being pressed
        bool qDown = GetAsyncKeyState('Q') & 0x8000;
        if (ctrlDown && qDown) { killSwitch = true; }
      }
      t.join();
    }
}


int displayCurrentDirectory() {
    TCHAR pathBuffer[MAX_PATH];
    DWORD length = GetCurrentDirectory(MAX_PATH, pathBuffer); // windows system call
    if (length == 0) {
      cerr << "FATAL ERROR: failed to fetch current directory. Exiting Armadillo" << endl;
      return -1;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // yellow
    cout << "ARDO";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // white
    cout << " => ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9); // blue
    cout << pathBuffer;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // white
    cout << " => ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // light grey
    return 1;
}