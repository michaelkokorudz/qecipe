#include "routes.h"
#include "database.h"
#include "api_methods.h"
#include "mongoose.h"
#include <string>
#include <sstream>
#include <map>

// Utility to get HTTP request body
std::string getBody(const mg_http_message* hm) {
    return hm->body.len > 0 ? std::string(hm->body.buf, hm->body.len) : "";
}

// Parse query string from body
std::map<std::string, std::string> parseQueryString(const std::string& body) {
    std::map<std::string, std::string> params;
    std::istringstream stream(body);
    std::string token;
    while (std::getline(stream, token, '&')) {
        size_t pos = token.find('=');
        if (pos != std::string::npos) {
            std::string key = token.substr(0, pos);
            std::string value = token.substr(pos + 1);
            params[key] = value;
        }
    }
    return params;
}

// Handler for adding a user
void handleAddUser(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    try {
        std::string username = params.at("username");
        std::string password = params.at("password");

        if (AddUser(username, password)) {
            mg_http_reply(conn, 200, "Content-Type: text/plain\r\n", "User added successfully");
        } else {
            mg_http_reply(conn, 400, "Content-Type: text/plain\r\n", "User already exists");
        }
    } catch (...) {
        mg_http_reply(conn, 400, "Content-Type: text/plain\r\n", "Invalid input parameters");
    }
}

// Handler for checking if user exists
void handleCheckUserExists(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    try {
        std::string username = params.at("username");

        if (CheckIfUserExists(username)) {
            mg_http_reply(conn, 200, "Content-Type: text/plain\r\n", "User exists");
        } else {
            mg_http_reply(conn, 404, "Content-Type: text/plain\r\n", "User not found");
        }
    } catch (...) {
        mg_http_reply(conn, 400, "Content-Type: text/plain\r\n", "Invalid input parameters");
    }
}

// Handler for logging in a user
void handleLoginUser(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    try {
        std::string username = params.at("username");
        std::string password = params.at("password");

        if (loginUser(username, password)) {
            mg_http_reply(conn, 200, "Content-Type: text/plain\r\n", "Login successful");
        } else {
            mg_http_reply(conn, 401, "Content-Type: text/plain\r\n", "Invalid credentials");
        }
    } catch (...) {
        mg_http_reply(conn, 400, "Content-Type: text/plain\r\n", "Invalid input parameters");
    }
}

// Handler for getting fridge items
void handleGetFridgeItems(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    try {
        std::string username = params.at("username");

        std::vector<std::string> items = GetFridgeItems(username);
        std::ostringstream response;
        for (const auto& item : items) {
            response << item << "\n";
        }

        mg_http_reply(conn, 200, "Content-Type: text/plain\r\n", response.str().c_str());
    } catch (...) {
        mg_http_reply(conn, 400, "Content-Type: text/plain\r\n", "Invalid input parameters");
    }
}

// Handler for adding a spice item
void handleAddSpiceItem(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    try {
        std::string username = params.at("username");
        std::string name = params.at("name");
        int percentage = std::stoi(params.at("percentage"));
        std::string unit = params.at("unit");
        int referenceAmount = std::stoi(params.at("reference_amount"));
        std::string expirationDate = params.at("expiration_date");

        if (AddSpiceItem(username, name, percentage, unit, referenceAmount, expirationDate)) {
            mg_http_reply(conn, 200, "Content-Type: text/plain\r\n", "Spice item added successfully");
        } else {
            mg_http_reply(conn, 500, "Content-Type: text/plain\r\n", "Failed to add spice item");
        }
    } catch (...) {
        mg_http_reply(conn, 400, "Content-Type: text/plain\r\n", "Invalid input parameters");
    }
}

// Handler for getting spice items
void handleGetSpiceItems(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    try {
        std::string username = params.at("username");

        std::vector<std::string> items = GetSpiceItems(username);
        std::ostringstream response;
        for (const auto& item : items) {
            response << item << "\n";
        }

        mg_http_reply(conn, 200, "Content-Type: text/plain\r\n", response.str().c_str());
    } catch (...) {
        mg_http_reply(conn, 400, "Content-Type: text/plain\r\n", "Invalid input parameters");
    }
}

// Handler for getting quantity and units of an item
void handleGetQuantityAndUnits(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    try {
        std::string username = params.at("username");
        std::string tableType = params.at("table_type");  // "Fridge" or "Spice"
        std::string itemName = params.at("item_name");

        auto [quantity, unit] = GetQuantityAndUnitsForItem(username, tableType, itemName);

        if (quantity != -1) {
            std::ostringstream response;
            response << "Quantity: " << quantity << ", Unit: " << unit;
            mg_http_reply(conn, 200, "Content-Type: text/plain\r\n", response.str().c_str());
        } else {
            mg_http_reply(conn, 404, "Content-Type: text/plain\r\n", "Item not found");
        }
    } catch (...) {
        mg_http_reply(conn, 400, "Content-Type: text/plain\r\n", "Invalid input parameters");
    }
}

// Setup routes for HTTP server
void setupRoutes(mg_mgr* mgr) {
    mg_connection* c = mg_http_listen(mgr, "http://localhost:8000", [](mg_connection* conn, int ev, void* ev_data) {
        if (ev == MG_EV_HTTP_MSG) {
            auto* hm = static_cast<mg_http_message*>(ev_data);

            std::string uri(hm->uri.buf, hm->uri.len);
            std::string method(hm->method.buf, hm->method.len);

            // Route the request to the appropriate handler
            if (uri == "/add_user" && method == "POST") {
                handleAddUser(conn, hm);
            } else if (uri == "/check_user_exists" && method == "POST") {
                handleCheckUserExists(conn, hm);
            } else if (uri == "/login_user" && method == "POST") {
                handleLoginUser(conn, hm);
            } else if (uri == "/get_fridge_items" && method == "POST") {
                handleGetFridgeItems(conn, hm);
            } else if (uri == "/add_spice_item" && method == "POST") {
                handleAddSpiceItem(conn, hm);
            } else if (uri == "/get_spice_items" && method == "POST") {
                handleGetSpiceItems(conn, hm);
            } else if (uri == "/get_quantity_and_units" && method == "POST") {
                handleGetQuantityAndUnits(conn, hm);
            } else {
                mg_http_reply(conn, 404, "Content-Type: text/plain\r\n", "Not found");
            }
        }
   }, nullptr);  // Passing nullptr as the user data parameter if not needed

    if (c == nullptr) {
        // Handle error if mg_http_listen fails
        fprintf(stderr, "Error setting up listener!\n");
        exit(1);
    }
}

