#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "Command.h"
#include "importAllCommands.h"

using namespace std;

template <typename T>
std::unique_ptr<T> createInstance(vector<string>& tokenizedInput) {
    return std::make_unique<T>(tokenizedInput); // T must be a class type that takes in a vector of strings
}

// syntax for calling: validateAndExecuteCommand<ClassType>(tokenizedInput);
template <typename T>
vector<string> validateAndExecuteCommand(vector<string>& tokenizedInput) {
    vector<string> input = {};
    for (int i = 1; i < tokenizedInput.size(); i++) { input.push_back(tokenizedInput[i]); }
    if (!T::validateSyntax(input)) {
        return {"Invalid command syntax/input. Information about correct syntax can be found by running 'man " + tokenizedInput[0] + "'", "4"};
    }
    static_assert(is_base_of<Command<T>, T>::value, "T must inherit Command"); // will throw a compile-time error if T doesn't inherit Command
    unique_ptr<T> instance = createInstance<T>(input);
    if (instance) { // Checks if creation of desired command object was successful
        if (auto basePtr = dynamic_cast<Command<T>*>(instance.get())) {
            return basePtr->executeCommand();
        }
        return {"Command Class definition doesn't inherit Command or lacks required methods", "error"};
    }
    return {"Failed to create command object", "error"};
}