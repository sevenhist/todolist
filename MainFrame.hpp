#pragma once
#include <wx/wx.h>
#include "database.hpp"

class MainFrame : public wxFrame
{
    public:
        MainFrame(const wxString& title);
    private:
        void CreateControls();
        void SetupSizers();
        void BindEventHandlers();
        void AddSavedTasks();
        
        void OnAddButtonClicked(wxCommandEvent& evt);
        void OnInputEnter(wxCommandEvent& evt);
        void OnDeleteButtonClicked(wxCommandEvent& evt);
        void OnListKeyDown(wxKeyEvent& evt);
        void OnClearButtonClicked(wxCommandEvent& evt);
        void OnWindowClose(wxCloseEvent& evt);
        void OnCheckListBoxToggle(wxCommandEvent& evt);
        
        void AddTaskFromInput();
        void DeleteSelectedTask();
        void MoveSelectedTask(int offset);
        void SwapTasks(int i, int j);

        wxPanel* panel;
        wxStaticText* headLineText;
        wxTextCtrl* inputField;
        wxButton* addButton;
        wxCheckListBox* checkListBox;
        wxButton* clearButton;
        wxButton* deleteButton;
        taskDB db_;
};