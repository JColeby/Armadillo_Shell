#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../../Command.h"
#include "Manual.h"
#include <filesystem>
#include <fstream>

// ===================={ Cat Command }====================
// TODO: add documentation to the manual file so James knows what flags and other info you implemented

class Cat : public Command<Cat>
{ // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit Cat(std::vector<std::string> &tokens)
  {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static std::string returnManText()
  {
    return CatManual;
  }

  static bool validateSyntax(std::vector<std::string> &tokens)
  {
    if (tokens.size() == 1)
    {
      return true;
    }
    return false;
  }

  vector<string> executeCommand() override
  {
    // TODO: implement

    if (!validateSyntax(tokenizedCommand))
    {
      return {"cat: usage : cat <file>", "400"};
    }

    const std::string &filename = tokenizedCommand[0];

    std::ifstream in(filename);
    if (!in.is_open())
    {
      return {"cat: cannot open file " + filename + "" + "404"};
    }

    std::string output;
    std::string line;

    while (std::getline(in, line))
    {
      output += line;
      output.push_back('\n');
    }

    if (!output.empty())
    {
      output.pop_back();
    }
    // Will assume validateSyntax was already called, but add error handling just in case
    return {output, "200"};
  }

private:
  // put your own method definitions here
};
