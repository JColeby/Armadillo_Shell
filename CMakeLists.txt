cmake_minimum_required(VERSION 3.10)  # 4.0 is invalid — latest stable minimum is 3.x
project(OperatingSystemsFinal)

set(CMAKE_CXX_STANDARD 14)
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
add_executable(CommandTestsExampleCommand_v2
        Tests/builtinCommandTests/testExampleCommand.cpp
        ${SHELL_SOURCES}
)

# -----------------------
# Enable testing via CTest (optional but recommended)
# -----------------------
enable_testing()

add_test(NAME RunCommandTests COMMAND CommandTests)
add_test(NAME RunExampleCommandTests COMMAND CommandTestsExampleCommand)

add_executable(CommandTestsExampleCommand
#        Armadillo_Shell/Tests/testMain.cpp
        Tests/builtinCommandTests/testExampleCommand.cpp
        Handlers/commandHandler.cpp
        Handlers/inputHandler.cpp
        Handlers/pipeHandler.cpp
        inputLoop.cpp
)

