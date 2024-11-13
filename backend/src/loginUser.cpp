#include <sqlite3.h>
#include <iostream>
#include <string>

int loginUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt;
    std::string sql = "SELECT user_id FROM Users WHERE username = ? AND password = ?;";
    int userId = -1;  // Default to -1 if login fails

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            userId = sqlite3_column_int(stmt, 0);  // Fetch the user_id
            std::cout << "Login successful. User ID: " << userId << std::endl;
        } else {
            std::cout << "Invalid username or password." << std::endl;
        }
    }
    sqlite3_finalize(stmt);
    return userId;  // Returns -1 if login fails, user_id otherwise
}
