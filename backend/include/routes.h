#ifndef ROUTES_H
#define ROUTES_H

#include "mongoose.h"
#include <string>
#include <map>
#include <ctime>

// Initializes routes and starts the HTTP listener
void setupRoutes(struct mg_mgr *mgr);

// Parses the body from an HTTP message
std::string getBody(const struct mg_http_message* hm);

// Parses the URI from an HTTP message
std::string getUri(const struct mg_http_message* hm);

// Parses the HTTP method from an HTTP message
std::string getMethod(const struct mg_http_message* hm);

// Parses a URL query string into a key-value map
std::map<std::string, std::string> parseQueryString(const std::string& body);

// Parses a date string in "YYYY-MM-DD" format to std::time_t
std::time_t parseDate(const std::string& dateStr);

// Compares a URI with a target string
bool uriEquals(const std::string& uri, const char* str);

// Handles adding an item to the fridge inventory
void handleAddFridgeItem(struct mg_connection* conn, struct mg_http_message* hm);

#endif // ROUTES_H
