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
#include "Manual.h"

using namespace VT;
using std::to_string;



class Process : public Command<Process> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  bool fullFilepath;
  bool hideDuplicates;

public:
  explicit Process(vector<string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
    fullFilepath = false;
    hideDuplicates = false;
  }


  static string returnManText() {
    return ProcessManual;
  }


  static bool validateSyntax(vector<string>& tokens) {
    if (tokens.empty()) { return true; }
    for (string token : tokens) {
      if (token.front() != '-') { return false; }
    }
    return true;
  }


  vector<string> executeCommand() override {
    setFlags();
    return readProcesses();
  }



private:
  void setFlags() {
    for (string param : tokenizedCommand) {
      for (int i = 1; i < param.size(); i++) {
        switch (param[i]) {
          case 'f': fullFilepath = true; break;
          case 'h': hideDuplicates = true; break;
          default: break;
        }
      }
    }
  }


  vector<string> readProcesses() {
    std::unordered_set<string> runningProcesses;

    // getting the process ids for all active processes
    DWORD processes[1024], bytesReturned;
    if (!EnumProcesses(processes, sizeof(processes), &bytesReturned)) {
      return {"Failed to get processes. System error message: " + to_string(GetLastError()), "500"};
    }

    std::stringstream outputBuffer;
    outputBuffer << WHITE << "PID:            Executable:\n" << RESET_TEXT;

    // looping through each process id that was returned
    unsigned int count = bytesReturned / sizeof(DWORD);
    for (unsigned int i = 0; i < count; ++i) {

      // getting the current process
      DWORD pid = processes[i];
      if (pid == 0) { continue; } // skip system idle process
      HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
      if (!process) { continue; } // skip if we can't get the process

      // getting the filepath
      wchar_t exeName[MAX_PATH] = L"[Unknown]";     // wchar is used so we can handle unicode characters correctly
      if (!GetModuleFileNameExW(process, nullptr, exeName, MAX_PATH)) {
        wcscpy_s(exeName, L"[Access Denied]");
      }
      string processName = utf8_encode(exeName); // encode it back to normal char characters

      // flag adjustments
      if (!fullFilepath) { processName = std::filesystem::path(processName).filename().string(); } // gets only the filename
      if (hideDuplicates) {
        if (runningProcesses.find(processName) == runningProcesses.end()) { runningProcesses.insert(processName); }
        else { CloseHandle(process); continue; }
      }

      // save the process to the buffer
      string itemBuffer = "  " + to_string(pid);
      while (itemBuffer.size() < 16) { itemBuffer += " "; }
      outputBuffer << itemBuffer << "  " << processName << "\n";

      CloseHandle(process);
    }

    return {outputBuffer.str(), "200"};
  }


  // chat gave me this which should encode everything correctly
  static string utf8_encode(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &str[0], size_needed, nullptr, nullptr);
    return str;
  }

};

