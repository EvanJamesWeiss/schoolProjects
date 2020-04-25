Created By Evan Weiss

CSE 3150-SEC001

Professor Yufeng Wu

April 25th, 2020

# Project Milestone 1 : A User Guide

## What to Download:
Download the Makefile and ECProject folder. This folder contains all project files. Make sure both the Makefile and ECProject folder
are in the same directory.

## Compilation and Execution:
Using a linux terminal, _cd_ into the folder containing the Makefile and ECProject folder. Type "make" into the command line and
execute. This will compile the files in the ECProject folder, and output them to an executable file called "project.exe". The program __requires__ exactly one command line argument. This argument will be the name of text file.

To execute the program: "./project.exe <file_name>.txt"

If the text file does exist, its text will be displayed within the text editor.

If the text file does not exist, a new file with that same name will be created in the current directory. The editor will be blank upon loading.

## Key Features:
__The following is a table of the features that have been implemented in this version of the project.__

| Feature | Status | Design Pattern | User Guide | Function |
| ------- | ------ | -------------- | ---------- | -------- |
| Save & Quit** | Implemented | n/a | Press Ctrl+q | Saves text in editor to file specified on CLI, then terminates application |
| Cursor Movement** | Implemented | Command | All four arrow keys supported | Moves cursor within the editor. Bound by line length and window size. Pressing down/up on the bottom/top of the window, will skip to the next/previous page. | 
| Text Entry** | Implemented | Command | All aplha-numeric characters + special characters | Adds the typed character to the screen at the cursor. Now supports line wrapping. |
| Enter** | Implemented | Command | Press Enter | Adds newline / breaks line at the cursor location. Wrapped lines will be split into two separate lines. Entering at the bottom of the page will bring you to the next page. |
| Text Removal | Implemented | Command | Press Backspace | Deletes a character to the left of cursor. If pressed at the beginning of a row, it will append the current line to the end of the line above. Known issue: cursor doesn't move to correct location when deleting from wrapped lines (not 100% of the time). | 
| Undo Action | Implemented | Observer / Command | Press Ctrl+z | Undoes previous action (repeatable) |
| Redo Action | Implemented | Observer / Command | Press Ctrl+y | Redoes previously undone action (repeatable) |
| Page Up* | Implemented | Observer / Command | Press PgUp (Page Up) | Goes to the next page. |
| Page Down* | Implemented | Observer / Command | Press PgDn (Page Down) | Goes to the previous page. |

\* = New feature.

\*\* = New functionality added to existing feature.

## Summary:

The text editor now supports reading into and saving to a specified text file. This text file is specified on the command line upon execution. Pagination is also supported, so the user can load multipage documents without issue. Line wrapping is now supported, long lines of text will be wrapped within the window of the editor. Navigation has been updated to support these additions as well. 

I used various methods of object orientation, including encapsulation and various design patterns. The observer pattern was used to handle the undo and redo functionality. The command pattern was used to implement each of the actions invoked when a button on the keyboard is pressed. Lastly, the adapter pattern was used to translate actions and changes to the text from a usable format to the view. 
