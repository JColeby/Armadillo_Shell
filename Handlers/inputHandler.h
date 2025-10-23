#pragma once
#include <string>
#include <iostream>
#include <vector>

#include "commandHandler.h"
#include "pipeHandler.h"

using namespace std;

int inputHandler(const string& userInput) ;

vector<string> tokenizeInput(string inputString) ;

