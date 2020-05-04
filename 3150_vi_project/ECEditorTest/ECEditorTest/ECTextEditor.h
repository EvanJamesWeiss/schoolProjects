#ifndef ECTEXTEDITOR_H
#define	ECTEXTEDITOR_H
#include "ECCommand.h"
#include "ECTextViewImp.h"

#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace  std;

// Wrapper on the TextView that allows functionality outside of the predefined view class
// independent of the TextView implementation
class ECTextEditor : public ECTextViewImp
{
public:

    ECTextEditor() {};
    void AddRow(const string& str)
    {
        ECTextViewImp::AddRow(str);
        listRows.push_back(str);
        this->UpdateView();
    }

    void InitRows()
    {
        ECTextViewImp::InitRows();
        listRows.clear();
    }

    string GetRowString(int row)
    {
        row = GetActualRow(row);
        return listRows[row];
    }

    void InsertRow(int row, string str = "", int mod = 0)
    {
        int Arow = GetActualRow(row);
        // Just in case the user tries to add a row at the end of the file
        if ((listRows.begin() + Arow + mod) > listRows.end())
        {
            listRows.push_back(str);
        }
        else
        {
            listRows.insert(listRows.begin() + Arow + mod, str);
        }
        this->UpdateView();
    }

    void UpdateRow(int row, string str, int mod = 0)
    {
        row = GetActualRow(row);
        listRows[row + mod] = str;
        this->UpdateView();
    }

    void DeleteRow(int row)
    {
        int Arow = GetActualRow(row);
        listRows.erase(listRows.begin() + Arow + (GetModifier(row + 1) - GetModifier(row)));
        this->UpdateView();
    }

    bool Undo()
    {
        return histCmds.Undo();
    }

    bool Redo()
    {
        return histCmds.Redo();
    }

    void Execute(ECCommand* cmd)
    {
        histCmds.ExecuteCmd(cmd);
    }

    int GetNumFilledRows()
    {
        int startpos = pageNum * this->GetRowNumInView();
        startpos = (pageNum == 0) ? startpos : startpos - GetOffset();
        int endpos = startpos + this->GetRowNumInView();
        endpos = ((int)currentDoc.size() < endpos) ? currentDoc.size() : endpos;
        
        return endpos - startpos;
    }

    int GetTotalRows()
    {
        return listRows.size();
    }

    string GetOutputString(int i)
    {
        return listRows[i];
    }

    void SetPageNum(int num)
    {
        pageNum = num;
        this->SetCursorX(0);
        this->SetCursorY(0);
        this->UpdateView();
    }

    int GetPageNum()
    {
        return pageNum;
    }

    int GetActualRow(int row) // Get row number within listRows
    {
        row += GetRowAdder();
        row -= modifiers[row];
        return row;
    }

    int GetActualCol(int CX) // Get column number within a string of listRows
    {
        int mod = 0; 
        int n = this->GetCursorY() + 1;
        while (GetModifier(n) != GetModifier(n - 1))
        {
            mod += GetCurrentDocString(n - 1).size() + 1;
            n -= 1;
        }
        return CX + mod;
    }

    int GetModifier(int row)
    {
        row += GetRowAdder();
        return modifiers[row];
    }

    string GetCurrentDocString(int row)
    {
        row += GetRowAdder();
        return currentDoc[row];
    }

    int GetRowAdder()
    {
        return (pageNum * (this->GetRowNumInView() - 1));
    }

private:

    vector<string> listRows;
    int pageNum = 0;
    vector<int> modifiers;
    vector<string> currentDoc;

    ECCommandHistory histCmds;
       
    // If a row was updated in the text editor, it also needs to be updated in the view
    void UpdateView()
    {
        ECTextViewImp::InitRows();
        MapRowsToPage();

        int startpos = pageNum * this->GetRowNumInView();
        startpos = (pageNum == 0) ? startpos : startpos - GetOffset();
        int endpos = startpos + this->GetRowNumInView();
        endpos = ((int)currentDoc.size() < endpos) ? currentDoc.size() : endpos;

        for (int i = startpos; i < endpos; ++i)
        {
            ECTextViewImp::AddRow(currentDoc[i]);
        }
    }

