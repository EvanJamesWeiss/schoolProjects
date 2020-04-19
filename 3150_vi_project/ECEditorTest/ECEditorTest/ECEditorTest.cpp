// Test code for editor
#include "ECTextViewImp.h"
#include "ECTextEditor.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace  std;

void CreateFile(string filename)
{
    ofstream newFile;
    newFile.open(filename);
    if (!newFile)
    {
        cerr << "Unable to create " << filename;
        exit(1);
    }
    newFile.close();
}

void InitializeRows(ECTextEditor& wnd, string filename)
{
    // Attempt to open the file for reading in
    ifstream inFile;
    inFile.open(filename);

    // If the file doesn't exist...
    if (!inFile)
    {
        // Create the file and attempt to open that
        CreateFile(filename);
        inFile.open(filename);

        // If the created file couldn't be opened, then stop
        if (!inFile)
        {
            cerr << "Unable to open " << filename;
            exit(1);
        }
        
        // The file was opened successfully, add a status
        wnd.AddStatusRow("Writing to " + filename, "New File Created!", true);

        // This is added because otherwise the user has nowhere to write anything
        wnd.AddRow("");
    }
    else
    {
        // The existing file was opened successfully, add a status
        wnd.AddStatusRow("Writing to " + filename, "", true);

        // Read in all text from the file
        string x;
        while (inFile >> x)
        {
            wnd.AddRow(x);
        }

        // If there was no text in the file, give the user somewhere to start writing
        if (wnd.GetNumFilledRows() == 1)
        {
            wnd.AddRow("");
        }
    }

    // finally, close the file
    inFile.close();

}

int main(int argc, char *argv[])
{

    ECTextEditor wndTest;
    wndTest.AddRow("~"); // For formatting

    InitializeRows(wndTest, (string)argv[1]);

    ECViewHandler Handler(wndTest);
    wndTest.Attach(&Handler);

    wndTest.Show();
    
    return 0;
}
