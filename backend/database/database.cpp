#include "database.h"
#include "inventory.h"
#include <sqlite3.h>
#include <iostream>

// Example: save a fridge item to the database
bool saveFridgeItem(int userId, const std::string& name, int quantity, const std::string& unit, const std::string& expirationDate) {
    sqlite3* db;
    if (sqlite3_open("data/inventory.db", &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string sql = "INSERT INTO FridgeItems (user_id, name, quantity, unit, expiration_date) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, quantity);
        sqlite3_bind_text(stmt, 4, unit.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, expirationDate.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return true;
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    return false;
}
