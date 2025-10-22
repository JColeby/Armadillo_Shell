#pragma once
#include <string>
#include <iostream>
#include <vector>

using namespace std;

// ===================={ Command Template }====================

class Command {
    vector<string> tokenizedCommand;

public:
    Command(vector<string>& tokenizedCommand);

    bool validateSyntax() {
        return false;
    }

    vector<string> executeCommand() {
        return {"command output", "error code as a string"}; ;;
    }
};

