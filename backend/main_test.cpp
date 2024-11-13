#include <sqlite3.h>
#include <iostream>
#include <string>

int main() {
    sqlite3* db;
    char* errMsg = 0;
    int exit = sqlite3_open("inventory.db", &db);

    if (exit) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return exit;
    } else {
        std::cout << "Connected to the database successfully!" << std::endl;
    }

    // SQL command to insert data into Users table
    std::string sqlInsert = "INSERT INTO Users (username, password) VALUES ('newuser', 'newpassword');";

    // Execute SQL insert statement
    exit = sqlite3_exec(db, sqlInsert.c_str(), 0, 0, &errMsg);
    if (exit != SQLITE_OK) {
        std::cerr << "Error inserting data: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Inserted data successfully!" << std::endl;
    }

    // SQL command to select all users
    std::string sqlSelect = "SELECT * FROM Users;";

    // Callback function to print query results
    auto callback = [](void* NotUsed, int argc, char** argv, char** azColName) -> int {
        for (int i = 0; i < argc; i++) {
            std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << std::endl;
        }
        std::cout << std::endl;
        return 0;
    };

    // Execute SQL select statement
    exit = sqlite3_exec(db, sqlSelect.c_str(), callback, 0, &errMsg);
    if (exit != SQLITE_OK) {
        std::cerr << "Error selecting data: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    // Close the database connection
    sqlite3_close(db);
    return 0;
}
