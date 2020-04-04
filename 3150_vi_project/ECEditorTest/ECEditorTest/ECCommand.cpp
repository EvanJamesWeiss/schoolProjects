#include "ECCommand.h"

// ******************************************************
// Implement command history

ECCommandHistory :: ECCommandHistory() 
{
  // your code goes here
}

ECCommandHistory :: ~ECCommandHistory()
{
  // your code goes here
}

bool ECCommandHistory::Undo()
{
    if (undoStack.size() == 0) { return false; }
    ECCommand* cmd = undoStack.back();
    cmd->UnExecute();
    redoStack.push_back(cmd);
    undoStack.pop_back();
    return true;
}

bool ECCommandHistory::Redo()
{
    if (redoStack.size() == 0) { return false; }
    ECCommand* cmd = redoStack.back();
    cmd->Execute();
    undoStack.push_back(cmd);
    redoStack.pop_back();
    return true;
}

void ECCommandHistory::ExecuteCmd(ECCommand* pCmd)
{
    pCmd->Execute();
    undoStack.push_back(pCmd);
}