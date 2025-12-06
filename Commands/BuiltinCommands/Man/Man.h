#pragma once
#include "../../Command.h"
#include "../../importAllCommands.h"
#include "../Help/Help.h"
#include "../PrintWorkingDirectory/PrintWorkingDirectory.h"
#include "../Process/Process.h"
#include "../Remove/Remove.h"
#include "../Run/Run.h"
#include "../SaveTo/SaveTo.h"
#include "../SystemInfo/SystemInfo.h"
#include "Manual.h"

#include <string>
#include <vector>

// ===================={ Man Command }====================
// TODO: add documentation to the manual file so James knows what flags and other info you implemented


class Man : public Command<Man> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit Man(vector<string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static string returnManText() {
    return ManManual;
  }

  static bool validateSyntax(vector<string>& tokens) {
    if (tokens.size() == 1) { return true; }
    return false;
  }

  vector<string> executeCommand() override {
    return { "'" + tokenizedCommand[0] + "' is not a valid command", "4"};
  }

private:
  // put your own method definitions here
};

