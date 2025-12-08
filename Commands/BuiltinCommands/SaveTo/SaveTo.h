#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../../Command.h"
#include "Manual.h"
#include <fstream>

// ===================={ save to Command }====================

class SaveTo : public Command<SaveTo> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit SaveTo(vector<string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static string returnManText() {
    return SaveToManual;
  }

  static bool validateSyntax(vector<string>& tokens) {
    if (tokens.size() != 2) { return false; }
    return true;
  }

  vector<string> executeCommand() override {
    std::ofstream outFile(tokenizedCommand[0]);

    if (!outFile) { // check if file opened successfully
      stringstream errorMessage;
      errorMessage << "Error opening file: " << tokenizedCommand[0];
      return {errorMessage.str(), "400"};
    }

    outFile << tokenizedCommand[1]; // write the string to the file

    outFile.close(); // optional, file closes automatically when outFile goes out of scope
    stringstream message;
    message << "String successfully saved to " << tokenizedCommand[0];

    return {message.str(), "201"};
  }

private:
  // put your own method definitions here
};

