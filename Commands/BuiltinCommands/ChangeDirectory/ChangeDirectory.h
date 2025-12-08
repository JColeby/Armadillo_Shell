#pragma once
#include <string>
#include <iostream>
#include <vector>     
#include <limits.h>     // PATH_MAX
#include "../../Command.h"
#include "Manual.h"

namespace fs = std::filesystem;

// ===================={ Change Directory Command }====================

class ChangeDirectory : public Command<ChangeDirectory> {
  std::vector<std::string> tokenizedCommand;

public:
  explicit ChangeDirectory(std::vector<std::string>& tokens) {
    tokenizedCommand = tokens;
  }

  static std::string returnManText() {
    return ChangeDirectoryManual;
  }

  static bool validateSyntax(std::vector<std::string>& tokens) {
    return tokens.size() <= 1;
  }

  std::vector<std::string> executeCommand() override {
    if (tokenizedCommand.empty()) {
      return {"cd: missing directory,", "400"};
    }

    if (!validateSyntax(tokenizedCommand)) {
      return {"cd: usage: cd <directory>", "400"};
    }

    try {
      fs::current_path(tokenizedCommand[0]);          // attempt directory change
      return {fs::current_path().string(), "201"};    // success → return new location
    }
    catch (...) {
      return {"cd: no such file or directory", "404"}; // simple error message
    }
  }
};