#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <filesystem>
#include "../../Command.h"
#include "../../../TerminalFormatting.h"
#include <unordered_set>
#pragma comment(lib, "Psapi.lib")

using namespace VT;
using std::to_string;


// ===================={ Command Template }====================
// TODO: DO NOT MODIFY THIS FILE DIRECTLY!!! Copy the contents of this file into a new header file
// TODO: Please add test code! This will make it easier to debug the shell when making changes that could impact other files
// I spent way too long trying to figure out how to get the test code to work for c++, so message me and I'll create a new file for tests
// might be a good idea to create the manual first so you know how to implement the class

class Process : public Command<Process> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  bool fullFilepath;
  bool showAll;

public:
  explicit Process(vector<string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
    fullFilepath = false;
  }

  static string returnManFilePath() {
    // TODO: update the folder name
    return "BuiltinCommands/Process/Manual.txt";
  }

  static bool validateSyntax(vector<string>& tokens) {
    if (tokens.empty()) { return true; }
    for (string token : tokens) {
      if (token.front() != '-') { return false; }
    }
    return true;
  }

  vector<string> executeCommand() override {
    // TODO: implement
    // Will assume validateSyntax was already called, but add error handling just in case
    setFlags();
    return readProcesses();
  }

private:

  void setFlags() {
    for (string param : tokenizedCommand) {
      for (int i = 1; i < param.size(); i++) {
        if (param[i] == 'f') { fullFilepath = true; }
        if (param[i] == 'a') { showAll = true; }
      }
    }
  }

  vector<string> readProcesses() {
    std::unordered_set<string> runningProcesses;

    DWORD processes[1024], bytesReturned;

    if (!EnumProcesses(processes, sizeof(processes), &bytesReturned)) {
      return {"Failed to get processes. System error message: " + to_string(GetLastError()), "500"};
    }

    unsigned int count = bytesReturned / sizeof(DWORD);
    string outputBuffer = "PID:            Executable:\n";

    for (unsigned int i = 0; i < count; ++i) {

      // getting the current process
      DWORD pid = processes[i];
      if (pid == 0) { continue; }
      HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
      if (!process) { continue; }

      // getting the filepath
      wchar_t exeName[MAX_PATH] = L"[Unknown]";
      if (!GetModuleFileNameExW(process, nullptr, exeName, MAX_PATH)) {
        wcscpy_s(exeName, L"[Access Denied]");
      }
      string processName = utf8_encode(exeName);

      // flag adjustments
      if (!fullFilepath) { processName = std::filesystem::path(processName).filename().string(); } // gets only the filename
      if (!showAll) {
        if (runningProcesses.find(processName) == runningProcesses.end()) { runningProcesses.insert(processName); }
        else { CloseHandle(process); continue; }
      }

      string itemBuffer = "  " + to_string(pid);
      while (itemBuffer.size() < 16) { itemBuffer += " "; }
      itemBuffer += "  " + processName + "\n";
      outputBuffer += itemBuffer;

      CloseHandle(process);
    }

    return {outputBuffer, "200"};
  }



  // chat gave me this which should encode everything correctly
  static string utf8_encode(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(),
                                          (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(),
                        &str[0], size_needed, nullptr, nullptr);
    return str;
  }

};

