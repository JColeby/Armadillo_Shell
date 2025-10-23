cmake_minimum_required(VERSION 3.10)
project(OperatingSystemsFinal)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------
# Shared source files
# -----------------------
set(SHELL_SOURCES
        Handlers/commandHandler.cpp
        Handlers/inputHandler.cpp
        Handlers/pipeHandler.cpp
        inputLoop.cpp
)

# -----------------------
# Main executable
# -----------------------
add_executable(OperatingSystemsFinal
        armadillo.cpp
        ${SHELL_SOURCES}
)

# -----------------------
# Test: General Test Runner
# -----------------------
add_executable(CommandTests
        Tests/testMain.cpp
        ${SHELL_SOURCES}
)

# -----------------------
# Test: Example Command
# -----------------------
add_executable(CommandTestsExampleCommand
        Tests/builtinCommandTests/testExampleCommand.cpp
        ${SHELL_SOURCES}
)



