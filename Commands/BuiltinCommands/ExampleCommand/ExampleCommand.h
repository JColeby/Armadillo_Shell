#pragma once
#include <string>
#include <iostream>
#include <vector>

#include "../../Command.h"

using namespace std;

// ===================={ Command Template }====================

class ExampleCommand : public Command { // Command NEEDS TO BE INHERITED
    vector<string> tokenizedCommand;

public:
    ExampleCommand(vector<string>& tokenizedCommand); // should save arguments in the order they were passed in

    bool validateSyntax() {
        return false;
    }

    vector<string> executeCommand() {
        return {"command output", "error code as a string"}; ;;
    }
};

