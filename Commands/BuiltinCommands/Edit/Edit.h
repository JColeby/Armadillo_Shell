#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "../../Command.h"


class Edit : public Command<Edit> {
  vector<string> tokenizedCommand;
  vector<string> status;
  vector<string> fileContent;
  int fileX;
  int fileY;
  bool saved; // keeps track if there have been edits since the file was last saved
  bool editLoopActive; // flag that will update when we want to kill the program
  HANDLE inputConsoleHandle;
  HANDLE outputConsoleHandle;
  CONSOLE_SCREEN_BUFFER_INFO screenBuffer;
  SHORT screenWidth;
  int xOffset;
  int yOffset;
  SHORT screenHeight;

public:
  explicit Edit(vector<string>& tokens) {
    tokenizedCommand = tokens;
    status = {"Unknown Internal Error", "500"}; // default status
    fileContent = {};
    fileX = 0;
    fileY = 0;
    xOffset = 0;
    yOffset = 0;
    saved = true;
    editLoopActive = true;
    inputConsoleHandle = GetStdHandle(STD_INPUT_HANDLE);
    outputConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  }

  static string returnManFilePath() {
    return "BuiltinCommands/Edit/Manual.txt";
  }


  static bool validateSyntax(vector<string>& tokens) {
    if (tokens.size() == 1 and !tokens[0].empty()) { return true; }
    return false;
  }


  vector<string> executeCommand() override {
    // Saving the current console mode so we can revert back to it later
    DWORD startingMode;
    GetConsoleMode(inputConsoleHandle, &startingMode);

    //TODO: remove this after testing silly!
    tokenizedCommand = {"Commands/BuiltinCommands/Edit/Manual.txt"};

    try {
      if (std::filesystem::exists(tokenizedCommand[0])) { readFileToBuffer(); } // if file already exists
      else { saved = false; throw runtime_error("test File was not detected!");} // if there isn't an existing file
      if (fileContent.empty()) fileContent.push_back("");
      
      // setting up the terminal for editing
      // disabling ENABLE_LINE_INPUT prevents the editor from automatically processing newline characters.
      // disabling ENABLE_ECHO_INPUT means we have to control how input is displayed to the terminal, which gives us more control
      DWORD newMode = startingMode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
      SetConsoleMode(inputConsoleHandle, newMode);

      updateScreenBufferInfo();
      clearConsole();

      editFile();
      SetConsoleMode(inputConsoleHandle, startingMode); // reverting back to normal
      system("cls");
      return status;
    }
    catch (const exception& e) {
      SetConsoleMode(inputConsoleHandle, startingMode);
      return {e.what(), "500"};
    }
  }



private:

  void updateScreenBufferInfo() {
    GetConsoleScreenBufferInfo(outputConsoleHandle, &screenBuffer); // getting our screen buffer
    screenWidth  = screenBuffer.dwSize.X;
    screenHeight = screenBuffer.dwSize.Y;
  }

  // reads in the file and saves it to our buffer
  void readFileToBuffer() {
    ifstream file(tokenizedCommand[0]);
    if (file.is_open()) {
      string line;
      while (getline(file, line)) { fileContent.push_back(line); }
    }
    else { throw "File failed to open"; }
  }


  // saves the buffer to our file
  void saveBufferToFile() {
    ofstream file(tokenizedCommand[0]);
    if (file.is_open()) {
      for (string line : fileContent) { file << line << endl; }
    }
    else { throw "File failed to open"; }
  }


