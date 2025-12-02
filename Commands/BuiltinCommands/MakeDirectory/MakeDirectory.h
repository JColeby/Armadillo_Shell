#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../../Command.h"
#include "Manual.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>


// ===================={ Make Directory Command }====================
// TODO: add documentation to the manual file so James knows what flags and other info you implemented


class MakeDirectory : public Command<MakeDirectory> { // Command class needs to be inherited in order to work!!!
  std::vector<std::string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit MakeDirectory(std::vector<std::string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static std::string returnManText() {
    return MakeDirectoryManual;
  }

  static bool validateSyntax(std::vector<std::string>& tokens) {
    // TODO: implement
    // this should be a simple validation so it can be used when validating
    // commands that are getting piped. More thorough validations can be done
    // in the execute command itself.
    // tokens should contain all of the command inputs the user provided
    // in order. However, It will not contain the command at the start.
    return tokens.size() == 1;
  }

  std::vector<std::string> executeCommand() override {
    // TODO: implement
    // Will assume validateSyntax was already called, but add error handling just in case
    if (!validateSyntax(tokenizedCommand)) {
      return { "mkdir: usage: mkdir <directory>", "400"};
    }
    std::string dirname = tokenizedCommand[0];

    int result = mkdir(dirname.c_str());

    if (result != 0) {
      if (errno == EEXIST) {
        return { "mkdir: directory already exists: " + dirname, "400"};
      }
      else if (errno == ENOENT) {
        return { "mkdir: parent directory does not exist: " + dirname, "404"};
      }
      else {
        return { "mkdir: failed to create directory: " + dirname, "500"};
      }
    }

    return { "mkdir: created directory '" + dirname + "'", "201"};
  }

private:
  // put your own method definitions here
};