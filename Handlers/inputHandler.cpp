#include "inputHandler.h"

void inputHandler(const string& userInput) {
    while (true) {
        if (killSwitch == true) {
          cout << "killing program" << endl;
          break;
        }
    }
    runningCommand = false;
}

vector<string> tokenizeInput(string inputString) {
    return {"tokenized", "input"}; ;
}