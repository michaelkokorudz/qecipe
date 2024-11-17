#include "database.h"
#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>


// Helper function to get the full path of the database
std::string getDatabasePath() {
    return "/Users/michaelkokorudz/Documents/3rd Year Sem 1/CMPE320/workspaceC++/leftoverlab/backend/database/inventory.db";
}

// Ensure the database directory exists
void ensureDatabaseDirectoryExists() {
    const std::string dir = "backend/database/";
    struct stat info;
    if (stat(dir.c_str(), &info) != 0) {
        mkdir(dir.c_str(), 0755);  // Create directory with proper permissions
    }
}


// Generate a table name for the fridge based on the username
std::string generateTableName(const std::string& username, const std::string& tableType) {
    return tableType + "_" + username;
}

// Initialize the core database structure
void initializeDatabase() {
    ensureDatabaseDirectoryExists();  // Ensure the directory exists

    sqlite3* db;
    std::string dbPath = getDatabasePath();
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database at path: " << dbPath << ". Error: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* createUserTableSQL = R"sql(
        CREATE TABLE IF NOT EXISTS User (
            username TEXT PRIMARY KEY,
            password TEXT NOT NULL,
            time_signed_up TEXT NOT NULL
        );
    )sql";

    char* errorMessage = nullptr;
    if (sqlite3_exec(db, createUserTableSQL, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error creating User table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    } else {
        std::cout << "User table initialized successfully.\n";
    }

    sqlite3_close(db);
}

