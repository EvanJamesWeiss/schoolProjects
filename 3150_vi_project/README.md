Created By Evan Weiss

CSE 3150-SEC001

Professor Yufeng Wu

May 5th, 2020

# Project Milestone 3 : Final Submission

## Section 1 & 2: Overview and User guide

### What to Download:
Download the Makefile and ECProject folder. This folder contains all project files. Make sure both the Makefile and ECProject folder
are in the same directory.

### Compilation and Execution:
Using a linux terminal, _cd_ into the folder containing the Makefile and ECProject folder. Type "make" into the command line and
execute. This will compile the files in the ECProject folder, and output them to an executable file called "project.exe". The program __requires__ exactly one command line argument. This argument will be the name of text file.

To execute the program: "./project.exe <file_name>.txt"

 * If the text file does exist, its text will be displayed within the text editor.

 * If the text file does not exist, a new file with that same name will be created in the current directory. The editor will be blank upon loading.

__Originally Tested Using:__ Cygwin64 (GNU version 8.26) terminal running on Windows 10.

### Key Features:
__The following is a table of the features that have been implemented in this version of the project.__

| Feature | Status | Design Pattern | User Guide | Function |
| ------- | ------ | -------------- | ---------- | -------- |
| Save & Quit | Implemented | n/a | Press Ctrl+q | Saves text in editor to file specified on CLI, then terminates application |
| Cursor Movement** | Implemented | Command | All four arrow keys supported | Moves cursor within the editor. Bound by line length and window size. Pressing down/up on the bottom/top of the window will skip to the next/previous page. | 
| Text Entry | Implemented | Command | All aplha-numeric characters + special characters | Adds the typed character to the screen at the cursor. Now supports line wrapping. |
| Enter | Implemented | Command | Press Enter | Adds newline / breaks line at the cursor location. Wrapped lines will be split into two separate lines. Entering at the bottom of the page will bring you to the next page. |
| Text Removal | Implemented | Command | Press Backspace | Deletes a character to the left of cursor. If pressed at the beginning of a row, it will append the current line to the end of the line above. | 
| Undo Action | Implemented | Observer / Command | Press Ctrl+z | Undoes previous action (repeatable) |
| Redo Action | Implemented | Observer / Command | Press Ctrl+y | Redoes previously undone action (repeatable) |
| Page Up | Implemented | Observer / Command | Press PgUp (Page Up) | Goes to the next page. |
| Page Down | Implemented | Observer / Command | Press PgDn (Page Down) | Goes to the previous page. |

\* = New feature.

\*\* = New functionality added to existing feature.

### Known Issues:
__The following is a list of known bugs and issues that have not been resolved as of the latest version of the project.__

_The exterminator was called and there are no known bugs_


## Section 3: Design Spec

### UML Class Diagram with Relationships:

![UML Diagram](images/uml.png)

#### Explanation of the UML Diagram:
  The ECTextEditor is at the heart of this project. It inherits from the provided ECTextViewImp class (which inherited from the ECObserver class) and acts as a wrapper on the ECTextViewImp to provide additional functionality. In order to access individual rows, ECTextEditor contains it's own copy of listRows that is updated at the same time as it's parent class's listRows. At initialization, a handler (ECViewHandler) is attached. This handler provides handling for all user commands and implements the __observer design pattern__. If the ECTextEditor detects a button has been pressed, based on the button, the handler will instantiate a command and ask the ECTextEditor to execute it. These user commands are specified by the multiple classes icon titled "Commands". The note to the right lists all classes that inherit from the ECCommand interface. They all implement the __command design pattern__ and one or more can be added to the ECCommandHistory object within the ECTextEditor object. Once in the history, the ECTextEditor provides the ability to Undo or Redo any action. 
  
## Section 4: Implementation Spec

### Technical Issues During Development:
   By far the largest hurdle to overcome was line wrapping and pagination while maintaining existing functionality. Displaying long lines as wrapped lines was trivial, but I experienced several errors. Initially, the cursor's location wasn't accurately determining where changes were made, as they would often be offset by one or several lines. After tweaking, I got something that appeared to work on the first page within single lines, but was similarlly offset on every other page and still wouldn't work with wrapped lines. My first solution fixed the issue of the offset on other pages when only considering single (non-wrapped) lines. Within the ECTextEditor class, I implemented a helper function that obtains an offset which I use in various places to counteract these offsets. The second solution made everything work together as it fixed the offsets and other unexpected behaviour within wrapped lines (on any page). Due to the way I mapped listRows strings to the currentDoc (long strings to wrapped strings), it created a consistent and predictable offset whenever a line would be wrapped. To solve this, I added the modifiers variable which is a vector of integers. this modifiers vector contains the same number of elements as the currentDoc, but at each position represents the offset required to map the index (equal to the cursor's Y position) in currentDoc to the correct index in listRows. By acting as a bridge between the cursor's position and the listRows string, I was able to more accurately and correctly update the strings. I was also able to use this to determine the correct X position within any wrapped string.
 
