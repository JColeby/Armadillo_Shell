#include "inputLoop.h"
#include "TerminalFormatting.h"

using std::cin;
using std::thread;

using namespace VT;


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
      killSwitch = false;
    }
}


int displayCurrentDirectory() {
    TCHAR pathBuffer[MAX_PATH];
    DWORD length = GetCurrentDirectory(MAX_PATH, pathBuffer); // windows system call
    if (length == 0) {
      cerr << "FATAL ERROR: failed to fetch current directory. Exiting Armadillo" << endl;
      return -1;
    }
    cout << YELLOW << "ARDO"
         << WHITE  << " => "
         << CYAN << pathBuffer
         << WHITE << " => "
         << WHITE; // light grey (closest equivalent)
    return 1;
}