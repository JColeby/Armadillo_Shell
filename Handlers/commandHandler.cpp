#include  "commandHandler.h"


vector<string> commandHandler(vector<string>& tokenizedInput) {
    const string command = tokenizedInput[0];
    if (command == "clear") { system("cls"); return {"Screen Cleared", "201"}; }
    if (command == "example") { return validateAndExecuteCommand<ExampleCommand>(tokenizedInput); }
    if (command == "example2") { return validateAndExecuteCommand<ExampleCommand2>(tokenizedInput); }
    if (command == "run") { return validateAndExecuteCommand<Run>(tokenizedInput); }
    if (command == "edit") { return validateAndExecuteCommand<Edit>(tokenizedInput); }
    return { "'" + tokenizedInput[0] + "' is not a valid command", "4"};
}