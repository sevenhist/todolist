#include "database.hpp"
#include <iostream>

taskDB::taskDB(std::string filepath) 
:db_(filepath, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE)
{
 try {
     db_.exec("CREATE TABLE IF NOT EXISTS tasks(taskID INTEGER PRIMARY KEY AUTOINCREMENT, description TEXT, done BOOL)");
     std::cout << "database initialized" << std::endl;
}
catch (std::exception& e) {
    std::cout << "Exception occured: " << e.what() << std::endl;
}
}

void taskDB::insertTask(const Task& task)
{
    SQLite::Statement query(db_, "INSERT INTO tasks (description, done) VALUES (?, ?)");
    query.bind(1, task.description);
    query.bind(2, task.done);
    query.exec();
}

void taskDB::deleteTask(wxString desc)
{
    SQLite::Statement query(db_, "DELETE FROM tasks WHERE description = ? ");
    query.bind(1, desc);
    query.exec();
}

void taskDB::deleteAllTasks() 
{
    SQLite::Statement query(db_, "DELETE FROM tasks");
    query.exec();  
}

void taskDB::changeIsDoneOfTask(bool isDone, wxString desc) 
{
    SQLite::Statement query(db_, "UPDATE tasks SET done = ? WHERE description = ? ");
    query.bind(1, isDone);
    query.bind(2, desc);

    query.exec();
}