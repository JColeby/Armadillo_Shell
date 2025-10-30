#include  "commandHandler.h"


vector<string> commandHandler(vector<string>& tokenizedInput) {
    const string command = tokenizedInput[0];
    if (command == "example") { return validateAndExecuteCommand<ExampleCommand>(tokenizedInput); }
    if (command == "example2") { return validateAndExecuteCommand<ExampleCommand2>(tokenizedInput); }
    if (command == "run") { return validateAndExecuteCommand<Run>(tokenizedInput); }
    return validateAndExecuteCommand<ExampleCommand>(tokenizedInput);

}