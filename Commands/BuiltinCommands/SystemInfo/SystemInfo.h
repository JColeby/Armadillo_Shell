#pragma once
#include "../../Command.h"
#include "../Edit/Edit.h"
#include "Manual.h"
#include <iostream>
#include <string>
#include <vector>
#include <psapi.h>

// ===================={ System Info Command }====================
// TODO: add documentation to the manual file so James knows what flags and other info you implemented


class SystemInfo : public Command<SystemInfo> { // Command class needs to be inherited in order to work!!!
  vector<string> tokenizedCommand;
  bool getArchitecture;
  bool getCpu;
  bool getMemory;
  bool getPerformance;
  bool getDrive;

public:
  explicit SystemInfo(vector<string>& tokens) {
    tokenizedCommand = tokens; // should save arguments in the order they were passed in
    getArchitecture = false;
    getCpu = false;
    getMemory = false;
    getPerformance = false;
    getDrive = false;
  }

  static string returnManText() {
    return SystemInfoManual;
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
    // querying the computer architecture
    SYSTEM_INFO system_info;
    GetNativeSystemInfo(&system_info);
    std::stringstream outputBuffer;
    if (getArchitecture) {
      outputBuffer << WHITE << "\n====={ Architecture Information }=====\n" << RESET_TEXT;
      outputBuffer << getArchitectureInfo(system_info);
    }
    if (getCpu) {
      outputBuffer << getCpuInfo(system_info);
    }
    if (getMemory) {
      outputBuffer << getMemoryInfo();
    }
    if (getDrive) {
      outputBuffer << getDriveInfo();
    }
    if (getPerformance) {
      outputBuffer << getPerformanceInfo();
    }

    return {outputBuffer.str(), "200"};
  }

private:
  void setFlags() {
    if (tokenizedCommand.empty()) {
      getArchitecture = true;
      getCpu = true;
      getMemory = true;
      getPerformance = true;
      getDrive = true;
      return;
    }
    for (string param : tokenizedCommand) {
      for (int i = 1; i < param.size(); i++) {
        switch (param[i]) {
        case 'a': getArchitecture = true; break;
        case 'c': getCpu = true; break;
        case 'm': getMemory = true; break;
        case 'p': getPerformance = true;  break;
        case 'd': getDrive = true;  break;
        default: break;
        }
      }
    }
  }


  static string getArchitectureInfo(SYSTEM_INFO system_info) {
    // deciphering the architecture
    switch (system_info.wProcessorArchitecture) {
    case PROCESSOR_ARCHITECTURE_AMD64:
      return "  System Architecture: AMD x64 \n";
    case PROCESSOR_ARCHITECTURE_ARM:
      return "  System Architecture: ARM \n";
    case PROCESSOR_ARCHITECTURE_ARM64:
      return "  System Architecture: ARM x64 \n";
    case PROCESSOR_ARCHITECTURE_IA64:
      return "  System Architecture: IA-64 \n";
    case PROCESSOR_ARCHITECTURE_INTEL:
      return "  System Architecture: INTEL x86 \n";
    default:
      return "  System Architecture: UNKNOWN \n";
    }
  }


  static string getCpuInfo(SYSTEM_INFO system_info) {
    std::stringstream outputBuffer;

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
    if (!GetLogicalProcessorInformation(buffer.data(), &len)) {
      std::stringstream errorMsg;
      errorMsg << RED << "\nERROR: Failed to get core info \n" << RESET_TEXT;
      return errorMsg.str();
    }

    int physicalCores = 0;
    for (const auto& info : buffer) {
      // RelationProcessorCore represents info about a core.
      // Since we are only interested in the number of cores, we don't do anything else with it.
      if (info.Relationship == RelationProcessorCore) { physicalCores++; }
    }

    int logicalProcessors = (int)system_info.dwNumberOfProcessors;
    int activeProcessors = 0;

    // because dwActiveProcessorMask is a bitmask, with each bit representing a processor, we have to
    // loop through each bit. Which is why we have this weird setup
    DWORD_PTR activeProcessorsMask = system_info.dwActiveProcessorMask;
    for (DWORD i = 0; i < sizeof(DWORD_PTR) * 8; i++) {
      if (activeProcessorsMask & ((DWORD_PTR)1 << i)) { activeProcessors += 1; }
    }

    outputBuffer << WHITE << "\n====={ CPU Information }=====\n" << RESET_TEXT;
    outputBuffer << "  Number of Cores: " << std::to_string(physicalCores) << "\n";
    outputBuffer << "  Logical Processors: " << std::to_string(logicalProcessors) << "\n";
    outputBuffer << "  Active Processors: " << std::to_string(activeProcessors) << "\n";
    return outputBuffer.str();
  }


