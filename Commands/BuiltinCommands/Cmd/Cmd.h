#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../../Command.h"
#include "Manual.h"

// ===================={ Cmd Command }====================
// TODO: add documentation to the manual file so James knows what flags and other info you implemented


class Cmd : public Command<Cmd> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit Cmd(vector<string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static string returnManText() {
    return CmdManual;
  }

  static bool validateSyntax(vector<string>& tokens) {
    return tokens.empty();
  }

  vector<string> executeCommand() override {
    stringstream commands;
    commands << WHITE << "File and Directory Commands:  " << RESET_TEXT;
    commands << "cat, cd, cp, edit, ls, mkdir, pwd, rm, saveto" << endl;
    commands << WHITE << "System Commands:              " << RESET_TEXT;
    commands << "date, find, ps, run, sys" << endl;
    commands << WHITE << "Informative Commands:         " << RESET_TEXT;
    commands << "cmd, help, man" << endl;
    commands << WHITE << "Other Commands:               " << RESET_TEXT;
    commands << "clear, echo, exit, grep";
    return {commands.str(), "200"};
  }

private:
  // put your own method definitions here
};

