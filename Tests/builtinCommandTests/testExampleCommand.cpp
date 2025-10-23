
#include <iostream>
#include <cassert>
#include "../../Commands/BuiltinCommands/ExampleCommand/ExampleCommand.h" // replace with your class

int main() {
    vector<string> commandInputToTest = {"-flag", "userInput.txt"};

    bool isValid = ExampleCommand::validateSyntax(commandInputToTest);
    assert(isValid);
    ExampleCommand command = ExampleCommand(commandInputToTest);
    vector<string> commandOutput = command.executeCommand();
    vector<string> desiredOutput = {"command output", "error code as a string"};
    assert(commandOutput == desiredOutput);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}