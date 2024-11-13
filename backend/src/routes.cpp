#include "routes.h"
#include "database.h"
#include "inventory.h"
#include <string>
#include <ctime>
#include <sstream>
#include <map>
#include "mongoose.h"

// Global Inventory instance
Inventory inventory;

std::string getBody(const mg_http_message* hm) {
    return hm->body.len > 0 ? std::string(hm->body.buf, hm->body.len) : "";
}

std::string getUri(const mg_http_message* hm) {
    return hm->uri.len > 0 ? std::string(hm->uri.buf, hm->uri.len) : "";
}

std::string getMethod(const mg_http_message* hm) {
    return hm->method.len > 0 ? std::string(hm->method.buf, hm->method.len) : "";
}

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

std::time_t parseDate(const std::string& dateStr) {
    std::tm tm = {};
    if (dateStr.size() == 10) {  // Expected format: YYYY-MM-DD
        tm.tm_year = std::stoi(dateStr.substr(0, 4)) - 1900;
        tm.tm_mon = std::stoi(dateStr.substr(5, 2)) - 1;
        tm.tm_mday = std::stoi(dateStr.substr(8, 2));
        return std::mktime(&tm);
    }
    return std::time_t(-1);  // Return invalid time if format is incorrect
}

void handleAddFridgeItem(struct mg_connection* conn, struct mg_http_message* hm) {
    std::string body = getBody(hm);  // Manually retrieve the body
    auto params = parseQueryString(body);

    try {
        int userId = std::stoi(params.at("user_id"));
        std::string name = params.at("name");
        int quantity = std::stoi(params.at("quantity"));
        std::string unit = params.at("unit");
        std::string expirationDate = params.at("expiration_date");

        std::time_t expirationTime = parseDate(expirationDate);

        inventory.addFridgeItem(userId, name, quantity, unit, expirationTime);

        bool saveSuccess = saveFridgeItem(userId, name, quantity, unit, expirationDate);

        if (saveSuccess) {
            mg_http_reply(conn, 200, "Content-Type: text/plain\r\n", "Item added successfully");
        } else {
            mg_http_reply(conn, 500, "Content-Type: text/plain\r\n", "Failed to add item to database");
        }
    } catch (const std::exception& e) {
        mg_http_reply(conn, 400, "Content-Type: text/plain\r\n", "Invalid input parameters");
    }
}

bool uriEquals(const std::string& uri, const char* str) {
    return uri == str;
}

// Setup routes with inline lambda for event handling
void setupRoutes(mg_mgr* mgr) {
    mg_connection* c = mg_http_listen(mgr, "http://localhost:8000", [](mg_connection* conn, int ev, void* ev_data) {
        if (ev == MG_EV_HTTP_MSG) {
            auto* hm = static_cast<mg_http_message*>(ev_data);

            std::string uri = getUri(hm);
            std::string method = getMethod(hm);

            if (uriEquals(uri, "/add_fridge_item") && method == "POST") {
                handleAddFridgeItem(conn, hm);
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
