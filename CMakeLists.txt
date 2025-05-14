cmake_minimum_required(VERSION 3.18)
project(todo_list VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion")

# ========= WX + SQLite ==========
find_package(wxWidgets REQUIRED COMPONENTS net core base)
if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif()
find_package(SQLiteCpp REQUIRED)

# ========= App =========
set(CPP_SRCS
    App.cpp
    MainFrame.cpp
    Task.cpp
    database.cpp
)

add_executable(${PROJECT_NAME} ${CPP_SRCS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES} SQLiteCpp)

# ========= Tests =========
include(CTest)
include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.1
)
FetchContent_MakeAvailable(Catch2)


add_executable(database_tests
    Task.cpp
    database.cpp
    database_test.cpp
)

target_link_libraries(database_tests
    PRIVATE
    Catch2::Catch2WithMain
    SQLiteCpp
)


target_link_libraries(database_tests PRIVATE ${wxWidgets_LIBRARIES})


add_test(NAME RunDatabaseTests COMMAND database_tests)
