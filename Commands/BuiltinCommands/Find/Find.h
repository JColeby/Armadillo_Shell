#pragma once
#include <string>
#include <vector>
#include "../../Command.h"
#include "Manual.h"
#include <filesystem>

using namespace std::filesystem;


class Find : public Command<Find> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  string startingDirectory;
  string desiredFile;
  vector<string> matches;
  bool matchDirectories;
  bool matchFiles;
  bool getAllMatches;
  bool displayFullPath;
  bool killed;
  // add more class variables as needed.

public:
  explicit Find(vector<string>& tokens) {
    matchDirectories = false;
    matchFiles = false;
    getAllMatches = false;
    displayFullPath = true;
    killed = false;

    desiredFile = "";
    startingDirectory = "";
    tokenizedCommand = tokens;
  }


  static string returnManText() {
    return FindManual;
  }


  static bool validateSyntax(vector<string>& tokens) {
    if (tokens.size() < 2) { return false; }
    if (tokens.size() > 2) {
      bool isDPresent = false;
      bool isFPresent = false;
      bool nextIsName = false;
      int numberOfNonFlags = 0;

      for (auto & token : tokens) { // looping through all the tokens
        if (token.front() == '-') {
          bool isFPresentLocally = false;
          bool isDPresentLocally = false;
          if (nextIsName) { return false; } // if the previous flag was F or D, we need a non-flag to be right after
          if (token.size() == 1) { return false; } // no flag was actually specified

          for (int i = 1; i < token.size(); i++) { // checking what is in the flag
            char t = token.at(i);
            if (t == 'D') {
              if (isDPresent) { return false; }
              isDPresent = true; isDPresentLocally = true; nextIsName = true;
            }
            else if (t == 'F') {
              if (isFPresent) { return false; }
              isFPresent = true; isFPresentLocally = true; nextIsName = true;
            }
            else if (t != 'f' and t != 'd' and t != 'a' and t != 's') { return false; }
          }

          if (isDPresentLocally and isFPresentLocally) { return false; } // these need to be separate
        }

        else { nextIsName = false; numberOfNonFlags += 1; }
      }

      if (nextIsName) { return false; } // supposed to be another string after the last command
      if (numberOfNonFlags != 2) { return false; }
    }

    else { // if it's just the destination and filename
      if (tokens[0].front() == '-') { return false; }
      if (tokens[1].front() == '-') { return false; }
    }

    return true;
  }


  vector<string> executeCommand() override {
    try {
      validateFlags();
      recursiveSearch();
    }
    catch (const std::invalid_argument& e) { return {e.what(), "400"}; }
    catch (const std::runtime_error& e) { return {e.what(), "500"}; }
    if (killed) { return { formatFoundFiles(), "600"}; }
    return { formatFoundFiles(), "200"};
  }


