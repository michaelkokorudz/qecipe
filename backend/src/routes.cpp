#include "routes.h"
#include "database.h"
#include "api_methods.h"
#include "mongoose.h"
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <stdexcept>

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
// Function to generate CORS headers
std::string getCORSHeaders() {
    return 
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n";
}

// Handler for CORS preflight requests
void handlePreflight(mg_connection* conn, const mg_http_message* hm) {
    (void)hm; // Unused parameter
    std::string headers = getCORSHeaders() + "Content-Length: 0\r\n";
    mg_http_reply(conn, 200, headers.c_str(), "");
}

// Handler for adding a user
void handleAddUser(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    std::string response_body;
    std::string headers = getCORSHeaders() + "Content-Type: text/plain\r\nConnection: keep-alive\r\n";

    try {
        std::string username = params.at("username");
        std::string password = params.at("password");

        if (AddUser(username, password)) {
            response_body = "User added successfully";
            mg_http_reply(conn, 200, headers.c_str(), "%s", response_body.c_str());
        } else {
            response_body = "User already exists";
            mg_http_reply(conn, 409, headers.c_str(), "%s", response_body.c_str());
        }
    } catch (const std::out_of_range&) {
        response_body = "Missing required parameters: username and password";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    } catch (const std::exception& e) {
        response_body = "Invalid input parameters";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    }
}

// Handler for checking if user exists
void handleCheckUserExists(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    std::string response_body;
    std::string headers = getCORSHeaders() + "Content-Type: text/plain\r\nConnection: keep-alive\r\n";

    try {
        std::string username = params.at("username");

        if (CheckIfUserExists(username)) {
           response_body = "User exists";
           mg_http_reply(conn, 200, headers.c_str(), "%s", response_body.c_str());
        } else {  
            response_body = "User not found";
            mg_http_reply(conn, 404, headers.c_str(), "%s", response_body.c_str());
        }
    } catch (const std::out_of_range&) {
        response_body = "Missing required parameter: username";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    } catch (const std::exception& e) {
        response_body = "Invalid input parameters";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    }
}

// Handler for logging in a user
void handleLoginUser(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    std::string response_body;
    std::string headers = getCORSHeaders() + "Content-Type: text/plain\r\nConnection: keep-alive\r\n";

    try {
        std::string username = params.at("username");
        std::string password = params.at("password");

        if (loginUser(username, password)) {
            response_body = "Login successful";
            mg_http_reply(conn, 200, headers.c_str(), "%s", response_body.c_str());
        } else {
            response_body = "Invalid credentials";
            mg_http_reply(conn, 401, headers.c_str(), "%s", response_body.c_str());
        }
    } catch (const std::out_of_range&) {
        response_body = "Missing required parameters: username and password";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    } catch (const std::exception& e) {
        response_body = "Invalid input parameters";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    }
}

// Handler for getting fridge items
void handleGetFridgeItems(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    std::string response_body;
    std::string headers = getCORSHeaders() + "Content-Type: text/plain\r\nConnection: keep-alive\r\n";

    try {
        std::string username = params.at("username");

        std::vector<std::string> items = GetFridgeItems(username);
        std::ostringstream oss;
        for (const auto& item : items) {
            oss << item << "\n";
        }
        response_body = oss.str();
        mg_http_reply(conn, 200, headers.c_str(), "%s", response_body.c_str());
    } catch (const std::out_of_range&) {
         response_body = "Missing required parameter: username";
         mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    } catch (const std::exception& e) {
         response_body = "Invalid input parameters";
         mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    }
}
void handleAddFridgeItem(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    std::string response_body;
    std::string headers = getCORSHeaders() + "Content-Type: text/plain\r\nConnection: keep-alive\r\n";

    try {
        std::string username = params.at("username");
        std::string name = params.at("name");
        std::string unit = params.at("unit");
        int quantity = std::stoi(params.at("quantity"));
        std::string expirationDate = params.at("expiration_date");

        if (AddFridgeItem(username, name, quantity, unit, expirationDate)) {
            response_body = "Fridge item added successfully";
            mg_http_reply(conn, 200, headers.c_str(), "%s", response_body.c_str());
        } else {
            response_body = "Failed to add Fridge item";
            mg_http_reply(conn, 500, headers.c_str(), "%s", response_body.c_str());
        }
    } catch (const std::out_of_range&) {
        response_body = "Missing required parameters for Fridge item";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    } catch (const std::invalid_argument&) {
        response_body = "Invalid number format in parameters";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    } catch (const std::exception& e) {
        response_body = "Invalid input parameters";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    }
}

// Handler for adding a spice item
void handleAddSpiceItem(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    std::string response_body;
    std::string headers = getCORSHeaders() + "Content-Type: text/plain\r\nConnection: keep-alive\r\n";

    try {
        std::string username = params.at("username");
        std::string name = params.at("name");
        int percentage = std::stoi(params.at("percentage"));
        std::string unit = params.at("unit");
        int referenceAmount = std::stoi(params.at("reference_amount"));
        std::string expirationDate = params.at("expiration_date");

        if (AddSpiceItem(username, name, percentage, unit, referenceAmount, expirationDate)) {
            response_body = "Spice item added successfully";
            mg_http_reply(conn, 200, headers.c_str(), "%s", response_body.c_str());
        } else {
            response_body = "Failed to add spice item";
            mg_http_reply(conn, 500, headers.c_str(), "%s", response_body.c_str());
        }
    } catch (const std::out_of_range&) {
        response_body = "Missing required parameters for spice item";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    } catch (const std::invalid_argument&) {
        response_body = "Invalid number format in parameters";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    } catch (const std::exception& e) {
        response_body = "Invalid input parameters";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    }
}

