#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../../Command.h"
#include "Manual.h"



// ===================={ Print Working Directory Command }====================



class PrintWorkingDirectory : public Command<PrintWorkingDirectory> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;

public:
  explicit PrintWorkingDirectory(vector<string>& tokens) {
    tokenizedCommand = tokens;
  }

  static string returnManText() {
    return PrintWorkingDirectoryManual;
  }

  static bool validateSyntax(vector<string>& tokens) {
    if (tokens.empty()) return true;
    return false;
  }

  vector<string> executeCommand() override {
    string returnString;    // Did this so the background would always format correctly
    returnString += R"ASCII(                                                                                 )ASCII""\n";
    returnString += R"ASCII(       #######                                                                   )ASCII""\n";
    returnString += R"ASCII(       #######         ___ _         _   _                 ____  _ _ _           )ASCII""\n";
    returnString += R"ASCII(       #######        |_ _| |_ ___  | | | | ___ _ __ ___  / ___|(_) | |_   _     )ASCII""\n";
    returnString += R"ASCII(       #######         | || __/ __| | |_| |/ _ \ '__/ _ \ \___ \| | | | | | |    )ASCII""\n";
    returnString += R"ASCII(   "#############"     | || |_\__ \ |  _  |  __/ | |  __/  ___) | | | | |_| |    )ASCII""\n";
    returnString += R"ASCII(     "#########"      |___|\__|___/ |_| |_|\___|_|  \___| |____/|_|_|_|\__, |    )ASCII""\n";
    returnString += R"ASCII(       "#####"                                                         |___/     )ASCII""\n";
    returnString += R"ASCII(         "#"                                                                     )ASCII""\n";
    returnString += R"ASCII(                                                                                 )ASCII";
    return { setBgColor(31,30,51) + YELLOW + returnString + RESET_TEXT, "200" };


    // TCHAR pathBuffer[MAX_PATH];
    // DWORD length = GetCurrentDirectory(MAX_PATH, pathBuffer);
    // if (length == 0) {
    //   return { "failed to fetch current directory", "500" };
    // }
    // return {string(pathBuffer), "200"};
  }

};