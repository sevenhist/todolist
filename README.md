## 📝 Storing Tasks Data in a SQLite3 Database

This project is a To-Do List application built with C++ and wxWidgets that allows users to create, manage, and persistently store their tasks. It integrates SQLite3 as a lightweight relational database to ensure tasks are saved across sessions. The application's user interface is intuitive, and designed with simplicity in mind.

## 📦 Technologies Used

C++17

wxWidgets (GUI Framework)

SQLite3 (via SQLiteCpp wrapper)

Standard C++ STL (for data handling and file I/O)

## 🧠 Features

Add, delete, and mark tasks as complete/incomplete.

Keyboard shortcuts for task management.

Persistent storage via SQLite.

Confirmation dialog for clearing all tasks.

Data saved to file on close for redundancy.

## ✅ Compile and run the Project

```shell
mkdir build
cd build
cmake ..
make -j8
```

## 🔍 How to Access the Database

```shell
cd build
sqlite3 tasks.db
```

## 🗄️ Database

```mermaid
classDiagram
    class Task {
        +std::string description
        +bool done
    }

    class MainFrame {
        +MainFrame(wxString title)
        -void CreateControls()
        -void BindEventHandlers()
        -void AddSavedTasks()
        -void AddTaskFromInput()
        -void DeleteSelectedTask()
        -void MoveSelectedTask(int offset)
        -void SwapTasks(int i, int j)
        -void OnAddButtonClicked(wxCommandEvent& evt)
        -void OnInputEnter(wxCommandEvent& evt)
        -void OnDeleteButtonClicked(wxCommandEvent& evt)
        -void OnListKeyDown(wxKeyEvent& evt)
        -void OnClearButtonClicked(wxCommandEvent& evt)
        -void OnWindowClose(wxCloseEvent& evt)
        -void OnCheckListBoxToggle(wxCommandEvent& evt)
        -wxPanel* panel
        -wxStaticText* headLineText
        -wxTextCtrl* inputField
        -wxButton* addButton
        -wxCheckListBox* checkListBox
        -wxButton* clearButton
        -wxButton* deleteButton
        -taskDB db_
    }

    class taskDB {
        +taskDB(std::string filepath)
        +void insertTask(Task task)
        +void deleteTask(wxString desc)
        +void deleteAllTasks()
        +void changeIsDoneOfTask(bool isDone, wxString desc)
        +SQLite::Database& getDB()
        -SQLite::Database db_
    }

    class App {
        +bool OnInit()
    }

    MainFrame --> taskDB : uses
    taskDB --> Task : uses
    App --> MainFrame : creates
```