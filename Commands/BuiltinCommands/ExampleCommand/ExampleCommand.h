#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../../Command.h"

using namespace std;


// ===================={ Command Template }====================
// TODO: DO NOT MODIFY THIS FILE DIRECTLY!!! Copy the contents of this file into a new header file
// may be a good idea to create the manual first so you know how to implement the class
// TODO: Please add test code! This will make it easier to debug the shell when making changes that could impact other files
// I spent way too long trying to figure out how to get the test code to work for c++, so message me and I'll create a new file for tests


class ExampleCommand : public Command<ExampleCommand> { // Command NEEDS TO BE INHERITED
    vector<string> tokenizedCommand;
    // add more class variables as needed.

public:
    explicit ExampleCommand(vector<string>& tokens) {
        tokenizedCommand = tokens; // should save arguments in the order they were passed in
    }

    static string returnManFilePath() {
        // TODO: update the parent folder name
        return "BuiltinCommands/<ExampleCommand>/Manual.txt";
    }


    static bool validateSyntax(vector<string>& tokens) {
        // TODO: implement
        // tokens should contain all of the command inputs the user provided
        // in order. However, It WON'T contain the command at the start.
        // we can add methods to the Command class that you can use across multiple instances of objects
        // (example: method that tests if the user gave a valid filepath)
        return true;
    }

    vector<string> executeCommand() override {
        // TODO: implement
        // Will assume validateSyntax was already called, but add error handling just in case
        return {"command output", "error code as a string"}; ;;
    }

private:
    // put your own method definitions here
};