  // main loop. will continue until
  void editFile() {
    while (editLoopActive) {
      updateConsole();
      status = {"Unknown Internal Error", "500"}; // just in case something goes wrong and status was overwritten
      if (killSwitch) { killSwitchScreen(); }
      // Wait indefinitely for console input
      DWORD result = WaitForSingleObject(inputConsoleHandle, INFINITE);
      if (result == WAIT_OBJECT_0) {
        INPUT_RECORD record;
        DWORD eventsRead;

        // Read one input event
        ReadConsoleInput(inputConsoleHandle, &record, 1, &eventsRead);

        if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown) {
          KEY_EVENT_RECORD keyEvent = record.Event.KeyEvent;
          char ch = keyEvent.uChar.AsciiChar;

          // detecting if ctrl is pressed
          DWORD mods = keyEvent.dwControlKeyState;
          bool ctrlDown = (mods & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0;

          int newCursorX; // for when we have to update cursorY
          string supportLine; // for dealing with newline changes

          switch (keyEvent.wVirtualKeyCode) {
            case VK_LEFT:
              if (fileX > 0) { fileX--; }
              else if (fileY > 0) { fileY--; fileX = fileContent.size() - 1; }
              updateConsoleOffset();
              break;

            case VK_RIGHT:
              if (fileX < fileContent[fileY].size()) { fileX++; }
              updateConsoleOffset();
              break;

            case VK_UP:
              if (fileY > 0) { fileY--; }
              newCursorX = fileContent[fileY].size();
              if (fileX > newCursorX) { fileX = newCursorX; }
              updateConsoleOffset();
              break;

            case VK_DOWN:
              if (fileY < fileContent.size() - 1) { fileY++; }
              newCursorX = fileContent[fileY].size();
              if (fileX > newCursorX) { fileX = newCursorX; }
              updateConsoleOffset();
              break;

            case VK_RETURN:
              supportLine = fileContent[fileY].substr(fileX);
              fileContent[fileY] = fileContent[fileY].substr(0, fileX);
              fileContent.insert(fileContent.begin() + fileY + 1, supportLine);
              fileY++;
              fileX = 0;
              saved = false;
              updateConsoleOffset();
              break;

            case VK_BACK: // backspace
              deleteCharacter(fileX, fileY);
              updateConsoleOffset();
              break;

            case VK_DELETE:
              if (fileX < fileContent[fileY].size()) { deleteCharacter(fileX + 1, fileY, false); }
              else if (fileY < fileContent.size() - 1) { fileY++; deleteCharacter(0, fileY, false); }
              updateConsoleOffset();
              break;

            case VK_ESCAPE:
              exitFilePrompt();
              break;

            case 'S': // Ctrl+S
              if (ctrlDown and !saved) {
                try {
                  saveBufferToFile();
                  saved = true;
                }
                catch (string e) { editError(e); }
              }
              else { writeCharToBuffer(ch); }
              break;

            case 'X': // Ctrl+x
              if (ctrlDown) { exitFilePrompt(); }
              else { writeCharToBuffer(ch); }
              break;

            case 'Q': // Ctrl+q (killSwitch)
              if (ctrlDown) { exitFilePrompt(); }
              else { writeCharToBuffer(ch); }
              break;

            default:
              writeCharToBuffer(ch);
          }
        }
      }
    }
  }


  // updates the screen position relative to the current file
  void updateConsoleOffset() {
    updateScreenBufferInfo();
    if (fileX < xOffset) { xOffset = fileX; }
    else if (fileX >= xOffset + screenWidth) { xOffset = fileX - screenWidth + 1; }
    if (fileY < yOffset) { yOffset = fileY; }
    else if (fileY >= yOffset + screenHeight) { yOffset = fileY - screenHeight + 1; }
    if (xOffset < 0) xOffset = 0;
    if (yOffset < 0) yOffset = 0;
  }


  void writeCharToBuffer(char ch) {
    if (ch >= 32 and ch <= 126) {
      fileContent[fileY].insert(fileX, 1, ch);
      fileX++;
      saved = false;
    }
  }

  // updates the position of the cursor
  void updateCursor() {
    COORD position = { (SHORT)(fileX-xOffset), (SHORT)(fileY-yOffset) };
    SetConsoleCursorPosition(outputConsoleHandle, position);
  }


