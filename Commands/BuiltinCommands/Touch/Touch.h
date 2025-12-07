#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../../Command.h"
#include "Manual.h"
#include <fstream>



// ===================={ Command Template }====================
// TODO: DO NOT MODIFY THIS FILE DIRECTLY!!! Copy the contents of this file into a new header file
// TODO: Please add test code! This will make it easier to debug the shell when making changes that could impact other files
// I spent way too long trying to figure out how to get the test code to work for c++, so message me and I'll create a new file for tests
// might be a good idea to create the manual first so you know how to implement the class

class Touch : public Command<Touch> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit Touch(vector<string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static string returnManText() {
    // TODO: update the global variable name
    return TouchManual;
  }

  static bool validateSyntax(vector<string>& tokens) {
    if (tokens.size() == 1) { return true;}
    return false;
  }

  vector<string> executeCommand() override {
    std::ofstream outputFile(tokenizedCommand[0]);
    if (outputFile.is_open()) { outputFile << ""; }
    else { return {"Failed to create new file!", "500"}; }
    return {"Created New File: " + tokenizedCommand[0] , "201"};
  }

private:
  // put your own method definitions here
};

