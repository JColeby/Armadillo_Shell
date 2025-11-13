#include "inputHandler.h"
#include "../TerminalFormatting.h"

bool containsPipe;

using namespace VT;


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
    cerr << RED << "ERROR: command output malformed." << endl
         << "    Command failed to return data to Armadillo in the following format: "
         << R"(["ErrorMsg/Output", "StatusCode"])" << endl;
    printBadCommandResult(commandOutput);
    cerr << RESET_TEXT;
  }
  else if (commandOutput[1][0] == '2') { // command ran successfully!
    if (commandOutput[1] != "201") { // ran successfully, but no string should be printed
      cout << commandOutput[0] << endl;
    }
  }
  else if (commandOutput[1][0] == '3') { // unauthorized
    cout << YELLOW << "UNAUTHORIZED: " << commandOutput[0] << endl << RESET_TEXT;
  }
  else if (commandOutput[1][0] == '4') { // user error
    cout << YELLOW << "MALFORMED ERROR: " << commandOutput[0] << endl << RESET_TEXT;
  }
  else if (commandOutput[1][0] == '5') { // command error
    cerr << RED << "ERROR: " << commandOutput[0] << endl << RESET_TEXT;
  }
  else if (commandOutput[1][0] == '6') { // early exit successful
    cout << YELLOW << "Exited Command Successfully" << endl
         << "  Command output: " << commandOutput[0] << endl << RESET_TEXT;
  }
  else {
    cerr << "ERROR: command output malformed." << endl
         << "    Command didn't return a valid error code: " << endl;
    printBadCommandResult(commandOutput);
  }
  // waiting so we don't have stupid race condition with input loop when it displays the current directory
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
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