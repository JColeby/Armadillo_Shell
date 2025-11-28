#pragma once
#include "../../../Handlers/inputHandler.h"
#include "../../../Handlers/pipeHandler.h"
#include "../../../Handlers/commandHandler.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "../../Command.h"
#include "Manual.h"




// ===================={ Run Command }====================


class Run : public Command<Run> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  std::map<string, string> varMap;
  // add more class variables as needed.

public:
    explicit Run(vector<string>& tokens) {
        tokenizedCommand = tokens; // should save arguments in the order they were passed in
    }

    static string returnManText() {
        return RunManual;
    }

    static bool validateSyntax(vector<string>& tokens) {
        // TODO: implement
        // this should be a simple validation so it can be used when validating
        // commands that are getting piped. More thorough validations can be done
        // in the execute command itself.
        // tokens should contain all of the command inputs the user provided
        // in order. However, It will not contain the command at the start.
        if(!tokens.empty()){ return true; }
        else { return false; }
    }

    vector<string> executeCommand() override {
        // TODO: implement
        // Will assume validateSyntax was already called, but add error handling just in case
        std::ifstream inputFile(tokenizedCommand[0]);
        if(!inputFile){
            return {"Unable to open file", "400"};
        }

        try {
          string line;
          while(std::getline(inputFile, line)){
            interpretLine(line);
          }
        }
        catch (const exception& e) {
          return {e.what(), "500"};
        }

        return {"Script ran successfully", "201"};
    }

private:
    void interpretLine(string& line) {
      containsPipe = false;
      vector<string> tokenizedLine = tokenizeInput(line, false);

      if (tokenizedLine.empty()) return;
      if (!tokenizedLine.empty() && !tokenizedLine[0].empty() && tokenizedLine[0][0] == '#') return;


      if (tokenizedLine.size() > 2 and tokenizedLine[0][0] == '$' and tokenizedLine[1] == "=") {
        vector<string> input; // removing the first string from the vector
        for (int i = 2; i < tokenizedLine.size(); i++) { input.push_back(tokenizedLine[i]); }
        vector<string> result = evaluate(input);
        if (result[1][0] != '2') {
          throw runtime_error("Script Error, variable malformed (" + result[1] + ") : " + result[0]);
        }
        varMap[tokenizedLine[0]] = result[0];
        return;
      }
      displayOutput(evaluate(tokenizedLine));

    }


    vector<string> evaluate(vector<string> statement) {
      if (statement.empty()) return {"Forgot to set the variable equal to something silly", "400" };
      // check if the first token is a valid command. If so, execute it and return the result
      if (statement[0][0] != '$' and statement[0][0] != '"') {
        replaceVarAndRemoveQuotes(statement);
        vector<string> evaluatedCommand;
        if (containsPipe) { evaluatedCommand = pipeHandler(statement); }
        else { evaluatedCommand = commandHandler(statement); }
        return evaluatedCommand;
      }

      replaceVarAndRemoveQuotes(statement);
      string result = statement[0];
      if (statement.size() % 2 == 0) {
        throw runtime_error("Improper string addition");
      }
      for (int i = 1; i < statement.size(); i += 2) {
        if (statement[i] != "+") { throw runtime_error("Improper string addition"); }
        result += statement[i+1];
      }
      return {result, "200"};

    }


    void replaceVarAndRemoveQuotes(vector<string>& statement) {
      for (string & word : statement) {
        if (word[0] == '$') {
          if (varMap.find(word) == varMap.end()) {
            throw runtime_error(word + " is not defined");
          }
          word = varMap[word];
        }
        if (!word.empty() && word[0] == '"') {
          if (word.size() >= 2 && word.back() == '"') {
            word = word.substr(1, word.size() - 2);
          } else {
            throw runtime_error("Malformed quoted string");
          }
        }
      }
    }

    // put your own method definitions here
};



// syntax
// $var = command here
// $var2 = "string goes here" + $var
