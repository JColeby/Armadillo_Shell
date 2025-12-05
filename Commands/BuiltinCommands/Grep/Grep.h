#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../../Command.h"
#include "Manual.h"



// ===================={ Grep Command }====================
// TODO: add documentation to the manual file so James knows what flags and other info you implemented


class Grep : public Command<Grep> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit Grep(vector<string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static string returnManText() {
    return GrepManual;
  }

  static bool validateSyntax(vector<string>& tokens) {
    // TODO: implement
    // this should be a simple validation so it can be used when validating
    // commands that are getting piped. More thorough validations can be done
    // in the execute command itself.
    // tokens should contain all of the command inputs the user provided
    // in order. However, It will not contain the command at the start.
    if(tokens.size()==2 || tokens.size()==3){
      return true;
    }
    else{
      return false;
    }
  }

  vector<string> executeCommand() override {
    // TODO: implement
    // Will assume validateSyntax was already called, but add error handling just in case
    // std::string stringToFind = tokenizedCommand[0];
    // std::string whereToLook = tokenizedCommand[1];
    // std::string whereToStore = tokenizedCommand[2];
    // bool outfile=false;
    //
    // std::ifstream inputFile(whereToLook);
    // if(!inputFile){
    //   return {"Unable to open input file", "400"};
    // }
    // if(whereToStore != NULL){
    //   std::ofstream outputFile(whereToStore, std::ofstream);
    //   if(!file.is_open()) {
    //     return {"Unable to open output file", "400"};
    //   }
    //   outfile=true;
    // }
    // try{
    //   string line;
    //   while(std::getline(inputFile, line)){
    //     if(line.find(stringToFind) != string::npos){
    //       outputFile << line << std::endl;
    //       std::cout << line << std::endl;
    //     }
    //   }
    // }
    return {"Done!", "200"};
  }

private:
  // put your own method definitions here
};