  void updateConsole() {
    updateScreenBufferInfo();

    SMALL_RECT writeRegion = { 0, 0, (SHORT)(screenWidth - 1), (SHORT)(screenHeight - 1) };
    COORD bufferSize = { screenWidth, screenHeight };
    COORD zero = { 0, 0 };

    // Create an offscreen buffer
    vector<CHAR_INFO> buffer(screenWidth * screenHeight);

    for (SHORT y = 0; y < screenHeight; y++) {
      for (SHORT x = 0; x < screenWidth; x++) {
        CHAR_INFO& cell = buffer[y * screenWidth + x]; // get buffer cell
        if (y < (SHORT)fileContent.size() && x < (SHORT)fileContent[y].size()) {
          cell.Char.AsciiChar = fileContent[y + yOffset][x + xOffset]; // set buffer cell to the correct ascii character
        } else { cell.Char.AsciiChar = ' '; }
        cell.Attributes = screenBuffer.wAttributes;
      }
    }

    // pushing changes to the screen (won't experience flickering
    WriteConsoleOutputA(outputConsoleHandle, buffer.data(), bufferSize, zero, &writeRegion);
    updateCursor();
  }


  // deletes character left of this position
  void deleteCharacter(int charX, int charY, bool updateCursor=true) {
    if (charX > 0) { // deleting normal character
      fileContent[charY].erase(charX - 1, 1);
      if (updateCursor) { fileX--; }
    }
    else if (charY > 0) { // deleting newline
      int previousLine = fileContent[charY - 1].size();
      fileContent[charY - 1] += fileContent[fileY];
      fileContent.erase(fileContent.begin() + fileY);
      fileY--;
      if (updateCursor) {
        fileX = previousLine;
      }
    }
    saved = false;
  }


  // clears the entire screen and resets cursor position
  void clearConsole() {
    system("cls");
    SetConsoleCursorPosition(outputConsoleHandle, { 0, 0 });
  }


  // prompt that will be shown if killSwitch is true
  void killSwitchScreen() {
    exitFilePrompt();
    status = {"Editor Killed Successfully", "600"};
  }



  // ===================={ Code for user prompts }====================


  // prompt that will be shown if a user tries to exit
  void exitFilePrompt() { // TODO: fix
    if (!saved) {
      clearConsole();
      cout << "You have unsaved changes. Do you want to save before exiting?" << "\n";
      cout << "Yes[y], No[n], Cancel[c]" << "\n";
      cout << flush;
      char keyPressed = tolower(returnKeyPress({'y', 'n', 'c'}));
      if (keyPressed == 'c') { return; }
      if (keyPressed == 'y') {
        try { saveBufferToFile(); saved = true; }
        catch (string e) {
          editError(e);
          exitFilePrompt(); // calls itself again so the user can retry if needed
          return;
        }
      }
    }
    status = {"Editor Exited Successfully", "201"};
    editLoopActive = false;
  }


  // will return one of the keys in the provided vector when it is pressed. Will wait until one of the keys is pressed
  int returnKeyPress(vector<int> keys, bool anyKey=false) {
    FlushConsoleInputBuffer(inputConsoleHandle); // flushes the buffer so we don't pick up keys that were pressed before calling
    while (true) {
      DWORD result = WaitForSingleObject(inputConsoleHandle, INFINITE);
      if (result == WAIT_OBJECT_0) {
        INPUT_RECORD record;
        DWORD eventsRead;
        ReadConsoleInput(inputConsoleHandle, &record, 1, &eventsRead); // read in input

        if (record.EventType == KEY_EVENT and record.Event.KeyEvent.bKeyDown) {
          if (anyKey) { return record.Event.KeyEvent.wVirtualKeyCode; } // returns the key that was pressed if anyKey is True

          KEY_EVENT_RECORD keyEvent = record.Event.KeyEvent;
          char ch = keyEvent.uChar.AsciiChar;
          for (int i = 0; i < keys.size(); i++) {
            if (tolower(ch) == tolower(keys[i])) return ch; // use tolower so the keys are still detected if uppercase
          }

        }
      }
    }
  }

  // used for prompts. will freeze the screen until a key is pressed
  void hitAnyKeyToContinue() {
    cout << "hit any key to continue" << "\n";
    cout << flush;
    returnKeyPress({}, true);
  }


  // used when handling expected errors
  void editError(string e) {
    clearConsole();
    cout << "EDIT ERROR:" << e << "\n";
    hitAnyKeyToContinue();
  }



};