bool addUser(const std::string& username, const std::string& password) {
    sqlite3* db;
    std::string dbPath = getDatabasePath();
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Insert into the global User table
    const char* insertSQL = "INSERT INTO User (username, password, time_signed_up) VALUES (?, ?, datetime('now'));";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "User added successfully: " << username << "\n";
        } else {
            std::cerr << "Error adding user to global table: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return false;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing statement for global table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Insert credentials into User_<username> table
    std::string userTableName = generateTableName(username, "User");
    std::string createUserTableSQL =
        "CREATE TABLE IF NOT EXISTS " + userTableName + R"sql(
            (username TEXT PRIMARY KEY,
             password TEXT NOT NULL,
             time_signed_up TEXT NOT NULL
            )
        )sql";

    char* errorMessage = nullptr;
    if (sqlite3_exec(db, createUserTableSQL.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error creating user-specific table for " << username << ": " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        sqlite3_close(db);
        return false;
    }

    std::string userTableInsertSQL =
        "INSERT INTO " + userTableName + " (username, password, time_signed_up) VALUES (?, ?, datetime('now'));";

    if (sqlite3_prepare_v2(db, userTableInsertSQL.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Credentials added to table '" << userTableName << "'.\n";
        } else {
            std::cerr << "Error adding credentials to user-specific table: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return false;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing statement for user-specific table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    return true;
}


// Check if a user exists in the User table
bool userExists(const std::string& username) {
    sqlite3* db;
    std::string dbPath = getDatabasePath();
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    const char* checkSQL = "SELECT 1 FROM User WHERE username = ?;";
    sqlite3_stmt* stmt;
    bool exists = false;

    if (sqlite3_prepare_v2(db, checkSQL, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = true;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_close(db);
    return exists;
}

bool createUserSpecificTables(const std::string& username) {
    sqlite3* db;
    std::string dbPath = getDatabasePath();
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Create Fridge_<username> table
    std::string fridgeTableSQL = 
        "CREATE TABLE IF NOT EXISTS " + generateTableName(username, "Fridge") + R"sql(
            (name TEXT NOT NULL,
             quantity INTEGER NOT NULL,
             unit TEXT NOT NULL,
             expiration_date TEXT NOT NULL
            );
        )sql";

    // Create Spice_<username> table
    std::string spiceTableSQL = 
        "CREATE TABLE IF NOT EXISTS " + generateTableName(username, "Spice") + R"sql(
            (name TEXT NOT NULL,
             percentage INTEGER NOT NULL,
             unit TEXT NOT NULL,
             reference_amount INTEGER NOT NULL
            );
        )sql";

    // Create User_<username> table for login credentials
    std::string userTableSQL = 
        "CREATE TABLE IF NOT EXISTS " + generateTableName(username, "User") + R"sql(
            (username TEXT PRIMARY KEY,
             password TEXT NOT NULL,
             time_signed_up TEXT NOT NULL
            );
        )sql";

    char* errorMessage = nullptr;

    // Execute fridge table creation
    if (sqlite3_exec(db, fridgeTableSQL.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error creating fridge table for " << username << ": " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        sqlite3_close(db);
        return false;
    }

    // Execute spice table creation
    if (sqlite3_exec(db, spiceTableSQL.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error creating spice table for " << username << ": " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        sqlite3_close(db);
        return false;
    }

    // Execute user-specific login table creation
    if (sqlite3_exec(db, userTableSQL.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error creating user-specific login table for " << username << ": " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        sqlite3_close(db);
        return false;
    }

    std::cout << "Tables created or verified for user: " << username << "\n";
    sqlite3_close(db);
    return true;
}

// Add or update a fridge item
bool saveFridgeItem(const std::string& username, const std::string& name, int quantity, const std::string& unit, const std::string& expirationDate) {
    sqlite3* db;
    std::string dbPath = getDatabasePath();
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string tableName = generateTableName(username, "Fridge");

    // Start a transaction to ensure atomicity
    const char* beginTransaction = "BEGIN TRANSACTION;";
    if (sqlite3_exec(db, beginTransaction, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to start transaction: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Check if the item already exists
    std::string checkSQL = "SELECT quantity, expiration_date FROM " + tableName + " WHERE name = ? AND unit = ?;";
    sqlite3_stmt* checkStmt;
    if (sqlite3_prepare_v2(db, checkSQL.c_str(), -1, &checkStmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(checkStmt, 1, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(checkStmt, 2, unit.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            // Item exists, update quantity and expiration date
            int existingQuantity = sqlite3_column_int(checkStmt, 0);
            std::string existingExpiration = reinterpret_cast<const char*>(sqlite3_column_text(checkStmt, 1));

            int newQuantity = existingQuantity + quantity;
            std::string newExpirationDate = (expirationDate < existingExpiration) ? expirationDate : existingExpiration;

            sqlite3_finalize(checkStmt);

            std::string updateSQL = "UPDATE " + tableName + " SET quantity = ?, expiration_date = ? WHERE name = ? AND unit = ?;";
            sqlite3_stmt* updateStmt;
            if (sqlite3_prepare_v2(db, updateSQL.c_str(), -1, &updateStmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(updateStmt, 1, newQuantity);
                sqlite3_bind_text(updateStmt, 2, newExpirationDate.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(updateStmt, 3, name.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(updateStmt, 4, unit.c_str(), -1, SQLITE_STATIC);

                if (sqlite3_step(updateStmt) == SQLITE_DONE) {
                    std::cout << "Updated item '" << name << "' in table '" << tableName << "'.\n";
                } else {
                    std::cerr << "Error updating item: " << sqlite3_errmsg(db) << std::endl;
                }
                sqlite3_finalize(updateStmt);
            }
        } else {
            // Item does not exist, insert as new
            sqlite3_finalize(checkStmt);

            std::string insertSQL = "INSERT INTO " + tableName + " (name, quantity, unit, expiration_date) VALUES (?, ?, ?, ?);";
            sqlite3_stmt* insertStmt;
            if (sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &insertStmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(insertStmt, 1, name.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(insertStmt, 2, quantity);
                sqlite3_bind_text(insertStmt, 3, unit.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(insertStmt, 4, expirationDate.c_str(), -1, SQLITE_STATIC);

                if (sqlite3_step(insertStmt) == SQLITE_DONE) {
                    std::cout << "Inserted item '" << name << "' into table '" << tableName << "'.\n";
                } else {
                    std::cerr << "Error inserting item: " << sqlite3_errmsg(db) << std::endl;
                }
                sqlite3_finalize(insertStmt);
            }
        }
    } else {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
    }

    // Commit the transaction
    const char* commitTransaction = "COMMIT;";
    if (sqlite3_exec(db, commitTransaction, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to commit transaction: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_close(db);
    return true;
}

// Retrieve fridge items for a user
bool getFridgeItems(const std::string& username, std::vector<std::string>& items) {
    sqlite3* db;
    std::string dbPath = getDatabasePath();
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string tableName = generateTableName(username, "Fridge");
    std::string sql = "SELECT name, quantity, unit, expiration_date FROM " + tableName + ";";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string item = "Name: ";
            item += reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            item += ", Quantity: " + std::to_string(sqlite3_column_int(stmt, 1));
            item += ", Unit: " + std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            item += ", Expiration: " + std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));

            items.push_back(item);
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    }

    std::cerr << "Error retrieving items for user " << username << ": " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return false;
}
// Add or update a spice item
bool saveSpiceItem(const std::string& username, const std::string& name, int percentage, const std::string& unit, int referenceAmount) {
    sqlite3* db;
    std::string dbPath = getDatabasePath();
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string tableName = generateTableName(username, "Spice");

    // Start a transaction to ensure atomicity
    const char* beginTransaction = "BEGIN TRANSACTION;";
    if (sqlite3_exec(db, beginTransaction, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to start transaction: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Check if the spice item already exists
    std::string checkSQL = "SELECT percentage, reference_amount FROM " + tableName + " WHERE name = ? AND unit = ?;";
    sqlite3_stmt* checkStmt;
    if (sqlite3_prepare_v2(db, checkSQL.c_str(), -1, &checkStmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(checkStmt, 1, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(checkStmt, 2, unit.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            // Spice item exists, update percentage and reference amount
            int existingPercentage = sqlite3_column_int(checkStmt, 0);
            int existingReference = sqlite3_column_int(checkStmt, 1);

            int newReferenceAmount = existingReference + referenceAmount;

            // Calculate new percentage as a weighted average
            int newPercentage = static_cast<int>(
                (existingPercentage * existingReference + percentage * referenceAmount) / static_cast<double>(newReferenceAmount)
            );

            sqlite3_finalize(checkStmt);

            std::string updateSQL = "UPDATE " + tableName + " SET percentage = ?, reference_amount = ? WHERE name = ? AND unit = ?;";
            sqlite3_stmt* updateStmt;
            if (sqlite3_prepare_v2(db, updateSQL.c_str(), -1, &updateStmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(updateStmt, 1, newPercentage);
                sqlite3_bind_int(updateStmt, 2, newReferenceAmount);
                sqlite3_bind_text(updateStmt, 3, name.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(updateStmt, 4, unit.c_str(), -1, SQLITE_STATIC);

                if (sqlite3_step(updateStmt) == SQLITE_DONE) {
                    std::cout << "Updated spice item '" << name << "' in table '" << tableName << "'.\n";
                } else {
                    std::cerr << "Error updating spice item: " << sqlite3_errmsg(db) << std::endl;
                }
                sqlite3_finalize(updateStmt);
            } else {
                std::cerr << "Error preparing update statement: " << sqlite3_errmsg(db) << std::endl;
            }
        } else {
            // Spice item does not exist, insert as new
            sqlite3_finalize(checkStmt);

            std::string insertSQL = "INSERT INTO " + tableName + " (name, percentage, unit, reference_amount) VALUES (?, ?, ?, ?);";
            sqlite3_stmt* insertStmt;
            if (sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &insertStmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(insertStmt, 1, name.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(insertStmt, 2, percentage);
                sqlite3_bind_text(insertStmt, 3, unit.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(insertStmt, 4, referenceAmount);

                if (sqlite3_step(insertStmt) == SQLITE_DONE) {
                    std::cout << "Inserted spice item '" << name << "' into table '" << tableName << "'.\n";
                } else {
                    std::cerr << "Error inserting spice item: " << sqlite3_errmsg(db) << std::endl;
                }
                sqlite3_finalize(insertStmt);
            } else {
                std::cerr << "Error preparing insert statement: " << sqlite3_errmsg(db) << std::endl;
            }
        }
    } else {
        std::cerr << "Error preparing check statement: " << sqlite3_errmsg(db) << std::endl;
    }

    // Commit the transaction
    const char* commitTransaction = "COMMIT;";
    if (sqlite3_exec(db, commitTransaction, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to commit transaction: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // Rollback transaction in case of failure
    }

    sqlite3_close(db);
    return true;
}

// Retrieve spice items for a user
bool getSpiceItems(const std::string& username, std::vector<std::string>& items) {
    sqlite3* db;
    std::string dbPath = getDatabasePath();
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string tableName = generateTableName(username, "Spice");
    std::string sql = "SELECT name, percentage, unit, reference_amount FROM " + tableName + ";";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string item = "Name: ";
            item += reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            item += ", Percentage: " + std::to_string(sqlite3_column_int(stmt, 1));
            item += "%, Unit: " + std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            item += ", Reference Amount: " + std::to_string(sqlite3_column_int(stmt, 3));

            items.push_back(item);
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    }

    std::cerr << "Error retrieving spice items for user " << username << ": " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return false;
}

std::string getUsernameById(int userId) {
    sqlite3* db;
    std::string username;

    std::string dbPath = getDatabasePath();
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    std::string sql = "SELECT username FROM User WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        } else {
            std::cerr << "User with ID " << userId << " not found.\n";
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_close(db);
    return username;
}