#include "../Commands/importAllCommands.h"

vector<string> pipeHandler(const vector<string>& tokenizedInput) {
    while (true) {
        if (killSwitch == true) {
          cout << "killing program" << endl;
          break;
        }
    }
    return {"command output", "error code as a string", "test"};
}