### Key Algorithms:
__The following lists, in no specific order, the algorithms of various methods and processes performed within the application.__
 * When first initialized and a valid file name is specified, a file stream is opened. Read from this stream are all lines from within the file. These files are added into the ECTextEditor object using AddRow. Doing this puts the strings into the listRows vector where their full, un-wrapped versions are stored. 
 * The listRows vector contains all unbroken strings. Any changes made to the document from within the application directly alter the string within listRows.
 * The view needs to be updated everytime there is a change made to a string within listRows, as changing listRows does not mean the view changes. Once a change is made, the method UpdateView is called. Within this method, the listRows within ECTextViewImp are cleared, the strings within listRows are mapped to currentDoc, and the view is updated to contain only the strings necessary to fill the page (and no more).
 * The mapping between listRows and currentDoc is the way I solved line wrapping. As perviously mentioned, listRows contains unbroken strings that can be longer than the window is wide. After running the MapRowsToPage method, currentDoc contains strings that are at most as long as the window will allow. These currentDoc strings are then used to be passed to the ECTextViewImp via it's implementation of the AddRows method. 
 * The MapRowsToPage method also generates the modifiers vector which is the link between the cursor's Y position and the string within listRows and the cursor's X position to the correct location in the string (even when wrapped).
    * For example: Supposed there is a single line wraps from lines 0 to 2 (covering 3 total rows for one continuous string). This string represents one string in listRows (index 0), but three in currentDoc (indeces 0-2) when mapped. Now suppose the cursor is at (0, 1). The modifier list allows for helper methods to determine the correct X and Y position so the correct string within listRows can be correctly updated. The modifier vector's values in this example would be \[0,1,2\]. The Y position of 1 is mapped to 0, and the X position is mapped to modifiers\[1\]\*(window width). 
 * The handler class, ECViewHandler, contains the required Update method as per the ECObserver interface. The ECViewHandler is initialized with a reference to the ECTextEditor it's attached to. This allows the Update method to query for information regarding the cursor position and last pressed key. Using the last pressed key, the Update method uses a switch-case statement to select which command to execute depending on the key pressed. Once chosen, the handler asks the ECTextEditor to execute the command. 
 * There are commands for each type of user input. These individual command classes implement the command design pattern and have unique implementations depending on the task. A commonality between them is they all have Execute and UnExecute methods. Execute is invoked when the command is first executed or redone. UnExecute is invoke when the command is undone.
 
### Refactored Changes:
  Due to my coding experience and knowledge that this project required object-orientaion, there was little refactoring that I felt was necessary. As I programmed the commands, I would first do them directly within the switch-case statement. Once their functionality was verified, I would create it's corresponding command class. This cleaned up my handler's Update method while isolating any potential issues to their respective classes. My wrapper class, ECTextEditor, contains several helper private and public that allow for safe access to important variables or information. Again, these were created as needed, and provide improved data encapsulation for my objects. Extraneous variables, numbers, and repeated statements were taken out as I went, with one exception. I left in several redundant calls to query for the cursor's X and Y location, which made the code feel cluttered. I have since factored this out where possible and acheived more readable code. 
  
## Section 5: Conclusion and Discussions

### What I learned:
  To say I learned something new (outside of solutions to this specific problem) might be disingenuous. After previously taking CSE 2102, a course dedicated to object orientation and design patterns, the lectures on different design patterns and how to implement them were review. Programming this assignment with object oriented design patterns in mind was not new either, as previous projects have also stressed object oriented design. 
  
  This is not to say I didn't take anything away from this project. This practical problem showcased a practical application for these design patterns. It provided me with additional experience using the various design patterns I've learned by posing a challenging, yet doable task. The biggest takeaway was how design patterns and object orientation can save you time and headaches. The bugs and other issues I was having were easily identifiable and remedied because I had been programming with these in mind. It highly increased readablity and refactorablity, two things that object orientation does in theory, but to see it in practice was quite satisfying. All in all, this project was a superb, practical application of the topics we've learned in class, and I enjoyed working on it.
  
### Possible Improvements:
  The UpdateView method within ECTextEditor runs relatively slowly. The present logic requires all lines to be cleared from the view, currentDoc, and modifiers, all to be repopulated and reconstructed again. The reasoning behind this originally was if a line in listRows suddenly spans 2 lines instead of just 1, I will need to shift all lines back in currentDoc and modifiers. Now this would still be faster, but would require several lines of logic to determine where and when to make these shifts. The improvement to my current code would be to implement this conditional shifting within ECTextEditor instead of clearing and repopulating them every time the view is updated.
  
  ECTextViewImp also does not provide an set or update method for it's rows, which necessitated me building my own. Another improvement would be to alter ECTextViewImp directly so it provides a set method for it's rows. This would eliminate the need for clearing the view's rows and re-adding them during every update.
  
## Section 6: Extra functionality
  The only extra functionality is the ability to use the page up and page down keys for navigating the pages of the text. This is not exactly non-trivial. Unfortunately, no additional non-trivial is available at this time.
