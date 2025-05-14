#include "database.hpp"
#include "Task.hpp"
#include <sstream>
#include <string>
#include <filesystem>
#include <SQLiteCpp/SQLiteCpp.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_case_sensitive.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

using Catch::CaseSensitive;
using Catch::Matchers::Equals;

TEST_CASE("Database checking", "[read]")
{
    std::string db_path = "check.db";

    if (std::filesystem::exists(db_path))
    {
        std::filesystem::remove(db_path);
    }
    taskDB db(db_path);

    SECTION("insertTask(const Task& task) CHECKING")
    {
        Task t;
        t.description = "Test description";
        t.done = false;

        db.insertTask(t);

        SQLite::Statement query(db.getDB(), "SELECT description, done FROM tasks WHERE description = ?");
        query.bind(1, t.description);

        REQUIRE(query.executeStep());
        REQUIRE(query.getColumn(0).getString() == t.description);
        REQUIRE(query.getColumn(1).getInt() == 0);
    }
    SECTION("deleteTask(wxString desc) CHECKING")
    {
        Task b;
        b.description = "Test 2 description";
        b.done = true;

        db.insertTask(b);

        db.deleteTask(b.description);

        SQLite::Statement query(db.getDB(), "SELECT description, done FROM tasks WHERE description = ?");
        query.bind(1, b.description);

        REQUIRE_FALSE(query.executeStep());
    }
    SECTION("changeIsDoneOfTask(bool isDone, wxString desc) CHECKING")
    {
        Task c;
        c.description = "Test 3 description";
        c.done = true;

        db.insertTask(c);

        db.changeIsDoneOfTask(!c.done, c.description);

        SQLite::Statement query(db.getDB(), "SELECT description, done FROM tasks WHERE description = ?");
        query.bind(1, c.description);

        REQUIRE(query.executeStep());
        REQUIRE(query.getColumn(0).getString() == c.description);
        REQUIRE(query.getColumn(1).getInt() == 0);
    }
    SECTION("deleteAllTasks() CHECKING")
    {
        Task e;
        e.description = "Test 4 description";
        e.done = false;

        Task f;
        f.description = "Test 5 description";
        f.done = true;

        Task g;
        g.description = "Test 6 description";
        g.done = false;

        Task j;
        j.description = "Test 7 description";
        j.done = true;

        Task k;
        k.description = "Test 8 description";
        k.done = false;

        db.insertTask(e);
        db.insertTask(f);
        db.insertTask(g);
        db.insertTask(j);
        db.insertTask(k);

        db.deleteAllTasks();

        SQLite::Statement query(db.getDB(), "SELECT * FROM tasks");

        REQUIRE_FALSE(query.executeStep());
    }
    std::filesystem::remove(db_path);
}