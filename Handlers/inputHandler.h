#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <windows.h>

#include "commandHandler.h"
#include "pipeHandler.h"

using namespace std;

void inputHandler(const string& userInput) ;

void displayOutput(vector<string> commandOutput);

vector<string> tokenizeInput(const string& inputString) ;

