#pragma once
#include <string>

const std::string SystemInfoManual = R"(
This file will contain the text that shows up when you call the manual command for this class.
All the code/files for the command should remain inside its own folder.
The name of this file should always be Manual.txt
When writing the manual, make sure to include valid flags

for whoever is creating the manual command, import "importAllCommands.h" so you don't have to define each command,
this will be useful for getting the manual file path

)";