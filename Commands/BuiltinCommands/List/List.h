#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../../Command.h"
#include "Manual.h"
#include <filesystem>

namespace fs = std::filesystem;

// ===================={ List Command }====================
// TODO: add documentation to the manual file so James knows what flags and other info you implemented

class List : public Command<List>
{ // Command class needs to be inherited in order to work!!!
  std::vector<std::string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit List(std::vector<std::string> &tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static std::string returnManText() { return ListManual; }

  static bool validateSyntax(std::vector<std::string> &tokens) {
    // TODO: implement
    // this should be a simple validation so it can be used when validating
    // commands that are getting piped. More thorough validations can be done
    // in the execute command itself.
    // tokens should contain all of the command inputs the user provided
    // in order. However, It will not contain the command at the start.
    if (tokens.size() == 1 || tokens.empty()) { return true; }
    if (tokens.size() == 2) { return tokens[0] == "-a"; }
    return false;
  }

  std::vector<std::string> executeCommand() override {
    // TODO: implement
    // Will assume validateSyntax was already called, but add error handling just in case
    if (!validateSyntax(tokenizedCommand)) { return {"ls: too many arguments", "400"}; }

    bool showHidden = false;
    std::string pathStr = ".";

    if (tokenizedCommand.size() == 1) {
      if (tokenizedCommand[0] == "-a") { showHidden = true; }
      else { pathStr = tokenizedCommand[0]; }
    } else if (tokenizedCommand.size() == 2) {
      showHidden = true;
      pathStr = tokenizedCommand[1];
    }

    std::stringstream output;

    try {
      for (const auto &entry : fs::directory_iterator(pathStr)) {
        std::string name = entry.path().filename().string();

        // checking if the file is hidden by windows or has a '.' at the front of the file
        DWORD attributes = GetFileAttributesA(entry.path().string().c_str());
        if (attributes == INVALID_FILE_ATTRIBUTES) { continue; }
        if (!showHidden and (attributes & FILE_ATTRIBUTE_HIDDEN) != 0) { continue; }
        if (!showHidden and !name.empty() and name[0] == '.') { continue; }

        if (entry.is_directory()) { output << CYAN; }
        else if (endsWith(name, ".exe")) { output << YELLOW; }
        else if (endsWith(name, ".ardo")) { output << GREEN; }
        output << " " << name << RESET_TEXT << "\n";
      }
    }
    catch (const fs::filesystem_error &e) {
      return {"ls: cannot open directory: " + pathStr, "404"};
    }

    return {output.str(), "200"};
  }

private:
  bool endsWith(const std::string& s, const std::string end) {
    if (s.length() < end.length()) return false;
    return s.compare(s.length() - end.length(), end.length(), end) == 0;
  }
  // put your own method definitions here
};