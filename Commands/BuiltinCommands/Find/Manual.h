#pragma once
#include <string>

const std::string FindManual = R"(
    Find Command:
        recursively finds files or directories within a provided folder .

    Syntax:
        find  <desired file> <starting directory> <flags>

    Flags:
        -d      will only match directory names
        -f      will only match file names
        -a      will recursively search the entire folder and return all files that match
        -s      will display the path that you provided instead of the full path

        -D      name of starting directory is located right after this flag (useful for piping)
        -F      name of desired file is located right after this flag (useful for piping)

    Capital letter flags can't be combined in the same parameter (i.e -DF is not valid syntax)

    note that using -D or -F just changes where the file/directory name is located. if you do
    something like "find <directory> <file> -D <directory>", it will fail the syntax validation
    as the directory is in there twice.

)";