  static string getMemoryInfo() {
    std::stringstream outputBuffer;

    MEMORYSTATUSEX memoryState = {};
    memoryState.dwLength = sizeof(memoryState);
    if (!GlobalMemoryStatusEx(&memoryState)) {
      return outputBuffer.str() + RED + "\nERROR: Failed to get memory status \n" + RESET_TEXT;
    }
    outputBuffer << WHITE << "\n====={ Memory Information }=====\n" << RESET_TEXT;
    outputBuffer << "  Memory Usage: " << memoryState.dwMemoryLoad << "%\n";
    outputBuffer << "  Memory Installed: " << bytesToReadableString((double)memoryState.ullTotalPhys) << "\n";
    outputBuffer << "  Memory Available: " << bytesToReadableString((double)memoryState.ullAvailPhys) << "\n";
    return outputBuffer.str();
  }


  static string getPerformanceInfo() {
    PERFORMANCE_INFORMATION performanceInfo;
    performanceInfo.cb = sizeof(PERFORMANCE_INFORMATION);

    if (!GetPerformanceInfo(&performanceInfo, performanceInfo.cb)) {
      std::stringstream errorMsg;
      errorMsg << RED << "\nERROR: Failed to get performance info \n" << RESET_TEXT;
      return errorMsg.str();
    }

    auto pgSize = (double)performanceInfo.PageSize;

    std::stringstream outputBuffer;
    outputBuffer << WHITE << "\n====={ Performance Information }=====\n" << RESET_TEXT;
    outputBuffer << "  Page Size: " << bytesToReadableString((double)performanceInfo.PageSize) << "\n";
    outputBuffer << "  Committed Pages: " << performanceInfo.CommitTotal << " pages\n";
    outputBuffer << "  Current Page Limit: " << performanceInfo.CommitLimit << " pages\n";
    outputBuffer << "  Allocated Kernel Memory: " << bytesToReadableString((double)performanceInfo.KernelTotal * pgSize) << "\n";
    outputBuffer << "  Paged Kernel Memory: " << bytesToReadableString((double)performanceInfo.KernelPaged * pgSize) << "\n";
    outputBuffer << "  System Cache Memory: " << bytesToReadableString((double)performanceInfo.SystemCache * pgSize) << "\n";
    outputBuffer << "  Handle Count: " << performanceInfo.HandleCount << "\n";
    outputBuffer << "  Process Count: " << performanceInfo.ProcessCount << "\n";
    outputBuffer << "  Thread Count: " << performanceInfo.ThreadCount << "\n";

    return outputBuffer.str();
  }


