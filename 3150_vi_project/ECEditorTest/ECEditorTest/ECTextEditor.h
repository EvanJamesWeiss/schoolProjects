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
        row += pageNum * (this->GetRowNumInView() - offset);
        return listRows[row];
    }

    void InsertRow(int row, string str = "")
    {
        row += pageNum * (this->GetRowNumInView() - offset);
        listRows.insert(listRows.begin() + row, str);
        this->UpdateView();
    }

    void UpdateRow(int row, string str)
    {
        row += pageNum * (this->GetRowNumInView() - offset);
        listRows[row] = str;
        this->UpdateView();
    }

    void DeleteRow(int row)
    {
        row += pageNum * (this->GetRowNumInView() - offset);
        listRows.erase(listRows.begin() + row);
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
        startpos = (pageNum == 0) ? startpos : startpos - offset;
        int endpos = startpos + this->GetRowNumInView();
        endpos = ((int)currentDoc.size() < endpos) ? currentDoc.size() : endpos;
        
        return endpos - startpos;
    }

    int GetTotalRows()
    {
        return listRows.size();
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

private:

    vector<string> listRows;
    int pageNum = 0;
    int offset = 1;
    vector<int> modifiers;
    vector<string> currentDoc; // = vector<string>();

    ECCommandHistory histCmds;
       
    // If a row was updated in the text editor, it also needs to be updated in the view
    void UpdateView()
    {
        ECTextViewImp::InitRows();
        MapRowsToPage();

        int startpos = pageNum * this->GetRowNumInView();
        startpos = (pageNum == 0) ? startpos : startpos - offset;
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
        string row;

        for (int i = 0; i < (int)listRows.size(); ++i)
        {
            row = listRows[i];
            while ((int)row.size() > this->GetColNumInView() - offset)
            {
                currentDoc.push_back(SplitRow(row));
            }
            currentDoc.push_back(row);
            //cerr << "***" + row << endl << "***" << (int)row.size() << endl << "***" << this->GetRowNumInView();
        }
    }

    string SplitRow(string& row)
    {
        int pos = this->GetColNumInView() - offset;
        while (row[pos] != ' ')
        {
            pos -= 1;
            if (pos == 0) { return row; }
        }
        string output = row.substr(0, pos);
        row = row.substr(pos + 1, row.size());
        return output;
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
        if (subject.GetCursorX() + 1 <= (int)subject.GetRowString(subject.GetCursorY() + 1).length() + 1)
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

class InputCharacter : public ECCommand
{
public:
    InputCharacter(ECTextEditor& doc, char pk) : subject(doc), PK(pk) {}
    void Execute()
    {
        string mystr = subject.GetRowString(subject.GetCursorY() + 1);
        mystr.insert(mystr.begin() + subject.GetCursorX(), PK);
        subject.UpdateRow(subject.GetCursorY() + 1, mystr);
        if (subject.GetCursorX() + 1 <= (int)subject.GetRowString(subject.GetCursorY() + 1).length() + 1)
        {
            subject.SetCursorX(subject.GetCursorX() + 1);
        }
    }
    void UnExecute()
    {
        string mystr = subject.GetRowString(subject.GetCursorY() + 1);
        mystr.erase(mystr.begin() + subject.GetCursorX() - 1);
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

class DeleteCharacter : public ECCommand
{
public:
    DeleteCharacter(ECTextEditor& doc) : subject(doc), PK(char(0)) {}
    void UnExecute()
    {
        string mystr = subject.GetRowString(subject.GetCursorY() + 1);
        mystr.insert(mystr.begin() + subject.GetCursorX(), PK);
        subject.UpdateRow(subject.GetCursorY() + 1, mystr);
        if (subject.GetCursorX() + 1 <= (int)subject.GetRowString(subject.GetCursorY() + 1).length() + 1)
        {
            subject.SetCursorX(subject.GetCursorX() + 1);
        }
    }
    void Execute()
    {
        string mystr = subject.GetRowString(subject.GetCursorY() + 1);
        PK = mystr.at(subject.GetCursorX() - 1);
        mystr.erase(mystr.begin() + subject.GetCursorX() - 1);
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
        subject.InsertRow(subject.GetCursorY() + 2);
        string lineStr = subject.GetRowString(subject.GetCursorY() + 1);
        string frontStr = lineStr.substr(0, CX);
        string backStr = "";
        if (CX != (int)lineStr.length() + 1)
        {
            backStr = lineStr.substr(CX, lineStr.length());
        }
        subject.UpdateRow(subject.GetCursorY() + 1, frontStr);
        subject.UpdateRow(subject.GetCursorY() + 2, backStr);
        subject.SetCursorX(0);
        subject.SetCursorY(subject.GetCursorY() + 1);
    }
    void UnExecute()
    {
        string lineStr = subject.GetRowString(subject.GetCursorY() + 1);
        string aboveStr = subject.GetRowString(subject.GetCursorY());
        string comboStr = aboveStr + lineStr;
        subject.UpdateRow(subject.GetCursorY(), comboStr);
        subject.DeleteRow(subject.GetCursorY() + 1);
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
        subject.InsertRow(subject.GetCursorY() + 2);
        string lineStr = subject.GetRowString(subject.GetCursorY() + 1);
        string frontStr = lineStr.substr(0, subject.GetCursorX());
        string backStr = lineStr.substr(subject.GetCursorX(), lineStr.length());
        subject.UpdateRow(subject.GetCursorY() + 1, frontStr);
        subject.UpdateRow(subject.GetCursorY() + 2, backStr);
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
        subject.SetCursorX(aboveStr.length());
        if (subject.GetCursorY() != 0)
        {
            subject.SetCursorY(subject.GetCursorY() - 1);
        }
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
            if (CX == 0 && CY != 0)
            {
                // Merge two rows if backspacing while on the first position in a row
                subject.Execute(new BackspaceMerge(subject));
            }
            else if (CX != 0)
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
                // input a character
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