// Handler for getting spice items
void handleGetSpiceItems(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    std::string response_body;
    std::string headers = getCORSHeaders() + "Content-Type: text/plain\r\nConnection: keep-alive\r\n";

    try {
        std::string username = params.at("username");

        std::vector<std::string> items = GetSpiceItems(username);
        std::ostringstream oss;
        for (const auto& item : items) {
            oss << item << "\n";
        }
        response_body = oss.str();
        mg_http_reply(conn, 200, headers.c_str(), "%s", response_body.c_str());
    } catch (const std::out_of_range&) {
       response_body = "Missing required parameter: username";
       mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    } catch (const std::exception& e) {
       response_body = "Invalid input parameters";
       mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    }
}

// Handler for getting quantity and units of an item
void handleGetQuantityAndUnits(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    std::string response_body;
    std::string headers = getCORSHeaders() + "Content-Type: text/plain\r\nConnection: keep-alive\r\n";

    try {
        std::string username = params.at("username");
        std::string tableType = params.at("table_type");  // "Fridge" or "Spice"
        std::string itemName = params.at("item_name");

        auto [quantity, unit] = GetQuantityAndUnitsForItem(username, tableType, itemName);

        if (quantity != -1) {
            std::ostringstream oss;
            oss << "Quantity: " << quantity << ", Unit: " << unit;
            response_body = oss.str();
            mg_http_reply(conn, 200, headers.c_str(), "%s", response_body.c_str());
        } else {
            response_body = "Item not found";
            mg_http_reply(conn, 404, headers.c_str(), "%s", response_body.c_str());
        }
    } catch (const std::out_of_range&) {
        response_body = "Missing required parameters: username, table_type, and item_name";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    } catch (const std::exception& e) {
        response_body = "Invalid input parameters";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    }
}
// Handler for removing a fridge item
void handleRemoveFridgeItem(mg_connection* conn, const mg_http_message* hm) {
    auto params = parseQueryString(getBody(hm));
    std::string response_body;
    std::string headers = getCORSHeaders() + "Content-Type: text/plain\r\nConnection: keep-alive\r\n";

    try {
        std::string username = params.at("username");
        std::string itemName = params.at("name");

        // Attempt to remove the fridge item
        if (removeFridgeItem(username, itemName)) {
            response_body = "Fridge item removed successfully";
            mg_http_reply(conn, 200, headers.c_str(), "%s", response_body.c_str());
        } else {
            response_body = "Failed to remove Fridge item";
            mg_http_reply(conn, 500, headers.c_str(), "%s", response_body.c_str());
        }
    } catch (const std::out_of_range&) {
        response_body = "Missing required parameters: username and name";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    } catch (const std::exception& e) {
        response_body = "Invalid input parameters";
        mg_http_reply(conn, 400, headers.c_str(), "%s", response_body.c_str());
    }
}





// Setup routes for HTTP server
void setupRoutes(mg_mgr* mgr) {
    mg_connection* c = mg_http_listen(mgr, "http://localhost:8000", 
    [](mg_connection* conn, int ev, void* ev_data) {
        if (ev == MG_EV_HTTP_MSG) {
            auto* hm = static_cast<mg_http_message*>(ev_data);

            std::string uri(hm->uri.buf, hm->uri.len);
            std::string method(hm->method.buf, hm->method.len);

            if (method == "OPTIONS") {
                // Handle CORS preflight
                handlePreflight(conn, hm);
            }
            else if (method == "POST") {
                if (uri == "/add_user") {
                    handleAddUser(conn, hm);
                } else if (uri == "/check_user_exists") {
                    handleCheckUserExists(conn, hm);
                } else if (uri == "/login_user") {
                    handleLoginUser(conn, hm);
                } else if (uri == "/get_fridge_items") {
                    handleGetFridgeItems(conn, hm);
                } else if (uri == "/add_fridge_item") {
                    handleAddFridgeItem(conn, hm);
                } else if (uri == "/add_spice_item") {
                    handleAddSpiceItem(conn, hm);
                } else if (uri == "/get_spice_items") {
                    handleGetSpiceItems(conn, hm);
                } else if (uri == "/get_quantity_and_units") {
                    handleGetQuantityAndUnits(conn, hm);
                } else if (uri == "/remove_fridge_item") {
                    handleRemoveFridgeItem(conn, hm);
                } else {
                    // Unknown POST route
                    std::string response_body = "Not found";
                    std::string headers = getCORSHeaders() + "Content-Type: text/plain\r\nConnection: keep-alive\r\n";
                    mg_http_reply(conn, 404, headers.c_str(), "%s", response_body.c_str());
                }
            }
            else {
                // Method not allowed
                std::string response_body = "Method Not Allowed";
                std::string headers = getCORSHeaders() + "Content-Type: text/plain\r\nConnection: keep-alive\r\n";
                mg_http_reply(conn, 405, headers.c_str(), "%s", response_body.c_str());
            }
        }
    }, nullptr);  // Passing nullptr as the user data parameter if not needed

    if (c == nullptr) {
        // Handle error if mg_http_listen fails
        fprintf(stderr, "Error setting up listener!\n");
        exit(1);
    }
}
