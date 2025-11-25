#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <thread>
#include <iostream>
using std::string;
using std::vector;
using std::cout;
using std::endl;

int inputLoop();

int displayCurrentDirectory();

#include "Handlers/inputHandler.h"
#include "Globals/GlobalFlags.h"