    // Maps listRows so a new list that can be viewed on screen
    void MapRowsToPage()
    {
        currentDoc.clear();
        modifiers.clear();
        string row;
        int mod = 0;

        for (int i = 0; i < (int)listRows.size(); ++i)
        {
            row = listRows[i];
            
            while ((int)row.size() > this->GetColNumInView() - 1)
            {
                currentDoc.push_back(SplitRow(row));
                modifiers.push_back(mod); 
                mod += 1;
            }
            currentDoc.push_back(row);
            modifiers.push_back(mod);
            //cerr << "***" + row << endl << "***" << (int)row.size() << endl << "***" << this->GetRowNumInView();
        }
        modifiers.push_back(mod);
    }

    // Helper to MapRowsToPage. Used to cut off the first part of the string and return it.
    // The first part will not be longer than the 
    string SplitRow(string& row)
    {
        int pos = this->GetColNumInView() - 1;
        while (row[pos] != ' ')
        {
            pos -= 1;
            if (pos == 0) { return row; }
        }
        string output = row.substr(0, pos);
        row = row.substr(pos + 1, row.size());
        return output;
    }

    // used in a few places, just to make things consistent
    int GetOffset()
    {
        //return 0;
        return pageNum;
    }

};

//---------------------------------------------------
// Beginning of command definitions
class MoveCursorRight : public ECCommand
{
public:
    MoveCursorRight(ECTextEditor& doc) : subject(doc) {}
    void Execute()
    {
        if (subject.GetCursorX() + 1 <= (int)subject.GetCurrentDocString(subject.GetCursorY() + 1).length() + 1)
        {
            subject.SetCursorX(subject.GetCursorX() + 1);
        }
    }
    void UnExecute()
    {
        if (subject.GetCursorX() - 1 >= 0)
        {
            subject.SetCursorX(subject.GetCursorX() - 1);
        }
    }
private:
    ECTextEditor& subject;
};

class MoveCursorLeft : public ECCommand
{
public:
    MoveCursorLeft(ECTextEditor& doc) : subject(doc) {}
    void Execute()
    {
        if (subject.GetCursorX() - 1 >= 0)
        {
            subject.SetCursorX(subject.GetCursorX() - 1);
        }
    }
    void UnExecute()
    {
        if (subject.GetCursorX() + 1 <= (int)subject.GetRowString(subject.GetCursorY() + 1).length() + 1)
        {
            subject.SetCursorX(subject.GetCursorX() + 1);
        }
    }
private:
    ECTextEditor& subject;
};

class MoveCursorUp : public ECCommand
{
public:
    MoveCursorUp(ECTextEditor& doc) : subject(doc), CX(0) {}
    void Execute()
    {
        CX = subject.GetCursorX();
        if (subject.GetCursorY() != 0)
        {
            if (CX > (int)subject.GetCurrentDocString(subject.GetCursorY()).length())
            {
                subject.SetCursorX(subject.GetCurrentDocString(subject.GetCursorY()).length());
            }
            subject.SetCursorY(subject.GetCursorY() - 1);
        }
        else
        {
            subject.SetCursorX(0);
        }
    }
    void UnExecute()
    {
        subject.SetCursorX(CX);
        if (subject.GetCursorY() + 2 < subject.GetNumFilledRows())
        {
            subject.SetCursorY(subject.GetCursorY() + 1);
        }
    }

private:
    ECTextEditor& subject;
    int CX;
};

class MoveCursorDown : public ECCommand
{
public:
    MoveCursorDown(ECTextEditor& doc, int cx) : subject(doc), CX(cx) {}
    void UnExecute()
    {
        subject.SetCursorX(CX);
        if (subject.GetCursorY() != 0)
        {
            subject.SetCursorY(subject.GetCursorY() - 1);
        }
    }
    void Execute()
    {
        if (subject.GetCursorY() + 2 < subject.GetNumFilledRows())
        {
            if (CX > (int)subject.GetCurrentDocString(subject.GetCursorY() + 2).length())
            {
                subject.SetCursorX(subject.GetCurrentDocString(subject.GetCursorY() + 2).length());
            }
            subject.SetCursorY(subject.GetCursorY() + 1);
        }
    }

private:
    ECTextEditor& subject;
    int CX;
};

