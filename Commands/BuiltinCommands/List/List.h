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
  explicit List(std::vector<std::string> &tokens)
  {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static std::string returnManText()
  {
    return ListManual;
  }

  static bool validateSyntax(std::vector<std::string> &tokens)
  {
    // TODO: implement
    // this should be a simple validation so it can be used when validating
    // commands that are getting piped. More thorough validations can be done
    // in the execute command itself.
    // tokens should contain all of the command inputs the user provided
    // in order. However, It will not contain the command at the start.
    if (tokens.size() == 1 || tokens.empty())
    {
      return true;
    }
    if (tokens.size() == 2)
    {
      return tokens[0] == "-a";
    }
    return false;
  }

  std::vector<std::string> executeCommand() override
  {
    // TODO: implement
    // Will assume validateSyntax was already called, but add error handling just in case
    if (!validateSyntax(tokenizedCommand))
    {
      return {"ls: too many arguments", "400"};
    }

    bool showHidden = false;
    std::string pathStr = ".";

    if (tokenizedCommand.size() == 1)
    {
      if (tokenizedCommand[0] == "-a")
      {
        showHidden = true;
      }
      else
      {
        pathStr = tokenizedCommand[0];
      }
    }
    else if (tokenizedCommand.size() == 2)
    {
      showHidden = true;
      pathStr = tokenizedCommand[1];
    }

    std::string output;

    try
    {
      for (const auto &entry : fs::directory_iterator(pathStr))
      {
        std::string name = entry.path().filename().string();
        if (!showHidden && !name.empty() && name[0] == '.')
          continue;
        output += name + "\n";
      }
    }
    catch (const fs::filesystem_error &e)
    {
      return {"ls: cannot open directory: " + pathStr, "404"};
    }

    return {output, "200"};
  }

private:
  // put your own method definitions here
};