  static string getDriveInfo() {
    // Querying the drives available.
    // GetLogicalDrives returns a bitmask that represents disk drives that are currently available
    // Bit position 0 (the least-significant bit) is drive A, bit position 1 is drive B, bit position 2 is drive C, and so on.
    DWORD driveBitmask = GetLogicalDrives();

    if (!driveBitmask) {
      std::stringstream errorMsg;
      errorMsg << RED << "\nERROR: Failed to get drive/disk info \n" << RESET_TEXT;
      return errorMsg.str();
    }

    std::stringstream outputBuffer;
    outputBuffer << WHITE << "\n====={ Drive Information }=====\n" << RESET_TEXT;


    // getting physical drive information
    for (int i = 0; i < 32; i++) {
      string physicalDriveOutput = getPhysicalDriveInfo(i);
      if (physicalDriveOutput == "") { break; }
      outputBuffer << physicalDriveOutput;
    }


    for (wchar_t i = 'A'; i <= 'Z'; i++) {
      if (driveBitmask & 1) {
        // setting up our buffers
        wchar_t drivePath[5] = { i, L':', L'\\', L'\\', L'\0' };
        ULARGE_INTEGER freeBytesAvailable;
        ULARGE_INTEGER totalNumberOfBytes;
        ULARGE_INTEGER totalNumberOfFreeBytes;

        if (!GetDiskFreeSpaceExW(drivePath, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
          outputBuffer << RED << "ERROR: Failed to fetch info for Drive " << i << "\n" << RESET_TEXT;
          continue;
        }
        outputBuffer << "  " << (char)i << "-Drive: " << "\n";
        outputBuffer << "     Drive size: " << bytesToReadableString((double)totalNumberOfBytes.QuadPart) << "\n";
        outputBuffer << "     Free space: " << bytesToReadableString((double)totalNumberOfFreeBytes.QuadPart) << "\n";
        outputBuffer << "     Space available: " << bytesToReadableString((double)freeBytesAvailable.QuadPart) << "\n";

      }
      driveBitmask = driveBitmask >> 1;
    }

    return outputBuffer.str();
  }




  // returns physical drive info. If the physical drive doesn't exist, the function returns an empty string
  // because there isn't a set function that will retrieve this info for us, we have use device/driver queries
  static string getPhysicalDriveInfo(int driveIndex) {
    std::stringstream outputBuffer;

    // Building the path to the physical drive
    std::wstring path = L"\\\\.\\PhysicalDrive" + std::to_wstring(driveIndex);

    // This function creates or opens a file or I/O device.
    // for more information about what this is doing, please visit https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
    HANDLE diskHandle = CreateFileW(path.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr, OPEN_EXISTING, 0, nullptr );

    // if the drive we are looking for does not exist
    if (diskHandle == INVALID_HANDLE_VALUE) { return ""; }

    // Because the physical drive is an I/O device, we have to call DeviceIoControl to get the information
    // Unfortunately this function is the biggest multi-tool of a function that I have ever seen, so we have to
    // specify what type of query we are making using this structure
    STORAGE_PROPERTY_QUERY driveQuery = {};
    driveQuery.PropertyId = StorageDeviceProperty;
    driveQuery.QueryType = PropertyStandardQuery;

    // Because of the nature of this function, what gets returned will vary depending on what query we make.
    // This means that we have to create a generic buffer to store the info from the query.
    // In order to get the correct size for our buffer, we first have to get the header information.
    // We do this by querying DeviceIoControl twice, where the first query will retrieve the response header, which we know the size of.
    STORAGE_DESCRIPTOR_HEADER driveQueryHeader = {};
    DWORD bytesReturned = 0;

    // DeviceIoControl is smart and will prevent itself from going beyond the buffer. Because of this, we can just
    // pass in the header structure, which will contains all the additional information that will be needed for the actual query
    if (!DeviceIoControl(
            diskHandle, // this is the handle to our drive that we got earlier
            IOCTL_STORAGE_QUERY_PROPERTY, // this tells the function what type of query we are performing (known as the control code)
            &driveQuery, // this is the input buffer, which we pass in our query. The query type is associated with the control code
            sizeof(driveQuery), // size of our query
            &driveQueryHeader, // this is the buffer where our output will be stored
            sizeof(driveQueryHeader), // size of the output buffer
            &bytesReturned, // the number of bytes that were filled in our output buffer
            nullptr)) // last parameter is for asynchronous calls. I didn't dive in too deep because it's not needed
    {
      std::stringstream errorMsg;
      errorMsg << RED << "\nERROR: Failed to get physical drive info for \\\\.\\PhysicalDrive" + std::to_string(driveIndex) << "\n" << RESET_TEXT;
      return errorMsg.str();
    }

    DWORD requiredSize = driveQueryHeader.Size; // now we have the size of the full query, which we can use to make a proper buffer
    vector<BYTE> buffer(requiredSize);

    // now we can call it with our proper buffer that will contain everything from the call.
    if (!DeviceIoControl(diskHandle, IOCTL_STORAGE_QUERY_PROPERTY, &driveQuery, sizeof(driveQuery),
      buffer.data(), requiredSize, &bytesReturned, nullptr))
    {
      std::stringstream errorMsg;
      errorMsg << RED << "\nERROR: Failed to get physical drive info for \\\\.\\PhysicalDrive" + std::to_string(driveIndex) << "\n" << RESET_TEXT;
      return errorMsg.str();
    }

    // now we just have to redefine the buffer so the compiler knows it contains a STORAGE_DEVICE_DESCRIPTOR
    STORAGE_DEVICE_DESCRIPTOR* driveDescriptor = reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR*>(buffer.data());



    outputBuffer << "  PhysicalDrive" << driveIndex << ": " << "\n";

    // now windows was silly, and decided that the function wouldn't actually include information like the
    // VendorId or ProductId in the structure. Instead, they included offsets to the actual information, which is stored
    // after the STORAGE_DEVICE_DESCRIPTOR object in the buffer (stupid, I know).
    string model;
    if (driveDescriptor->ProductIdOffset != 0) { // extracting the Product ID, which contains the model of our drive
      model = reinterpret_cast<const char*>(sizeof(buffer) + driveDescriptor->ProductIdOffset);
    } else { model = "UnknownModel"; }
    outputBuffer << "    Drive Model: " << model << "\n";

    string vendor;
    if (driveDescriptor->VendorIdOffset != 0) { // extracting the Product ID, which contains the model of our drive
      vendor = reinterpret_cast<const char*>(sizeof(buffer) + driveDescriptor->VendorIdOffset);
    } else { vendor = "UnknownVendor"; }
    outputBuffer << "    Drive Vendor: " << vendor << "\n";

    if (driveDescriptor->RemovableMedia) { outputBuffer << "    Removable Media\n"; }
    else { outputBuffer << "    Installed Media\n"; }

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
