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
#pragma comment(lib, "Psapi.lib")

using namespace VT;
using std::to_string;



class Process : public Command<Process> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  bool fullFilepath;
  bool hideDuplicates;
  bool showPerformanceInfo;



public:
  explicit Process(vector<string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
    fullFilepath = false;
    hideDuplicates = false;
    showPerformanceInfo = false;
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
    if (showPerformanceInfo) { return getPerformanceInfo(); }
    return readProcesses();
  }




private:

  void setFlags() {
    for (string param : tokenizedCommand) {
      for (int i = 1; i < param.size(); i++) {
        switch (param[i]) {
          case 'f': fullFilepath = true; break;
          case 'h': hideDuplicates = true; break;
          case 'p': showPerformanceInfo = true; break;
          default: break;
        }
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
    std::stringstream outputBuffer;
    outputBuffer << WHITE << "PID:            Executable:\n" << RESET_TEXT;

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
      string processName = utf8_encode(exeName); // encode it back to

      // flag adjustments
      if (!fullFilepath) { processName = std::filesystem::path(processName).filename().string(); } // gets only the filename
      if (hideDuplicates) {
        if (runningProcesses.find(processName) == runningProcesses.end()) { runningProcesses.insert(processName); }
        else { CloseHandle(process); continue; }
      }

      string itemBuffer = "  " + to_string(pid);
      while (itemBuffer.size() < 16) { itemBuffer += " "; }
      itemBuffer += "  " + processName + "\n";
      outputBuffer << itemBuffer;

      CloseHandle(process);
    }

    return {outputBuffer.str(), "200"};
  }


  vector<string> getPerformanceInfo() { // basically dumping the entire PERFORMANCE_INFORMATION structure
    PERFORMANCE_INFORMATION performanceInfo;
    performanceInfo.cb = sizeof(PERFORMANCE_INFORMATION);

    if (!GetPerformanceInfo(&performanceInfo, performanceInfo.cb)) {
      return {"Failed to get Performance Information. System error message: " + to_string(GetLastError()), "500"};
    }

    double pageMB = (double)performanceInfo.PageSize / (1024.0 * 1024.0);

    std::stringstream outputBuffer;
    outputBuffer << WHITE << "\n====={ Performance Information }=====\n" << RESET_TEXT;
    outputBuffer << "  CommitTotal:          " << (double)performanceInfo.CommitTotal * pageMB << " MB\n";
    outputBuffer << "  CommitLimit:          " << (double)performanceInfo.CommitLimit * pageMB << " MB\n";
    outputBuffer << "  CommitPeak:           " << (double)performanceInfo.CommitPeak * pageMB << " MB\n";
    outputBuffer << "  PhysicalTotal:        " << (double)performanceInfo.PhysicalTotal * pageMB << " MB\n";
    outputBuffer << "  PhysicalAvailable:    " << (double)performanceInfo.PhysicalAvailable * pageMB << " MB\n";
    outputBuffer << "  SystemCache:          " << (double)performanceInfo.SystemCache * pageMB << " MB\n";
    outputBuffer << "  KernelTotal:          " << (double)performanceInfo.KernelTotal * pageMB << " MB\n";
    outputBuffer << "  KernelPaged:          " << (double)performanceInfo.KernelPaged * pageMB << " MB\n";
    outputBuffer << "  KernelNonpaged:       " << (double)performanceInfo.KernelNonpaged * pageMB << " MB\n";
    outputBuffer << "  PageSize:             " << performanceInfo.PageSize << " bytes\n";
    outputBuffer << "  HandleCount:          " << performanceInfo.HandleCount << "\n";
    outputBuffer << "  ProcessCount:         " << performanceInfo.ProcessCount << "\n";
    outputBuffer << "  ThreadCount:          " << performanceInfo.ThreadCount << "\n";

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

