#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../Globals/GlobalFlags.h"

using namespace std;

// ===================={ Command Parent }====================
// Please don't modify this file unless you know what you are doing!

template <typename T>
class Command {

public:
    // setting virtual methods to 0/default tells the compiler that each child class will have its own implementation
    static bool validateSyntax(vector<string> tokens) { return T::validateSyntax(tokens); };
    virtual vector<string> executeCommand() = 0;
    static string returnManFilePath() { return T::returnManFilePath();}
    virtual ~Command() = default;

protected: // like private, but it allows child classes to inherit the methods
    // this will contain methods that you want to have shared across multiple commands
};

