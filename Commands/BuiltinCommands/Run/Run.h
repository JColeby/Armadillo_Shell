#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../../Command.h"
#include "Manual.h"



// ===================={ Command Template }====================
// TODO: DO NOT MODIFY THIS FILE DIRECTLY!!! Copy the contents of this file into a new header file
// TODO: Please add test code! This will make it easier to debug the shell when making changes that could impact other files
// I spent way too long trying to figure out how to get the test code to work for c++, so message me and I'll create a new file for tests
// might be a good idea to create the manual first so you know how to implement the class

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
        return true;
    }

    vector<string> executeCommand() override {
        // TODO: implement
        // Will assume validateSyntax was already called, but add error handling just in case
        return {"Test command was able to execute successfully!", "200"}; ;;
    }

private:
    // put your own method definitions here
};

