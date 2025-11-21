#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include "../../Command.h"
#include "Manual.h"

using std::chrono::system_clock;


// ===================={ date Command }====================


class Date : public Command<Date> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit Date(vector<string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static string returnManText() {
    return DateManual;
  }

  static bool validateSyntax(vector<string>& tokens) {
    if (tokens.empty()) { return true; }
    return false;
  }

  vector<string> executeCommand() override {
    std::chrono::time_point<system_clock> systemTime = system_clock::now();
    std::time_t dateAndTime = system_clock::to_time_t(systemTime);
    return {std::ctime(&dateAndTime), "200"};
  }

private:
  // put your own method definitions here
};

