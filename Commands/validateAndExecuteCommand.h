#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "Command.h"
#include "importAllCommands.h"

using std::is_base_of;
using std::unique_ptr;


template <typename T>
vector<string> validateAndExecuteCommand(vector<string>& tokenizedInput, bool validateOnly) {
  vector<string> input; // removing the first string from the vector
  for (int i = 1; i < tokenizedInput.size(); i++) { input.push_back(tokenizedInput[i]); }

  if (!T::validateSyntax(input)) { // validating syntax
    return {"Invalid command syntax/input. Information about correct syntax can be found by running 'man " + tokenizedInput[0] + "'", "4"};
  }
  if (validateOnly) { return {"valid syntax", "200"}; }

  // make sure the class passed in inherits the Command class
  static_assert(is_base_of<Command<T>, T>::value, "T must inherit CommandBase");

  unique_ptr<T> instance = std::make_unique<T>(input); // creates a new object instance
  if (!instance) { return {"Failed to create command object", "500"}; } // if no object was created
  Command<T>* basePtr = instance.get();
  return basePtr->executeCommand();
}

