#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <windows.h>
#include <thread>
#include <chrono>

#include "commandHandler.h"
#include "pipeHandler.h"

using namespace std;

void printBadCommandResult(const vector<string>& commandOutput);

void displayOutput(const vector<string>& commandOutput);

vector<string> tokenizeInput(const string& inputString);

void inputHandler(const string& userInput) ;
