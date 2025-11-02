#include "inputHandler.h"

bool containsPipe;


void printBadCommandResult(const vector<string>& commandOutput) {
  cerr << "    got:  [";
  int size = commandOutput.size();
  for (int i = 0; i < size; ++i) {
    cerr << '"' << commandOutput[i] << '"';
    if (i < size - 1) { cerr << ", "; }
  }
  cerr << "]" << endl;
}


void displayOutput(const vector<string>& commandOutput) {
    if (commandOutput.empty() or commandOutput.size() != 2) { // bad command
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // red
        cerr << "ERROR: command output malformed." << endl;
        cerr << "    Command failed to return data to Armadillo in the following format: ";
        cerr << R"(["ErrorMsg/Output", "StatusCode"])" << endl;
        printBadCommandResult(commandOutput);
    }
    else if (commandOutput[1][0] == '2') { // command ran successfully!
        if (commandOutput[1] != "201") { // ran successfully, but no string should be printed
          cout << commandOutput[0] << endl;
        }
    }
    else if (commandOutput[1][0] == '3') { // unauthorized
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // yellow
      cout << "UNAUTHORIZED: " << commandOutput[0] << endl;
    }
    else if (commandOutput[1][0] == '4') { // user error
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // yellow
      cout << "MALFORMED ERROR: " << commandOutput[0] << endl;
    }
    else if (commandOutput[1][0] == '5') { // command error
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // red
      cerr << "ERROR:" << commandOutput[0] << endl;
    }
    else if (commandOutput[1][0] == '6') { // early exit successful
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // yellow
      cout << "Exited Command Successfully" << "Command output: " << commandOutput[0] << endl;
    }
    else {
      cerr << "ERROR: command output malformed." << endl;
      cerr << "    Command didn't return a valid error code: " << endl;
      printBadCommandResult(commandOutput);
    }
    // waiting so we don't have stupid race condition with input loop when it displays the current directory
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    runningCommand = false;
}


vector<string> tokenizeInput(const string& inputString) {
    std::istringstream wordSeparator(inputString);
    vector<string> tokens;
    string token;
    // separating characters by whitespace
    while (wordSeparator >> token) {
        if (token == "|") { containsPipe = true; } // so we know to call pipeHandler
        if (token[0] == '"') { // logic for quoted input. quotes will be automatically removed
            string quotedToken;
            string word = token.erase(0, 1);
            do { // gets next token until we hit a word that ends with " or until the input ends
                quotedToken += word + " ";
            } while (word[word.size() - 1] != '"' and wordSeparator >> word);
            quotedToken.erase(quotedToken.size() - 1);
            if (quotedToken[quotedToken.size() - 1] == '"') {
              quotedToken.erase(quotedToken.size() - 1); } // removed trailing "
            tokens.push_back(quotedToken);
        }
        else { tokens.push_back(token); }
    }
    return tokens;
}

void inputHandler(const string& userInput) {
  containsPipe = false;
  vector<string> tokens = tokenizeInput(userInput);
  if (containsPipe) { displayOutput(pipeHandler(tokens)); }
  else { displayOutput(commandHandler(tokens)); }
}