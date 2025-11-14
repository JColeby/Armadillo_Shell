#pragma once
#include <string>

const std::string SystemInfoManual = R"(SystemInfo Command:
    displays information about your computer

    Syntax:
        sys <flags>

    Flags:
        -a      displays system architecture
        -c      displays cpu information
        -m      displays memory information
        -p      displays performance information

    Will display all options if no flags are provided
)";