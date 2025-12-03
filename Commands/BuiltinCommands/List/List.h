#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../../Command.h"
#include "Manual.h"
#include <dirent.h>



// ===================={ List Command }====================
// TODO: add documentation to the manual file so James knows what flags and other info you implemented


class List : public Command<List> { // Command class needs to be inherited in order to work!!!
  std::vector<std::string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit List(std::vector<std::string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static std::string returnManText() {
    return ListManual;
  }

  static bool validateSyntax(std::vector<std::string>& tokens) {
    // TODO: implement
    // this should be a simple validation so it can be used when validating
    // commands that are getting piped. More thorough validations can be done
    // in the execute command itself.
    // tokens should contain all of the command inputs the user provided
    // in order. However, It will not contain the command at the start.
    if (tokens.size() == 1 or tokens.empty()) {
      return true;
    }
    return false;

  }

  std::vector<std::string> executeCommand() override {
    // TODO: implement
    // Will assume validateSyntax was already called, but add error handling just in case
    if (!validateSyntax(tokenizedCommand)) {
      return { "ls: too many arguments", "400"};
    }

    const char* path = nullptr;

    if (tokenizedCommand.empty()) {
      path = ".";
    } else {
      path = tokenizedCommand[0].c_str();
    }

    DIR* dir = opendir(path);
    if (!dir) {
      return { "ls: cannot open directory: " + tokenizedCommand[0], "404"};
    }

    std::string output;
    struct dirent* entry;

    while ((entry = readdir(dir)) != nullptr) {
      output += entry->d_name;
      output += "\n";
    }


    closedir(dir);

    return { output, "200 "};
  }

private:
  // put your own method definitions here
};