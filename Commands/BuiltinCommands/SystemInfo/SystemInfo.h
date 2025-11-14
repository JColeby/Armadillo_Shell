#pragma once
#include "../../Command.h"
#include "../Edit/Edit.h"
#include "Manual.h"
#include <iostream>
#include <string>
#include <vector>

// ===================={ System Info Command }====================
// TODO: add documentation to the manual file so James knows what flags and other info you implemented


class SystemInfo : public Command<SystemInfo> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  // add more class variables as needed.

public:
  explicit SystemInfo(vector<string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
  }

  static string returnManText() {
    return SystemInfoManual;
  }

  static bool validateSyntax(vector<string>& tokens) {
    // TODO: implement
    // this should be a simple validation so it can be used when validating
    // commands that are getting piped. More thorough validations can be done
    // in the execute command itself.
    // tokens should contain all of the command inputs the user provided
    // in order. However, It will not contain the command at the start.
    return true;
  }

  vector<string> executeCommand() override {
    // querying the computer architecture
    SYSTEM_INFO system_info;
    GetNativeSystemInfo(&system_info);

    return {"Not Implemented", "500"};
  }

private:
  string getSystemArchitecture(SYSTEM_INFO system_info) {
    // deciphering the architecture
    switch (system_info.wProcessorArchitecture) {
    case PROCESSOR_ARCHITECTURE_AMD64:
      return "System Architecture: AMD x64 \n";
    case PROCESSOR_ARCHITECTURE_ARM:
      return "System Architecture: ARM \n";
    case PROCESSOR_ARCHITECTURE_ARM64:
      return "System Architecture: ARM x64 \n";
    case PROCESSOR_ARCHITECTURE_IA64:
      return "System Architecture: IA-64 \n";
    case PROCESSOR_ARCHITECTURE_INTEL:
      return "System Architecture: INTEL x86 \n";
    default:
      return "System Architecture: UNKNOWN \n";
    }
  }


  string getProcessorInfo(SYSTEM_INFO system_info) {
    int logicalProcessors = (int)system_info.dwNumberOfProcessors;

    int activeProcessors = 0;

    // because dwActiveProcessorMask is a bitmask, with each bit representing a processor, we have to
    // loop through each bit. Which is why we have this weird setup
    DWORD_PTR activeProcessorsMask = system_info.dwActiveProcessorMask;
    for (DWORD i = 0; i < sizeof(DWORD_PTR) * 8; i++) {
      if (activeProcessorsMask & ((DWORD_PTR)1 << i)) { activeProcessors += 1; }
    }

    return "Logical Processors: " + std::to_string(logicalProcessors) + "\n"
           + "Active Processors: " + std::to_string(activeProcessors) + "\n";
  }


  string getCoreInfo() {
    // because GetLogicalProcessorInformation returns an array of structures, we'll need to get the length of the buffer to
    // pass in. Fortunately GetLogicalProcessorInformation will do this for us if we pass in a nullptr.
    // It saves the required array size in bytes to DWORD len.
    DWORD len = 0;
    GetLogicalProcessorInformation(nullptr, &len);

    // We can then calculate the number of SYSTEM_LOGICAL_PROCESSOR_INFORMATION structs that will be returned,
    // which we use to allocate an array of the same size
    DWORD count = len / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
    vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buffer(count);

    // now we can pass in our buffer and get the info we need!
    if (!GetLogicalProcessorInformation(buffer.data(), &len)) { return 0; }

    int physicalCores = 0;

    for (const auto& info : buffer) {
      // RelationProcessorCore represents info about a core.
      // Since we are only interested in the number of cores, we don't do anything else with it.
      if (info.Relationship == RelationProcessorCore) { physicalCores++; }
    }

    return "Number of Cores: " + std::to_string(physicalCores) + "\n";
  }

  string getSystemMemory() {
    std::stringstream memoryInfo;

    MEMORYSTATUSEX memoryState = {};
    memoryState.dwLength = sizeof(memoryState);
    if (!GlobalMemoryStatusEx(&memoryState)) {
      return memoryInfo.str() + RED + "ERROR: Failed to get memory status \n" + RESET_TEXT;
    }
    memoryInfo << "Memory Usage: " << memoryState.dwMemoryLoad << "%\n";
    memoryInfo << "Memory Installed: " << memoryState.ullTotalPhys / (1024*1024) << " MB\n";
    memoryInfo << "Memory Available: " << memoryState.ullAvailPhys / (1024*1024) << " MB\n";
    return memoryInfo.str();
  }




};


// // This was the initial way chat recommended I get the core count, as the way above won't work if the machine has more than 64 logical cores
// // However, your computer will only have that many if it's a server or just over the top. Left this in here for educational reasons
  // int GetPhysicalCoreCount() {
  //   // in order to get the physical core count, we need query GetLogicalProcessorInformationEx, which is kinda complicated
  //   // this windows API function returns a structure called SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX, which varies in size
  //   // the reason why it varies in size is because the structure contains a union that contains a GROUP_RELATIONSHIP struct
  //   // the GROUP_RELATIONSHIP struct kinda makes everything complicated because it's an array, making
  //   // SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX vary in size. Now we could have used the simpler SYSTEM_LOGICAL_PROCESSOR_INFORMATION
  //   // structure, but that can only count
  //
  //   // to get around this, we have to call GetLogicalProcessorInformationEx with a null pointer, which turns it into a query
  //   // for the total size we will need in bytes, and saves the result in length
  //   DWORD length = 0;
  //   GetLogicalProcessorInformationEx(RelationProcessorCore, nullptr, &length);
  //   // We are querying RelationProcessorCore, which will give us the core count
  //
  //   // because SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX will vary in size, we can't just divide length by its size to get
  //   // a proper buffer. To get around this, we end up creating a buffer of characters that is the correct length in bytes.
  //   // We then cast our char buffer into a SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX pointer that is the correct size.
  //   std::vector<char> buffer(length);
  //   PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX processorInfoBuffer = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.data());
  //
  //   // We can now properly call GetLogicalProcessorInformationEx by passing in our buffer pointer
  //   if (!GetLogicalProcessorInformationEx(RelationProcessorCore, processorInfoBuffer, &length)) {
  //     return 0;
  //   }
  //
  //   DWORD physicalCoreCount = 0;
  //
  //   // Because the size of each struct may vary, we have to iterate through it using pointers
  //   char* ptr = buffer.data();
  //   char* end = buffer.data() + length;
  //
  //   while (ptr < end) {
  //     PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX entry =
  //         reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(ptr);
  //
  //     if (entry->Relationship == RelationProcessorCore)
  //       physicalCoreCount++;
  //
  //     ptr += entry->Size; // moves the pointer to the next struct
  //   }
  //
  //   return (int)physicalCoreCount;
  // }
