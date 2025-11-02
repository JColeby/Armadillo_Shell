#pragma once
#include <string>
#include <windows.h>
#include <thread>
#include <iostream>
#include "Handlers/inputHandler.h"
#include "Globals/GlobalFlags.h"
using std::string;
using std::vector;
using std::cout;
using std::endl;

int inputLoop();

int displayCurrentDirectory();