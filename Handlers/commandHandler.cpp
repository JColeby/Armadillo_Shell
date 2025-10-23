#include  "commandHandler.h"

vector<string> commandHandler(vector<string>& tokenizedInput) {
    return validateAndExecuteCommand<ExampleCommand>(tokenizedInput);;
}