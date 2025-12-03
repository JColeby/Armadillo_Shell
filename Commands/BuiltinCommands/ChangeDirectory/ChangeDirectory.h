#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>     // chdir, getcwd
#include <limits.h>     // PATH_MAX
#include "../../Command.h"
#include "Manual.h"

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

  vector<string> executeCommand() override {
    if (!validateSyntax(tokenizedCommand)) {
      return { "cd: too many arguments", "400" };
    }

    const char* targetPath = nullptr;

    // Case: "cd" with NO arguments → go HOME
    if (tokenizedCommand.empty()) {
      targetPath = getenv("HOME");
      if (!targetPath) {
        return { "cd: HOME not set", "500" };
      }
    }
    // Case: "cd <path>"
    else {
      targetPath = tokenizedCommand[0].c_str();
    }

    if (chdir(targetPath) != 0) {
      return { "cd: no such file or directory: " + tokenizedCommand[0], "404" };
    }

    // Return the new directory
    char cwd[PATH_MAX];
    if (!getcwd(cwd, sizeof(cwd))) {
      return { "cd: changed directory but failed to get working directory", "500" };
    }

    return { string(cwd), "201" };
  }
};