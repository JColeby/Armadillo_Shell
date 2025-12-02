#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "../../Command.h"
#include "Manual.h"




// ===================={ Remove Command }====================
// TODO: add documentation to the manual file so James knows what flags and other info you implemented


class Copy : public Command<Copy> { // Command class needs to be inherited in order to work!!!
  std::vector<std::string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit Copy(std::vector<std::string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static std::string returnManText() {
    return CopyManual;
  }

  static bool validateSyntax(std::vector<std::string>& tokens) {
    // TODO: implement
    // this should be a simple validation so it can be used when validating
    // commands that are getting piped. More thorough validations can be done
    // in the execute command itself.
    // tokens should contain all of the command inputs the user provided
    // in order. However, It will not contain the command at the start.
    return tokens.size() == 2;
  }

  std::vector<std::string> executeCommand() override {
    // TODO: implement
    // Will assume validateSyntax was already called, but add error handling just in case
    if (!validateSyntax(tokenizedCommand)) {
      return { "copy: usage: copy <source> <destination>", "400"};
    }
    std::string src = tokenizedCommand[0];
    std::string dst = tokenizedCommand[1];

    std::ifstream in(src, std::ios::binary);
    if (!in.is_open()) {
      return { "copy: could not open source file: " + src, "404"};
    }

    std::ofstream out(dst, std::ios::binary | std::ios::trunc);
    if (!out.is_open()) {
      return { "copy: could not open destination file: " + dst, "500"};
    }

    out << in.rdbuf();

    if (!out.good()) {
      return { "copy: error writing to destination file: " + dst, "500"};
    }

    return { "copy: '" + src + "' -> '" + dst + "'", "201"};
  }

private:
  // put your own method definitions here
};