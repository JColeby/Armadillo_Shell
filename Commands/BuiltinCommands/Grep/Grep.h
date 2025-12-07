#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../../Command.h"
#include "Manual.h"
#include <regex>



// ===================={ Grep Command }====================
// TODO: add documentation to the manual file so James knows what flags and other info you implemented


class Grep : public Command<Grep> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  bool dontIncludeRegex;
  bool notCaseSensitive;
  // add more class variables as needed.

public:
  explicit Grep(vector<string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
    dontIncludeRegex = false;
    notCaseSensitive = false;
  }


  static string returnManText() {
    return GrepManual;
  }


  static bool validateSyntax(vector<string>& tokens) {
    if (tokens.size()==2) { return true; }
    if (tokens.size()==3 and tokens[0][0] == '-') { return true; }
    return false;
  }


  vector<string> executeCommand() override {
    int cmdSize = tokenizedCommand.size();
    setFlags();
    try {
      std::regex regularExpression;
      if (notCaseSensitive) {
        regularExpression = std::regex(tokenizedCommand[cmdSize-2], std::regex_constants::icase);
      } else {
        regularExpression = std::regex(tokenizedCommand[cmdSize-2]);
      }
      stringstream inputString(tokenizedCommand[cmdSize-1]);
      stringstream output;
      string line;
      std::smatch match;
      while (std::getline(inputString, line)) { // Read line by line until end of stream
        bool matchFound = std::regex_search(line, match, regularExpression);
        if ((matchFound and !dontIncludeRegex) or (!matchFound and dontIncludeRegex)) {
          output << line << "\n";
        }
      }
      return {output.str(), "200"};

    } catch (const std::regex_error& e) {
      stringstream errorMessage;
      errorMessage << "Invalid regular expression: " << e.what();
      return {errorMessage.str(), "400"};
    }
  }

private:
  void setFlags() {
    for (string param : tokenizedCommand) {
      if (param[0] == '-') {
        for (int i = 1; i < param.size(); i++) {
          switch (param[i]) {
          case 'v': dontIncludeRegex = true; break;
          case 'i': notCaseSensitive = true; break;
          default: break;
          }
        }
      }
    }
  }
  // put your own method definitions here
};

