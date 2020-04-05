// Test code for editor
#include "ECTextViewImp.h"
#include "ECTextEditor.h"
#include <iostream>
#include <cstdlib>

using namespace  std;

int main(int argc, char *argv[])
{
    ECTextEditor wndTest;
    wndTest.AddRow("~");
    wndTest.AddRow("CSE 3150");
    wndTest.AddRow("This is a very simple demo of the ECTextViewImp functionalities.");
    wndTest.AddRow("Press ctrl-q to quit");

    ECViewHandler Handler(wndTest);
    wndTest.Attach(&Handler);

    wndTest.Show();
    
    return 0;
}