class InputCharacter : public ECCommand
{
public:
    InputCharacter(ECTextEditor& doc, char pk) : subject(doc), PK(pk) {}
    void Execute()
    {
        CX = subject.GetCursorX();
        CY = subject.GetCursorY();
        string mystr = subject.GetRowString(CY + 1);

        // In the event that the cursor is beyond the length of the string... just so it won't crash
        if (mystr.begin() + subject.GetActualCol(CX) > mystr.end())
        {
            mystr.push_back(' ');
            mystr.push_back(PK);
            subject.SetCursorX(CX + 1);
        }
        else
        {
            mystr.insert(mystr.begin() + subject.GetActualCol(CX), PK);
        }

        string lineStr = subject.GetCurrentDocString(CY + 1);
        int eol = subject.GetColNumInView() - 1;

        // Find the first last char in the line,
        while (lineStr[eol] == ' ')
        {
            eol -= 1;
        }

        while (lineStr[eol] != ' ')
        {
            eol -= 1;
        }

        // Handle cursor movement if the line is as long as the window
        if (CX >= eol && ((int)subject.GetCurrentDocString(CY + 1).size() >= subject.GetColNumInView() - 1)) 
        {
            subject.SetCursorX(CX - eol);
            subject.SetCursorY(CY + 1); // next row down
        }
        else if (CX + 1 <= (int)subject.GetCurrentDocString(CY + 1).length() + 1)
        {
            // normal functionality if there aren't multiple lines
            subject.SetCursorX(CX + 1);
        }
        // finally, update the row
        subject.UpdateRow(CY + 1, mystr);
    }
    void UnExecute()
    {
        int iCX = subject.GetCursorX();
        string mystr = subject.GetRowString(subject.GetCursorY() + 1);
        mystr.erase(mystr.begin() + subject.GetActualCol(iCX) - 1);
        subject.UpdateRow(subject.GetCursorY() + 1, mystr);
        subject.SetCursorX(CX);
        subject.SetCursorY(CY);
    }

private:
    ECTextEditor& subject;
    char PK;
    int CX = 0;
    int CY = 0;
};

class DeleteCharacter : public ECCommand
{
public:
    DeleteCharacter(ECTextEditor& doc) : subject(doc), PK(char(0)), Movedup(false) {}
    void UnExecute()
    {
        string mystr = subject.GetRowString(subject.GetCursorY() + 1);
        mystr.insert(mystr.begin() + subject.GetActualCol(subject.GetCursorX()), PK);
        subject.UpdateRow(subject.GetCursorY() + 1, mystr);
        if (Movedup)
        {
            subject.SetCursorY(subject.GetCursorY() + 1);
        }
        subject.SetCursorX(CX);
    }
    void Execute()
    {
        CX = subject.GetCursorX();

        // Get the distance to the nearest space character (to the right)
        int initdist = DistToSpace(CX);
        int tospace = DistToSpace(0);

        string mystr = subject.GetRowString(subject.GetCursorY() + 1);
        PK = mystr.at(subject.GetActualCol(CX) - 1);
        mystr.erase(mystr.begin() + subject.GetActualCol(CX) - 1);

        bool Up = ShouldMoveUp(CX);

        subject.UpdateRow(subject.GetCursorY() + 1, mystr);

        if (subject.GetCursorX() - 1 >= 0)
        {
            subject.SetCursorX(subject.GetCursorX() - 1);
        }
        else
        {
            subject.SetCursorX(DistToSpace(CX) - initdist);
        }

        if (Up)
        {
            Movedup = true;
            subject.SetCursorX(subject.GetColNumInView() - 1 - tospace + CX);
            subject.SetCursorY(subject.GetCursorY() - 1);
        }
    }

private:
    ECTextEditor& subject;
    char PK;
    int CX = 0;
    bool Movedup;

    int DistToSpace(int CX)
    {
        if (CX != 0) { return -1; }
        string currentrow = subject.GetCurrentDocString(subject.GetCursorY() + 1);
        while (currentrow[CX] != ' ')
        {
            CX += 1;
        }
        return CX;
    }

