
#include <iostream>
#include <cassert>
#include "../inputLoop.h"
#include "../Commands/validateAndExecuteCommand.h"
#include "../Commands/importAllCommands.h"

int main() {
    vector<string> commandInputTest = {"-flag", "userInput.txt"};
    vector<string> commandOutput = validateAndExecuteCommand<ExampleCommand>(commandInputTest);
    vector<string> desiredOutput = {"command output", "error code as a string"};
    assert(commandOutput == desiredOutput);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

