#pragma once
#include <string>

const std::string GrepManual = R"(
    Grep Command:
        stands for Global Regular Expression Print. Uses regular expressions to filter text

    Syntax:
        grep <flags> <regex> <stringOfText>

    Flags:
        -v     inverts matches
        -i     ignores casing

    works best if you pipe the output of other commands into this one

)";

