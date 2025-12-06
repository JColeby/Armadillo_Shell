#pragma once

// this will contain import statements for every builtin command. Use to reduce the amount of imports in files that need access to all commands
// #import "BuiltinCommands/<CommandFolder>/<ClassName>"
#include "../TerminalFormatting.h"
#include "BuiltinCommands/Cat/Cat.h"
#include "BuiltinCommands/ChangeDirectory/ChangeDirectory.h"
#include "BuiltinCommands/Cmd/Cmd.h"
#include "BuiltinCommands/Copy/Copy.h"
#include "BuiltinCommands/Date/Date.h"
#include "BuiltinCommands/Echo/Echo.h"
#include "BuiltinCommands/Edit/Edit.h"
#include "BuiltinCommands/ExampleCommand/ExampleCommand.h"
#include "BuiltinCommands/ExampleCommand2/ExampleCommand2.h"
#include "BuiltinCommands/Find/Find.h"
#include "BuiltinCommands/Grep/Grep.h"
#include "BuiltinCommands/List/List.h"
#include "BuiltinCommands/MakeDirectory/MakeDirectory.h"
#include "BuiltinCommands/Man/Man.h"
#include "BuiltinCommands/PrintWorkingDirectory/PrintWorkingDirectory.h"
#include "BuiltinCommands/Process/Process.h"
#include "BuiltinCommands/Run/Run.h"
#include "BuiltinCommands/SystemInfo/SystemInfo.h"
#include "BuiltinCommands/Help/Help.h"
#include "BuiltinCommands/Remove/Remove.h"
#include "BuiltinCommands/SaveTo/SaveTo.h"


using std::string;
using std::vector;
using std::cout;
using std::endl;
using namespace VT;
