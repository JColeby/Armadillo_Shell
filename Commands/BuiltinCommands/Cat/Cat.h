#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../../Command.h"
#include "Manual.h"



// ===================={ Cat Command }====================
// TODO: add documentation to the manual file so James knows what flags and other info you implemented


class Cat : public Command<Cat> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit Cat(vector<string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static string returnManText() {
    return CatManual;
  }

  static bool validateSyntax(vector<string>& tokens) {
    if (tokens.size() == 1) { return true; }
    return false;
  }

  vector<string> executeCommand() override {
    // TODO: implement
    // Will assume validateSyntax was already called, but add error handling just in case
    return {"Not Implemented", "500"};
  }

private:
  // put your own method definitions here
};

