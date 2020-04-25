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
        // The get modifier calls need to be before it's modified
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

    int GetActualCol(int CX)
    {
        int mod = 0;
        // doesn't work: Y + 1, AY + 1 (2nd page), 
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
        return (pageNum * (this->GetRowNumInView() - 1)) - GetOffset();
    }

private:

    vector<string> listRows;
    int pageNum = 0;
    vector<int> modifiers;
    vector<string> currentDoc; // = vector<string>();

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
        subject.SetCursorX(0);
        if (subject.GetCursorY() != 0)
        {
            subject.SetCursorY(subject.GetCursorY() - 1);
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
        subject.SetCursorX(0);
        if (subject.GetCursorY() + 2 < subject.GetNumFilledRows())
        {
            subject.SetCursorY(subject.GetCursorY() + 1);
        }
    }

private:
    ECTextEditor& subject;
    int CX;
};

// Works for first page
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
    DeleteCharacter(ECTextEditor& doc) : subject(doc), PK(char(0)) {}
    void UnExecute()
    {
        int CX = subject.GetCursorX();
        string mystr = subject.GetRowString(subject.GetCursorY() + 1);
        mystr.insert(mystr.begin() + subject.GetActualCol(CX), PK);
        subject.UpdateRow(subject.GetCursorY() + 1, mystr);
        if (subject.GetCursorX() + 1 <= (int)subject.GetRowString(subject.GetCursorY() + 1).length() + 1)
        {
            subject.SetCursorX(subject.GetCursorX() + 1);
        }
    }
    void Execute()
    {
        int CX = subject.GetCursorX();
        string mystr = subject.GetRowString(subject.GetCursorY() + 1);
        PK = mystr.at(subject.GetActualCol(CX) - 1);
        mystr.erase(mystr.begin() + subject.GetActualCol(CX) - 1);
        subject.UpdateRow(subject.GetCursorY() + 1, mystr);
        if (subject.GetCursorX() - 1 >= 0)
        {
            subject.SetCursorX(subject.GetCursorX() - 1);
        }
    }

private:
    ECTextEditor& subject;
    char PK;
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
        subject.UpdateRow(subject.GetCursorY(), comboStr);
        subject.DeleteRow(subject.GetCursorY() + 1);
        if (subject.GetCursorY() != 0)
        {
            subject.SetCursorY(subject.GetCursorY() - 1);
        }
        int currlen = subject.GetCurrentDocString(subject.GetCursorY() + 1).length();
        subject.SetCursorX(((int)aboveStr.length() > subject.GetColNumInView()) ? currlen - lineStr.length() : aboveStr.length());
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
        case CTRL_A:
            subject.ClearStatusRows();
            subject.AddStatusRow("mod=" + to_string(subject.GetModifier(CY + 1)) + ", pagenum = " + to_string(subject.GetPageNum()), "AX, AY=" + to_string(subject.GetActualCol(CX)) + ", " + to_string(subject.GetActualRow(CY + 1)) + " : x,y=" + to_string(CX) + ", " + to_string(CY), true);
            //subject.AddStatusRow(subject.GetRowString(CY + 1), "", true);
            //subject.AddStatusRow("actual col=" + to_string(subject.GetActualCol(CX)), "", true);
            break;
        case CTRL_B:
            subject.ClearStatusRows();
            subject.AddStatusRow(subject.GetRowString(CY + 1), "", true);
            break;
        case CTRL_C:
            exit(1);
            break;
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
