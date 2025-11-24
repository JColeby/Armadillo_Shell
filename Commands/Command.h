#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../TerminalFormatting.h"
#include "../Globals/GlobalFlags.h"

#include <iomanip>


using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::flush;
using std::runtime_error;
using std::exception;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using namespace VT;

// ===================={ Command Parent }====================
// TODO: Please reach out to me (Jonathan) before touching this file!!!
// This file contains critical code that is used across ALL commands
// Please don't be the reason multiple things break

template <typename T>
class Command {

public:
    // setting virtual methods to 0/default tells the compiler that each child class will have its own implementation
    static bool validateSyntax(vector<string> tokens) { return T::validateSyntax(tokens); };
    virtual vector<string> executeCommand() = 0;
    static string returnManText() { return T::returnManFilePath(); }
    virtual ~Command() = default;



protected: // like private, but it allows child classes to inherit the methods
    // this will contain methods that you want to have shared across multiple commands
    // Only put code here that is in a final state, as multiple files will rely on it
    // once you put something here, don't change it (especially after you push/merge changes)


    // takes in a double containing the size of data in bytes and converts it into a readable string
    static string bytesToReadableString(double size) {
      int unitsTracker = 1;
      while (size >= 1024) {
        unitsTracker += 1;
        size /= 1024;
      }
      string units;
      switch (unitsTracker) {
      case 1: units = "bytes"; break;
      case 2: units = "KB"; break;
      case 3: units = "MB"; break;
      case 4: units = "GB"; break;
      case 5: units = "TB"; break;
      default: // converting anything larger than a TB down to TB
        for (int i = 5 - unitsTracker; i > 0; i--) { size *= 1024; }
        units = "TB"; break;
      }

      stringstream output;
      output << std::fixed << std::setprecision(2) << size; // Format to 2 decimal places
      string numStr = output.str();
      // removes the decimal if it's a whole number
      if (numStr.find(".00") != string::npos) {
        numStr.erase(numStr.size() - 3);
      }

      return numStr + " " + units;
    }

    // surrounds the string with the corresponding color
    static string colorText(string message, std::string_view color) {
      std::stringstream formattedMessage;
      formattedMessage << color << message << RESET_TEXT;
      return formattedMessage.str();
    }
};

