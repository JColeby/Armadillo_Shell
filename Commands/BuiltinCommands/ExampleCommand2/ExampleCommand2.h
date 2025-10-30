#pragma once
#include "../../Command.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;


// ===================={ Command Template }====================
// TODO: DO NOT MODIFY THIS FILE DIRECTLY!!! Copy the contents of this file into a new header file
// TODO: Please add test code! This will make it easier to debug the shell when making changes that could impact other files
// I spent way too long trying to figure out how to get the test code to work for c++, so message me and I'll create a new file for tests
// might be a good idea to create the manual first so you know how to implement the class

class ExampleCommand2 : public Command<ExampleCommand2> { // Command class needs to be inherited in order to work!!!
    vector<string> tokenizedCommand;
    // add more class variables as needed.

public:
    explicit ExampleCommand2(vector<string>& tokens) {
        tokenizedCommand = tokens; // should save arguments in the order they were passed in
    }

    static string returnManFilePath() {
        // TODO: update the folder name
        return "BuiltinCommands/ExampleCommand2/Manual.txt";
    }

    static bool validateSyntax(vector<string>& tokens) {
        if (tokens.size() != 1) return false;
        // TODO: implement
        // this should be a simple validation so it can be used when validating
        // commands that are getting piped. More thorough validations can be done
        // in the execute command itself.
        // tokens should contain all of the command inputs the user provided
        // in order. However, It will not contain the command at the start.
        return true;
    }

    vector<string> executeCommand() override {
        // TODO: implement
        // Will assume validateSyntax was already called, but add error handling just in case
        return {"Test command 2 was able to execute successfully! Input received: " + tokenizedCommand[0], "200"};
    }

private:
    // put your own method definitions here
};

