#pragma once
#include "../../Command.h"
#include "../../importAllCommands.h"
#include "../Help/Help.h"
#include "../PrintWorkingDirectory/PrintWorkingDirectory.h"
#include "../Process/Process.h"
#include "../Remove/Remove.h"
#include "../Run/Run.h"
#include "../SaveTo/SaveTo.h"
#include "../SystemInfo/SystemInfo.h"
#include "Manual.h"
#include "../Touch/Touch.h"

#include <string>
#include <vector>

// ===================={ Man Command }====================
// TODO: add documentation to the manual file so James knows what flags and other info you implemented


class Man : public Command<Man> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit Man(vector<string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static string returnManText() {
    return ManManual;
  }

  static bool validateSyntax(vector<string>& tokens) {
    if (tokens.size() == 1) { return true; }
    return false;
  }

  vector<string> executeCommand() override {
    //return { "'" + tokenizedCommand[0] + "' is not a valid command", "4"};
    const string command = tokenizedCommand[0];
    // if (command == "clear") { system("cls"); return {"Screen Cleared", "201"}; }
    //if (command == "example") { return validateAndExecuteCommand<ExampleCommand>(tokenizedInput, validateOnly); }
    if (command == "example") { return {ExampleCommand::returnManText(), "200"};}
    if (command == "example2") { return {ExampleCommand2::returnManText(), "200"}; }
    if (command == "run") { return {Run::returnManText(), "200"}; }
    if (command == "edit") { return {Edit::returnManText(), "200"}; }
    if (command == "process" or command == "ps") { return {Process::returnManText(), "200"}; }
    if (command == "find") { return {Find::returnManText(), "200"}; }
    if (command == "changeDirectory" or command == "cd") { return {ChangeDirectory::returnManText(), "200"}; }
    if (command == "copy" or command == "cp") { return {Copy::returnManText(), "200"}; }
    if (command == "echo") { return {Echo::returnManText(), "200"}; }
    if (command == "grep") { return {Grep::returnManText(), "200"}; }
    if (command == "list" or command == "ls") { return {List::returnManText(), "200"}; }
    if (command == "makeDirectory" or command == "mkdir") { return {MakeDirectory::returnManText(), "200"}; }
    if (command == "manual" or command == "man") { return {Man::returnManText(), "200"}; }
    if (command == "printWorkingDirectory" or command == "pwd") { return {PrintWorkingDirectory::returnManText(), "200"}; }
    if (command == "find") { return {Find::returnManText(), "200"}; }
    if (command == "sys" or command == "systemInfo") { return {SystemInfo::returnManText(), "200"}; }
    if (command == "date") { return {Date::returnManText(), "200"}; }
    if (command == "cmd") { return {Cmd::returnManText(), "200"}; }
    if (command == "cat") { return {Cat::returnManText(), "200"}; }
    if (command == "help") { return {Help::returnManText(), "200"}; }
    if (command == "touch") { return {Touch::returnManText(), "200"}; }
    if (command == "rm" or command == "remove") { return {Remove::returnManText(), "200"}; }
    if (command == "saveto" or command == "saveTo") { return {SaveTo::returnManText(), "200"}; }
    return { "'" + command + "' is not a valid command", "400"};
  }

private:
  // put your own method definitions here
};

