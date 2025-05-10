#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <SQLiteCpp/SQLiteCpp.h>
#include "Task.hpp"
#include <wx/wx.h>

class taskDB {
public:
    taskDB(std::string filepath);
    void insertTask(const Task& task);
    void deleteTask(wxString desc);
    void deleteAllTasks();
    void changeIsDoneOfTask(bool isDone, wxString desc);
    SQLite::Database& getDB() { return db_; }
private:
    SQLite::Database db_;
};

#endif
