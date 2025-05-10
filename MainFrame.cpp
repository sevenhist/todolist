#include "MainFrame.hpp"
#include <wx/wx.h>
#include <vector>
#include <string>
#include "Task.hpp"
#include <iostream>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title), db_("tasks.db") 
{
    db_.deleteAllTasks();
    CreateControls();
    BindEventHandlers();
    AddSavedTasks();
}

void MainFrame::CreateControls() {
    wxFont headLineFont(wxFontInfo(wxSize(0, 36)).Bold());
    wxFont mainFont(wxFontInfo(wxSize(0, 16)));

    panel = new wxPanel(this);
    panel->SetFont(mainFont);

    headLineText = new wxStaticText(panel, wxID_ANY, "To-Do List", 
        wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
    headLineText->SetFont(headLineFont);
    //headLineText->SetBackgroundColour(*wxRED);

    inputField = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(100, 80), wxSize(495, 35), wxTE_PROCESS_ENTER);
    addButton = new wxButton(panel, wxID_ANY, "Add", wxPoint(600, 80), wxSize(100, 35));
    checkListBox = new wxCheckListBox(panel, wxID_ANY, wxPoint(100, 120), wxSize(600, 400));
    clearButton = new wxButton(panel, wxID_ANY, "Clear", wxPoint(100, 525), wxSize(100, 35));
    deleteButton = new wxButton(panel, wxID_ANY, "Delete Task", wxPoint(300, 525), wxSize(100, 35));
}

void MainFrame::BindEventHandlers() {
    addButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClicked, this);
    inputField->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInputEnter, this);
    checkListBox->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeyDown, this);
    checkListBox->Bind(wxEVT_CHECKLISTBOX, &MainFrame::OnCheckListBoxToggle, this);
    deleteButton->Bind(wxEVT_BUTTON, &MainFrame::OnDeleteButtonClicked, this);
    clearButton->Bind(wxEVT_BUTTON, &MainFrame::OnClearButtonClicked, this);
    this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnWindowClose, this);
}
        
void MainFrame::OnAddButtonClicked(wxCommandEvent& evt) {
    AddTaskFromInput();
}

void MainFrame::AddTaskFromInput() {
    wxString description = inputField->GetValue();

    if(!description.IsEmpty()) {
        checkListBox->Insert(description, checkListBox->GetCount());
        Task task = {std::string(description.mb_str()), false};
        db_.insertTask(task);
        inputField->Clear();
    }

    inputField->SetFocus();
}

void MainFrame::OnInputEnter(wxCommandEvent& evt) {
    AddTaskFromInput();
}

void MainFrame::OnListKeyDown(wxKeyEvent& evt) {
    switch(evt.GetKeyCode()) {
        case WXK_DELETE:
            DeleteSelectedTask();
            break;
        case 8:
            DeleteSelectedTask();
            break;
        case WXK_UP:
            MoveSelectedTask(-1);
            break;
        case WXK_DOWN:
            MoveSelectedTask(1);
            break;
    }
}

void MainFrame::DeleteSelectedTask() {
    int selectedIndex = checkListBox->GetSelection();
    if(selectedIndex == wxNOT_FOUND) {
        return;
    }

    db_.deleteTask(checkListBox->GetString(selectedIndex));

    checkListBox->Delete(selectedIndex);
}

void MainFrame::OnDeleteButtonClicked(wxCommandEvent& evt) {
    DeleteSelectedTask();
}


void MainFrame::MoveSelectedTask(int offset) {
    int selectedIndex = checkListBox->GetSelection();

    if(selectedIndex == wxNOT_FOUND) {
        return;
    }

    int newIndex = selectedIndex + offset;
    if(newIndex >= 0 && newIndex < checkListBox->GetCount()) {
        SwapTasks(selectedIndex, newIndex);
        checkListBox->SetSelection(newIndex, true);
    }
}

void MainFrame::SwapTasks(int i, int j) {
    Task taskI{checkListBox->GetString(i).ToStdString(), checkListBox->IsChecked(i)};
    Task taskJ{checkListBox->GetString(j).ToStdString(), checkListBox->IsChecked(j)};

    checkListBox->SetString(i, taskJ.description);
    checkListBox->Check(i, taskJ.done);
    
    checkListBox->SetString(j, taskI.description);
    checkListBox->Check(j, taskI.done);
}   

void MainFrame::OnClearButtonClicked(wxCommandEvent& evt) {
    if(checkListBox->IsEmpty()) {
        return;
    }

    wxMessageDialog dialog(this, "Are you sure you want to delete all tasks?", 
        "Clear Modal Window", wxYES_NO | wxCANCEL);

    int result = dialog.ShowModal();

    if(result == wxID_YES) {
        db_.deleteAllTasks();
        checkListBox->Clear();
    }

}

void MainFrame::OnWindowClose(wxCloseEvent& evt) {
    std::vector<Task> tasks;

    for(int i = 0; i < checkListBox->GetCount(); i++) {
        Task task;
        task.description = checkListBox->GetString(i);
        task.done = checkListBox->IsChecked(i);
        tasks.push_back(task);
    }
    SaveTasksToFile(tasks, "../tasks.txt");
    evt.Skip();
}

void MainFrame::AddSavedTasks() {
    std::vector<Task> tasks = LoadTasksFromFile("../tasks.txt");

    for(const Task& task : tasks) {
        int index = checkListBox->GetCount();
        checkListBox->Insert(task.description, index);
        db_.insertTask(task);
        checkListBox->Check(index, task.done);
    }
}

void MainFrame::OnCheckListBoxToggle(wxCommandEvent& evt) {
    int index = evt.GetInt();  
    db_.changeIsDoneOfTask(checkListBox->IsChecked(index), checkListBox->GetString(index));
}
