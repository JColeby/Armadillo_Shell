#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../Commands/validateAndExecuteCommand.h"
#include "../Commands/importAllCommands.h"

using namespace std;

vector<string> commandHandler(vector<string>& tokenizedInput);