private:
  void validateFlags() {
    vector<string> nonFlags;

    // mapping flags to class variables
    // NOTICE: errors will only be thrown if you didn't call validate syntax and checked that it returned true.
    bool skipNext = false;
    for (int i = 0; i < tokenizedCommand.size(); i++) {
      if (skipNext) { skipNext = false; continue; }
      if (tokenizedCommand[i].front() == '-') {
        for (int j = 1; j < tokenizedCommand[i].size(); j++) { // checking what is in the flag
          if (tokenizedCommand[i].at(j) == 'D') {
            if (i + 1 >= tokenizedCommand.size()) { throw std::invalid_argument("Missing argument after -D"); }
            startingDirectory = tokenizedCommand[i+1]; skipNext = true;
          }
          else if (tokenizedCommand[i].at(j) == 'F') {
            if (i + 1 >= tokenizedCommand.size()) { throw std::invalid_argument("Missing argument after -F"); }
            desiredFile = tokenizedCommand[i+1]; skipNext = true;
          }
          else if (tokenizedCommand[i].at(j) == 'd') { matchDirectories = true; }
          else if (tokenizedCommand[i].at(j) == 'f') { matchFiles = true; }
          else if (tokenizedCommand[i].at(j) == 'a') { getAllMatches = true; }
          else if (tokenizedCommand[i].at(j) == 's') { displayFullPath = false; }
        }
      }
      else { nonFlags.push_back(tokenizedCommand[i]); }
    }

    // setting the starting directory and desired file
    try {
      if (startingDirectory.empty() and !desiredFile.empty()) { startingDirectory = nonFlags[0]; } // If only -F was specified, assume remaining non-flag is the starting directory
      else if (!startingDirectory.empty() and desiredFile.empty()) { desiredFile = nonFlags[0]; }  // If only -D was specified, assume remaining non-flag is the desired file
      else if (startingDirectory.empty() and desiredFile.empty()) {
        startingDirectory = nonFlags[1];
        desiredFile = nonFlags[0];
      }
    }
    catch (const std::out_of_range& e){
      throw std::invalid_argument("Invalid arguments. Didn't specify the starting directory or desired file");
    }

    if (!matchDirectories and !matchFiles) { matchFiles = true; }
  }


  // recursively searches through files to find the desired file
  void recursiveSearch() {
    path startPath = startingDirectory;

    // recursively searching through folders
    for (auto it = recursive_directory_iterator(startPath); it != recursive_directory_iterator(); ++it) {

      if (killSwitch) { killed = true; return; }

      path entryPath = it->path();
      string entryName = entryPath.filename().string();

      // Skip special folders so we don't get errors
      if (entryPath.has_parent_path() && entryPath.string().find(".git") != string::npos) {
        if (it->is_directory()) { it.disable_recursion_pending(); }
        continue;
      }

      string currentDirectory;
      if (displayFullPath) { currentDirectory = absolute(entryPath.parent_path()).string(); }
      else { currentDirectory = entryPath.string(); }

      if (it->is_directory() && matchDirectories) {
          if (search(entryName, currentDirectory, true)) { return; }
      }

      if (it->is_regular_file() && matchFiles) {
          if (search(entryName, currentDirectory, false)) { return; }
      }
    }
  }


  // will return true if we found a match and the getAllMatches flag is false
  bool search(string file, string currentDirectory, bool isDirectory) {
      if (desiredFile.front() == '*' and desiredFile.back() == '*') {
        string check = desiredFile;
        if (check.size() > 1) {
          check.erase(0, 1);
          check.erase(check.size() - 1);
        } else { check.clear(); }
        if (file.find(check) != string::npos) {
          if (addFoundItem(file, currentDirectory, isDirectory)) { return true; }
        }
      }

      else if (desiredFile.front() == '*') {
        string check = desiredFile;
        check.erase(0, 1);
        if (endsWith(file, check)) {
          if (addFoundItem(file, currentDirectory, isDirectory)) { return true; }
        }
      }

      else if (desiredFile.back() == '*') {
        string check = desiredFile;
        check.erase(check.size() - 1);
        if (startsWith(file, check)) {
          if (addFoundItem(file, currentDirectory, isDirectory)) { return true; }
        }
      }

      else {
        if (desiredFile == file) {
          if (addFoundItem(file, currentDirectory, isDirectory)) { return true; }
        }
      }
      return false;
  }


  // adds the found filepath to our matches list and returns true if getAllMatches is set to false
  bool addFoundItem(string& file, string& currentDirectory, bool isDirectory) {
    string newMatch = currentDirectory;
    if (displayFullPath) { newMatch += "\\" + file; }
    if (isDirectory) { newMatch += "\\"; }
    matches.push_back(newMatch);
    if (!getAllMatches) { return true; }
    return false;
  }


  static bool endsWith(const string& str, const string& suffix) {
    if (suffix.size() > str.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
  }


  static bool startsWith(const string& str, const string& prefix) {
    if (prefix.size() > str.size()) return false;
    return std::equal(prefix.begin(), prefix.end(), str.begin());
  }


  // loops through each item in our matches list and formats it into a single string
  string formatFoundFiles() {
    std::stringstream finalString;
    for (int i = 0; i < matches.size(); i++) {
      finalString << matches[i];
      if (i < matches.size() - 1) { finalString << "\n";}
    }
    return finalString.str();
  }

};


