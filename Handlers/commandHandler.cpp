#include  "commandHandler.h"


vector<string> commandHandler(vector<string>& tokenizedInput) {
    const string command = tokenizedInput[0];
    if (command == "clear") { system("cls"); return {"Screen Cleared", "201"}; }
    if (command == "example") { return validateAndExecuteCommand<ExampleCommand>(tokenizedInput); }
    if (command == "example2") { return validateAndExecuteCommand<ExampleCommand2>(tokenizedInput); }
    if (command == "run") { return validateAndExecuteCommand<Run>(tokenizedInput); }
    if (command == "edit") { return validateAndExecuteCommand<Edit>(tokenizedInput); }
    if (command == "process" or command == "ps") { return validateAndExecuteCommand<Process>(tokenizedInput); }
    if (command == "find") { return validateAndExecuteCommand<Find>(tokenizedInput); }
    if (command == "changeDirectory" or command == "cd") { return validateAndExecuteCommand<ChangeDirectory>(tokenizedInput); }
    if (command == "copy" or command == "cp") { return validateAndExecuteCommand<Copy>(tokenizedInput); }
    if (command == "echo") { return validateAndExecuteCommand<Echo>(tokenizedInput); }
    if (command == "grep") { return validateAndExecuteCommand<Grep>(tokenizedInput); }
    if (command == "list" or command == "ls") { return validateAndExecuteCommand<List>(tokenizedInput); }
    if (command == "makeDirectory" or command == "mkdir") { return validateAndExecuteCommand<MakeDirectory>(tokenizedInput); }
    if (command == "manual" or command == "man") { return validateAndExecuteCommand<Man>(tokenizedInput); }
    if (command == "printWorkingDirectory" or command == "pwd") { return validateAndExecuteCommand<PrintWorkingDirectory>(tokenizedInput); }
    if (command == "find") { return validateAndExecuteCommand<Find>(tokenizedInput); }
    if (command == "sys" or command == "systemInfo") { return validateAndExecuteCommand<SystemInfo>(tokenizedInput); }
    if (command == "date") { return validateAndExecuteCommand<Date>(tokenizedInput); }
    return { "'" + tokenizedInput[0] + "' is not a valid command", "4"};
}