    bool ShouldMoveUp(int CX)
    {
        bool output = true;
        if (subject.GetActualRow(subject.GetCursorY()) == 0)
        {
            return false;
        }
        output &= (CX <= DistToSpace(0)); // if the cursor is in the first word of a line
        
        // If the sum of length of the line above and length of the first word in the row (with a char deleted)
        // can fit in the row above
        output &= ((int)subject.GetCurrentDocString(subject.GetCursorY()).length() + DistToSpace(0) - 1 <= subject.GetColNumInView() - 2);

        // If it's the first line in the paragraph, don't worry about it
        output &= (subject.GetModifier(subject.GetCursorY()) != subject.GetModifier(subject.GetCursorY() + 1));

        return output;
    }

    void msg(string first, string second = "")
    {
        subject.ClearStatusRows();
        subject.AddStatusRow(first, second, true);
    }


};

class PressEnter : public ECCommand
{
public:
    PressEnter(ECTextEditor& doc, int cx) : subject(doc), CX(cx) {}
    void Execute()
    {
        //subject.InsertRow(subject.GetCursorY() + 2);
        string lineStr = subject.GetRowString(subject.GetCursorY() + 1);
        string frontStr = lineStr.substr(0, subject.GetActualCol(CX));
        string backStr = "";
        
        if (subject.GetActualCol(CX) != (int)lineStr.length() + 1)
        {
            backStr = lineStr.substr(subject.GetActualCol(CX), lineStr.length());
        }

        subject.InsertRow(subject.GetCursorY() + 1, "", 1);

        subject.UpdateRow(subject.GetCursorY() + 1, frontStr);

        subject.UpdateRow(subject.GetCursorY() + 1, backStr, 1);

        subject.SetCursorX(0);
        subject.SetCursorY(subject.GetCursorY() + 1);
    }
    void UnExecute()
    {
        string lineStr = subject.GetRowString(subject.GetCursorY() + 1);
        string aboveStr = subject.GetRowString(subject.GetCursorY());
        string comboStr = aboveStr + lineStr;

        int n = subject.GetCursorY();
        while (subject.GetModifier(n) != subject.GetModifier(n - 1))
        {
            n -= 1;
        }

        subject.DeleteRow(subject.GetCursorY());
        subject.UpdateRow(n, comboStr);
        subject.SetCursorX(CX);
        if (subject.GetCursorY() != 0)
        {
            subject.SetCursorY(subject.GetCursorY() - 1);
        }
    }

private:
    ECTextEditor& subject;
    int CX;
};

class BackspaceMerge : public ECCommand
{
public:
    BackspaceMerge(ECTextEditor& doc) : subject(doc) {}
    void UnExecute()
    {
        string lineStr = subject.GetRowString(subject.GetCursorY() + 1);
        string frontStr = lineStr.substr(0, subject.GetActualCol(subject.GetCursorX()));
        string backStr = lineStr.substr(subject.GetActualCol(subject.GetCursorX()), lineStr.length());
        subject.InsertRow(subject.GetCursorY() + 1, "", 1);
        subject.UpdateRow(subject.GetCursorY() + 1, frontStr);
        subject.UpdateRow(subject.GetCursorY() + 1, backStr, 1);
        subject.SetCursorX(0);
        subject.SetCursorY(subject.GetCursorY() + 1);
    }
    void Execute()
    {
        string lineStr = subject.GetRowString(subject.GetCursorY() + 1);
        string aboveStr = subject.GetRowString(subject.GetCursorY());
        string comboStr = aboveStr + lineStr;
        int currlen = subject.GetCurrentDocString(subject.GetCursorY()).length();
        subject.UpdateRow(subject.GetCursorY(), comboStr);
        subject.DeleteRow(subject.GetCursorY() + 1);
        if (subject.GetCursorY() != 0)
        {
            subject.SetCursorY(subject.GetCursorY() - 1);
        }
        subject.SetCursorX(((int)aboveStr.length() > subject.GetColNumInView()) ? currlen : aboveStr.length());
    }

private:
    ECTextEditor& subject;
};

class PageUp : public ECCommand
{
public:
    PageUp(ECTextEditor& doc) : subject(doc), CX(0), CY(0) {}
    void Execute()
    {
        CX = subject.GetCursorX();
        CY = subject.GetCursorY();
        subject.SetPageNum(subject.GetPageNum() - 1);
    }
    void UnExecute()
    {
        subject.SetPageNum(subject.GetPageNum() + 1);
        subject.SetCursorX(CX);
        subject.SetCursorY(CY);
    }
private:
    ECTextEditor& subject;
    int CX;
    int CY;
};

