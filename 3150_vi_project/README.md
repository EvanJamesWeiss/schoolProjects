Created By Evan Weiss

CSE 3150-SEC001

Professor Yufeng Wu

April 5th, 2020

# Project Milestone 1 : A User Guide

## What to Download:
Download the Makefile and ECProject folder. This folder contains all project files. Make sure both the Makefile and ECProject folder
are in the same directory.

## Compilation and Execution:
Using a linux terminal, _cd_ into the folder containing the Makefile and ECProject folder. Type "make" into the command line and
execute. This will compile the files in the ECProject folder, and output them to an executable file called "project.exe". 

To execute the program: "./project.exe"

## Key Features:
__The following is a list of the features that have been implemented in this version of the project. Included in this list are 
keystrokes associated with the features, and what each feature does.__
* Allowing you to quit the application
  * Keystroke: _CTRL + q_
  * Function: 
    * Terminates the application.
* Cursor Movement
  * Keystrokes: _ARROW KEYS_
    * Up, Down, Left, Right
  * Function: 
    * Cursor can move in the standard cardinal directions about the terminal.
    * Cursor movement limited by the number of rows and size of text on the row.
* Text Insertion
  * Keystrokes: _ENTER_ + _all alpha-numerical characters_ + _special characters_
  * Function:
    * _Alpha-numerical characters_ & _special character_ keys cause those characters to appear
      on screen at the cursor.
    * _ENTER_ key causes the current line to split at the cursor, putting the text to the right of the cursor on a new line below.
* Text Removal
  * Keystrokes: _BACKSPACE_
  * Function:
    * Deletes the character to the left of the cursor.
    * If the cursor is on the first space in the line, the row is removed, and text on the row is appended to the end of the line above.
* Undo / Redo
  * Keystrokes:
    * Undo: _CTRL + z_
    * Redo: _CTRL + y_
  * Function:
    * Undo: Undoes the previous action.
    * Redo: Redoes an action that was previously undone.
