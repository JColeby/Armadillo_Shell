#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../../Command.h"
#include "Manual.h"



// ===================={ Grep Command }====================
// TODO: add documentation to the manual file so James knows what flags and other info you implemented


class Help : public Command<Help> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit Help(vector<string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static string returnManText() {
    return HelpManual;
  }

  static bool validateSyntax(vector<string>& tokens) {
    return true;
  }

  vector<string> executeCommand() override {
    stringstream helpText;
    helpText << WHITE << "  - press ctrl+q to terminate any active command" << endl;
    helpText << "  - run 'exit' to close the shell" << endl;
    helpText << "  - run 'cmd' to view a list of all commands" << endl;
    helpText << "  - run 'man <command>' to view additional info about a specific command" << RESET_TEXT;
    return {helpText.str(), "200"};
  }

private:
  // put your own method definitions here
};

