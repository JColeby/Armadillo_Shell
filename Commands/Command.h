#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../TerminalFormatting.h"
#include "../Globals/GlobalFlags.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::flush;
using std::runtime_error;
using std::exception;
using std::ifstream;
using std::ofstream;
using namespace VT;

// ===================={ Command Parent }====================
// TODO: Please reach out to me (Jonathan) before touching this file!!!
// This file contains critical code that is used across ALL commands
// Please don't be the reason multiple things break

template <typename T>
class Command {

public:
    // setting virtual methods to 0/default tells the compiler that each child class will have its own implementation
    static bool validateSyntax(vector<string> tokens) { return T::validateSyntax(tokens); };
    virtual vector<string> executeCommand() = 0;
    static string returnManText() { return T::returnManFilePath(); }
    virtual ~Command() = default;

protected: // like private, but it allows child classes to inherit the methods
    // this will contain methods that you want to have shared across multiple commands
    // Only put code here that is in a final state, as multiple files will rely on it
    // once you put something here, don't change it (especially after you push/merge changes)
};

