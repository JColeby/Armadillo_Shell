#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../../Command.h"
#include "Manual.h"
#include <filesystem>

namespace fs = std::filesystem;

// ===================={ Remove Command }====================
// TODO: add documentation to the manual file so James knows what flags and other info you implemented


class Remove : public Command<Remove> { // Command class needs to be inherited in order to work!!!
  std::vector<std::string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit Remove(std::vector<std::string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static std::string returnManText() {
    return RemoveManual;
  }

  static bool validateSyntax(std::vector<std::string>& tokens) {
    if (tokens.empty()) {
      return false;
    }
    if (tokens.size() == 1) {
      return true;
    }
    if (tokens.size() >= 2 && tokens[0] == "-r"){
      return true;
    }
    return false;
  }

  std::vector<std::string> executeCommand() override {
    if (!validateSyntax(tokenizedCommand)) {
      return {"Invalid usage", "500"};
    }

    bool recursive = false;
    std::vector<std::string> targets;

    if (tokenizedCommand[0] == "-r") {
      recursive = true;
      targets = std::vector<std::string>(tokenizedCommand.begin() + 1, tokenizedCommand.end());
    } else {
      targets = tokenizedCommand;
    }

    std::string output;

    for (const auto& target : targets) {
      fs::path path(target);

      if (!fs::exists(path)) {
        output += "rm: no such file or directory: " + target + "\n";
        continue;
      }

      if (fs::is_directory(path) && !recursive) {
        output += "rm: cannot remove directory: '" + target + "': is a directory\n";
        continue;
      }

      try {
        if (recursive) {
          fs::remove_all(path);
          output += "removed recursively: " + target + "\n";
        } else {
          if (fs::remove(path)) {
            output += "removed: " + target + "\n";
          } else {
            output += "rm: failed to remove: " + target + "\n";
          }
        }
      }

      catch (...) {
        output += "rm: failed to remove: " + target + "\n";
      }
    }
    return { output, "200"};
  }


private:
  // put your own method definitions here
};

