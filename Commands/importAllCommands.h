#pragma once

// this will contain import statements for every builtin command. Use to reduce the amount of imports in files that need access to all commands
// #import "BuiltinCommands/ExampleCommand/<ClassName>"
#include "../TerminalFormatting.h"
#include "BuiltinCommands/ExampleCommand/ExampleCommand.h"
#include "BuiltinCommands/ExampleCommand2/ExampleCommand2.h"
#include "BuiltinCommands/Run/Run.h"
#include "BuiltinCommands/Edit/Edit.h"
using std::string;
using std::vector;
using std::cout;
using std::endl;
using namespace VT;