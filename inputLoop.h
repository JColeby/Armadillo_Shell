#pragma once
#include <string>
#include <iostream>
#include "Handlers/inputHandler.cpp"
using namespace std;

int inputLoop() {

    std::cout << "Hello World!" << std::endl;
    inputHandler("userInput");
    return 0;
}