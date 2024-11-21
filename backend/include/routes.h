#ifndef ROUTES_H
#define ROUTES_H

#include "mongoose.h"
#include <string>
#include <map>

// Utility functions
void addCORSHeaders(mg_connection* conn);
void handlePreflight(mg_connection* conn);
std::string getBody(const mg_http_message* hm);
std::map<std::string, std::string> parseQueryString(const std::string& body);

// Request handlers
void handleAddUser(mg_connection* conn, const mg_http_message* hm);
void handleCheckUserExists(mg_connection* conn, const mg_http_message* hm);
void handleLoginUser(mg_connection* conn, const mg_http_message* hm);
void handleGetFridgeItems(mg_connection* conn, const mg_http_message* hm);
void handleAddSpiceItem(mg_connection* conn, const mg_http_message* hm);
void handleGetSpiceItems(mg_connection* conn, const mg_http_message* hm);
void handleGetQuantityAndUnits(mg_connection* conn, const mg_http_message* hm);

// Route setup
void setupRoutes(mg_mgr* mgr);

#endif // ROUTES_H
