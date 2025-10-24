#include "inputHandler.h"

bool containsPipe;

void inputHandler(const string& userInput) {
    containsPipe = false;
    vector<string> tokens = tokenizeInput(userInput);
    vector<string> commandOutput;
    if (containsPipe) { displayOutput(pipeHandler(tokens)); }
    else { displayOutput(commandHandler(tokens)); }
    runningCommand = false;
}

void displayOutput(vector<string> commandOutput) {
    if (commandOutput.empty() or commandOutput.size() != 2) { // bad command
        cerr << "ERROR: command output malformed." << endl;
        cerr << "    Command failed to return data to Armadillo in the following format: ";
        cerr << R"(["Error/Output", "StatusCode"])" << endl;
        cerr << "    got:  [";
        int size = commandOutput.size();
        for (int i = 0; i < size; ++i) {
          cerr << '"' << commandOutput[i] << '"';
          if (i < size - 1) { cerr << ", "; }
        }
        cerr << "]" << endl;
    }
    else if (commandOutput[1][0] == '2') { // command ran successfully!
        cout << commandOutput[0] << endl;
    }
    else if (commandOutput[1][0] == '3') { // unauthorized
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
      cout << "UNAUTHORIZED: " << commandOutput[0] << endl;
    }
    else if (commandOutput[1][0] == '4') { // user error.
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
      cout << "USER ERROR: " << commandOutput[0] << endl;
    }
    else if (commandOutput[1][0] == '5') { // command error
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
      cerr << "ERROR:" << commandOutput[0] << endl;
    }
    else if (commandOutput[1][0] == '6') { // early exit successful
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
      cout << "Exited Command Successfully" << "Command output: " << commandOutput[0] << endl;
    }
    else {
      cerr << "ERROR: command output malformed." << endl;
      cerr << "    Command didn't return a valid error code: " << endl;
      cerr << "    got:  " << commandOutput[1] << endl;
    }
}

vector<string> tokenizeInput(const string& inputString) {
    istringstream wordSeparator(inputString);
    vector<string> tokens;
    string token;
    // separating characters by whitespace
    while (wordSeparator >> token) {
        if (token == "|") { containsPipe = true; } // so we know to call pipeHandler
        tokens.push_back(token);
    }
    return tokens;
}