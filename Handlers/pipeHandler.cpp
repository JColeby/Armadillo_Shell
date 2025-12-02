#include "../Commands/importAllCommands.h"
#include "commandHandler.h"

vector<string> pipeHandler(const vector<string>& tokenizedInput) {
    vector<vector<string>> individualCommands = {};
    vector<string> commandTokens;
    for (const auto& token : tokenizedInput) {
      if (token == "|") {
        individualCommands.push_back(commandTokens);
        commandTokens.clear();
      }
      else {
        commandTokens.push_back(token);
      }
    }
    individualCommands.push_back(commandTokens);

    // validating syntax
    vector<string> validateOutput;
    for (int i = 0; i < individualCommands.size(); i++) {
      vector<string> currentCommand = individualCommands[i];
      if (i != 0) {
        currentCommand.push_back("$!PLACEHOLDER!$");
      }
      validateOutput = commandHandler(currentCommand, true);
      if (validateOutput[1][0] != '2') { return {"Invalid command syntax/input. Information about correct syntax can be found by running 'man " + individualCommands[i][0] + "'", "4"}; }
    }

    // running commands
    vector<string> output;
    for (int i = 0; i < individualCommands.size(); i++) {
      vector<string> currentCommand = individualCommands[i];
      if (i != 0) {
        currentCommand.push_back(output[0]);
      }
      output = commandHandler(currentCommand, false);
      if (output[1][0] != '2') { return {"Error returned from input being piped to another command. \n  " + output[0], output[1]}; }
    }

    return output;
}