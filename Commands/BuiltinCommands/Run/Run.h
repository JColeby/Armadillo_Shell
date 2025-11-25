#pragma once
#include "../../../Handlers/inputHandler.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "../../Command.h"
#include "Manual.h"




// ===================={ Run Command }====================


class Run : public Command<Run> { // Command class needs to be inherited in order to work!!!
    vector<string> tokenizedCommand;
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
        if(tokens.size() != 0){
            return true;
        }
        else{
            return false;
        }
    }

    vector<string> executeCommand() override {
        // TODO: implement
        // Will assume validateSyntax was already called, but add error handling just in case
        std::ifstream inputFile(tokenizedCommand[0]);
        if(!inputFile){
            std::cerr << "Error opening file!" << std::endl;
            return {"Unable to open file", "400"};
        }

        std::string line;
        while(std::getline(inputFile, line)){
            inputHandler(line);
        }

        return {"Test command was able to execute successfully!", "200"}; ;;
    }

private:
    // put your own method definitions here
};

