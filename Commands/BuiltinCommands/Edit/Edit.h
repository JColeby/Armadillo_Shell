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
  vector<string> status;  // returned when exiting the editor
  vector<string> fileContent; // stores the current file state
  int fileX; // x position in the file
  int fileY; // y position in the file
  bool saved; // keeps track if there have been edits since the file was last saved
  bool editLoopActive; // flag that will update when we want to exit the editor
  HANDLE inputConsoleHandle; // used for changing console mode and detecting key events
  HANDLE outputConsoleHandle; // used for displaying output
  CONSOLE_SCREEN_BUFFER_INFO screenBuffer; // essentially stores the console screen
  SHORT consoleWidth;  // read the name again silly
  SHORT consoleHeight;
  int xOffset; // used to get the cursor and character positions inside screenBuffer
  int yOffset;

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
    updateScreenBufferInfo();
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
    consoleWidth  = screenBuffer.dwSize.X;
    consoleHeight = screenBuffer.dwSize.Y;
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
      DWORD result = WaitForSingleObject(inputConsoleHandle, INFINITE); // will wait indefinitely until it detects user input
      if (result == WAIT_OBJECT_0) { // if user input was detected

        INPUT_RECORD record; // stores info about the input event
        DWORD eventsRead; // stores the number of events actually read. Only needed for ReadConsoleInput
        ReadConsoleInput(inputConsoleHandle, &record, 1, &eventsRead); // Reads in the first input event

        if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown) { // if the event was a key down action
          KEY_EVENT_RECORD keyEvent = record.Event.KeyEvent;
          char ch = keyEvent.uChar.AsciiChar; // get the character of the key that was pressed
          bool ctrlDown = keyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED); // detecting if ctrl was also pressed

          int newCursorX; // for when we have to update cursorY
          string supportLine; // for dealing with newline changes

          switch (keyEvent.wVirtualKeyCode) {
            case VK_LEFT:
              if (fileX > 0) { fileX--; }
              else if (fileY > 0) { fileY--; fileX = fileContent[fileY].size() - 1; }
              updateConsoleOffset();
              break;

            case VK_RIGHT:
              if (fileX < fileContent[fileY].size()) { fileX++; }
              else if (fileY < fileContent.size() - 1) { fileY++; fileX = 0; }
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
                try { saveBufferToFile(); saved = true; }
                catch (string e) { editError(e); }
              }
              else { writeCharToFileInfo(ch); }
              break;

            case 'X': // Ctrl+x
              if (ctrlDown) { exitFilePrompt(); }
              else { writeCharToFileInfo(ch); }
              break;

            case 'Q': // Ctrl+q (detecting killSwitch because GetAsyncKeyState doesn't work with the current console mode)
              if (ctrlDown) { exitFilePrompt(); }
              else { writeCharToFileInfo(ch); }
              break;

            default:
              writeCharToFileInfo(ch);
          }
        }
      }
    }
  }


  // updates the screen position relative to the current file
  void updateConsoleOffset() {
    updateScreenBufferInfo();
    while (fileX < xOffset) { xOffset = fileX; } // using while loops for edge cases where it needs to be done multiple times
    while (fileX >= xOffset + consoleWidth) { xOffset = fileX - consoleWidth + 1; }
    while (fileY < yOffset) { yOffset = fileY; }
    while (fileY >= yOffset + consoleHeight) { yOffset = fileY - consoleHeight + 1; }
    if (xOffset < 0) xOffset = 0;
    if (yOffset < 0) yOffset = 0;
  }


  void writeCharToFileInfo(char ch) {
    if (ch >= 32 and ch <= 126) {
      fileContent[fileY].insert(fileX, 1, ch);
      fileX++;
      saved = false;
    }
  }


  // makes sure the cursor is visible and updates its position
  void updateCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(outputConsoleHandle, &cursorInfo);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(outputConsoleHandle, &cursorInfo);
    COORD position = { (SHORT)(fileX-xOffset), (SHORT)(fileY-yOffset) };
    SetConsoleCursorPosition(outputConsoleHandle, position);
  }


  // updates file changes to the screen. This code solved the flickering issues with the old updateConsole
  void updateConsole() {
    updateScreenBufferInfo();

    SMALL_RECT writeRegion = { 0, 0, (SHORT)(consoleWidth - 1), (SHORT)(consoleHeight - 1) };
    COORD bufferSize = { consoleWidth, consoleHeight };
    COORD zero = { 0, 0 };
    vector<CHAR_INFO> buffer(consoleWidth * consoleHeight); // Offscreen buffer that will be pushed to the screen

    for (SHORT y = 0; y < consoleHeight; y++) {
      for (SHORT x = 0; x < consoleWidth; x++) {
        CHAR_INFO& cell = buffer[y * consoleWidth + x]; // get buffer cell
        if (y < (SHORT)(fileContent.size() - yOffset) && x < (SHORT)(fileContent[y].size() - xOffset)) { // verifying there is a corresponding char for the cell position
          cell.Char.AsciiChar = fileContent[y + yOffset][x + xOffset]; // set buffer cell to the correct ascii character
        } else { cell.Char.AsciiChar = ' '; }
        cell.Attributes = screenBuffer.wAttributes;
      }
    }
    WriteConsoleOutputA(outputConsoleHandle, buffer.data(), bufferSize, zero, &writeRegion); // pushing changes to the screen
    updateCursor();
  }


  // deletes character left of this position. used for both backspace and delete button events.
  void deleteCharacter(int charX, int charY, bool updatePosition=true) {
    if (charX > 0) { // deleting normal character
      fileContent[charY].erase(charX - 1, 1);
      if (updatePosition) { fileX--; } // position should only update when backspace is pressed
    }
    else if (charY > 0) { // deleting newline
      int previousLine = fileContent[charY - 1].size();
      fileContent[charY - 1] += fileContent[fileY];
      fileContent.erase(fileContent.begin() + fileY);
      fileY--;
      if (updatePosition) {
        fileX = previousLine;
      }
    }
    saved = false;
  }



  // ===================={ Code for user prompts }====================


  // clears the entire screen and hides the cursor.
  void clearConsole() {
    system("cls");
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(outputConsoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE; // making the cursor invisible
    SetConsoleCursorInfo(outputConsoleHandle, &cursorInfo);
  }



  // prompt that will be shown if a user tries to exit
  void exitFilePrompt() {
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


  // prompt that will be shown if killSwitch is true
  void killSwitchScreen() {
    exitFilePrompt();
    status = {"Editor Killed Successfully", "600"};
  }


  // will return one of the keys in the provided vector when it is pressed. Will wait until one of the keys is pressed
  int returnKeyPress(vector<int> keys, bool anyKey=false) {
    FlushConsoleInputBuffer(inputConsoleHandle); // flushes the buffer so we don't pick up keys that were pressed before calling

    while (true) { // code is the same as what's in editFile. Look there for more information
      DWORD result = WaitForSingleObject(inputConsoleHandle, INFINITE);
      if (result == WAIT_OBJECT_0) {
        INPUT_RECORD record;
        DWORD eventsRead;
        ReadConsoleInput(inputConsoleHandle, &record, 1, &eventsRead);
        if (record.EventType == KEY_EVENT and record.Event.KeyEvent.bKeyDown) {
          KEY_EVENT_RECORD keyEvent = record.Event.KeyEvent;
          char ch = keyEvent.uChar.AsciiChar;

          if (anyKey) { return record.Event.KeyEvent.wVirtualKeyCode; } // returns the key that was pressed if anyKey is True
          for (int i = 0; i < keys.size(); i++) {
            if (tolower(ch) == tolower((char)keys[i])) return ch; // use tolower so the keys are still detected if uppercase
          }
        }
      }
    }
  }



};

