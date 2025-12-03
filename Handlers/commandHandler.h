#pragma once
#include <string>
#include <iostream>
#include <vector>


std::vector<std::string> commandHandler(std::vector<std::string>& tokenizedInput, bool validateOnly);


#include "../Commands/validateAndExecuteCommand.h"
#include "../Commands/importAllCommands.h"