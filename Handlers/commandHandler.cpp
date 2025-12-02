#include "commandHandler.h"


// calls validateAndExecuteCommand and passes in the class associated with the command
vector<string> commandHandler(vector<string>& tokenizedInput, bool validateOnly) {
    const string command = tokenizedInput[0];
    if (command == "clear") { system("cls"); return {"Screen Cleared", "201"}; }
    if (command == "example") { return validateAndExecuteCommand<ExampleCommand>(tokenizedInput, validateOnly); }
    if (command == "example2") { return validateAndExecuteCommand<ExampleCommand2>(tokenizedInput, validateOnly); }
    if (command == "run") { return validateAndExecuteCommand<Run>(tokenizedInput, validateOnly); }
    if (command == "edit") { return validateAndExecuteCommand<Edit>(tokenizedInput, validateOnly); }
    if (command == "process" or command == "ps") { return validateAndExecuteCommand<Process>(tokenizedInput, validateOnly); }
    if (command == "find") { return validateAndExecuteCommand<Find>(tokenizedInput, validateOnly); }
    if (command == "changeDirectory" or command == "cd") { return validateAndExecuteCommand<ChangeDirectory>(tokenizedInput, validateOnly); }
    if (command == "copy" or command == "cp") { return validateAndExecuteCommand<Copy>(tokenizedInput, validateOnly); }
    if (command == "echo") { return validateAndExecuteCommand<Echo>(tokenizedInput, validateOnly); }
    if (command == "grep") { return validateAndExecuteCommand<Grep>(tokenizedInput, validateOnly); }
    if (command == "list" or command == "ls") { return validateAndExecuteCommand<List>(tokenizedInput, validateOnly); }
    if (command == "makeDirectory" or command == "mkdir") { return validateAndExecuteCommand<MakeDirectory>(tokenizedInput, validateOnly); }
    if (command == "manual" or command == "man") { return validateAndExecuteCommand<Man>(tokenizedInput, validateOnly); }
    if (command == "printWorkingDirectory" or command == "pwd") { return validateAndExecuteCommand<PrintWorkingDirectory>(tokenizedInput, validateOnly); }
    if (command == "find") { return validateAndExecuteCommand<Find>(tokenizedInput, validateOnly); }
    if (command == "sys" or command == "systemInfo") { return validateAndExecuteCommand<SystemInfo>(tokenizedInput, validateOnly); }
    if (command == "date") { return validateAndExecuteCommand<Date>(tokenizedInput, validateOnly); }
    if (command == "cmd") { return validateAndExecuteCommand<Cmd>(tokenizedInput, validateOnly); }
    if (command == "cat") { return validateAndExecuteCommand<Cat>(tokenizedInput, validateOnly); }

    return { "'" + tokenizedInput[0] + "' is not a valid command", "4"};
}