class PageDown : public ECCommand
{
public:
    PageDown(ECTextEditor& doc) : subject(doc), CX(0), CY(0) {}
    void Execute()
    {
        CX = subject.GetCursorX();
        CY = subject.GetCursorY();
        subject.SetPageNum(subject.GetPageNum() + 1);
    }
    void UnExecute()
    {

        subject.SetPageNum(subject.GetPageNum() - 1);
        subject.SetCursorX(CX);
        subject.SetCursorY(CY);

    }
private:
    ECTextEditor& subject;
    int CX;
    int CY;
};
// End of command definitions
//---------------------------------------------------


// Implements the observer pattern
class ECViewHandler : public ECObserver
{
public:
    ECViewHandler(ECTextEditor& S) : subject(S), CX(0), CY(0), PK(0) {}
    virtual void Update()
    {
        CX = subject.GetCursorX();
        CY = subject.GetCursorY();
        PK = subject.GetPressedKey();

        switch (PK)
        {
        case CTRL_A: //  for testing purposes
            subject.ClearStatusRows();
            subject.AddStatusRow("Len of above line=" + to_string(subject.GetCurrentDocString(CY).length()), "Total len=" + to_string(subject.GetColNumInView()), true);
        //    //subject.AddStatusRow(subject.GetRowString(CY + 1), "", true);
        //    //subject.AddStatusRow("actual col=" + to_string(subject.GetActualCol(CX)), "", true);
            break;
        case CTRL_B:
        {
            subject.ClearStatusRows();
            int i = 0;
            string s = subject.GetCurrentDocString(CY + 1);
            while (s[i] != ' ')
            {
                i += 1;
            }
            s = s.substr(0, i);
            subject.AddStatusRow("first word=" + s, "Length is=" + to_string(s.length()), true);
            //    subject.AddStatusRow(subject.GetRowString(CY + 1), "", true);
            break;
        }
        //case CTRL_C:
        //    exit(1);
        //    break;
        case PAGE_UP:
            if (subject.GetPageNum() == 0)
            {
                break;
            }
            subject.Execute(new PageUp(subject));
            break;
        case PAGE_DOWN:
            subject.Execute(new PageDown(subject));
            break;
        case KEY_NULL:
            // If there is no input, skip this
            break;
        case ARROW_LEFT:
            subject.Execute(new MoveCursorLeft(subject));
            break;
        case ARROW_RIGHT:
            subject.Execute(new MoveCursorRight(subject));
            break;
        case ARROW_DOWN:
            if (CY == subject.GetRowNumInView() - 2)
            {
                subject.Execute(new PageDown(subject));
            }
            else
            {
                subject.Execute(new MoveCursorDown(subject, CX));
            }
            break;
        case ARROW_UP:
            if (CY == 0 && subject.GetPageNum() != 0)
            {
                subject.Execute(new PageUp(subject));
            }
            else
            {
                subject.Execute(new MoveCursorUp(subject));
            }
            break;
        case ENTER:
            subject.Execute(new PressEnter(subject, CX));
            if (CY == subject.GetRowNumInView() - 2)
            {
                subject.Execute(new PageDown(subject));
            }
            break;
        case BACKSPACE:
            {
                if (subject.GetActualCol(CX) == 0 && CY != 0)
                {
                    // Merge two rows if backspacing while on the first position in a row
                    subject.Execute(new BackspaceMerge(subject));
                }
                else if (subject.GetActualCol(CX) != 0)
                {
                    // Delete a character
                    subject.Execute(new DeleteCharacter(subject));
                }
                break;
            }
        case CTRL_Z:
            // Undo a command
            subject.Undo();
            break;
        case CTRL_Y:
            // Redo a command
            subject.Redo();
            break;
        default:
            {
                if (PK >= 32 && PK <= 125)
                {
                    // (then) insert a character
                    subject.Execute(new InputCharacter(subject, char(PK)));
                }
                else
                {
                    // If this is added, it means that the keystoke handler has not been added yet
                    subject.AddRow("Coming Soon..." + to_string(PK));
                }
                break;
            }
        }
    }

private:
    ECTextEditor& subject;
    int CX;
    int CY;
    int PK;
};

#endif // !ECTEXTEDITOR_H
