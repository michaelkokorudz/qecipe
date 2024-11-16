#include "mongoose.h"
#include "routes.h" // Include your routes setup function
#include <iostream>

int main() {
    // Initialize the Mongoose event manager
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);

    try {
        // Set up HTTP routes
        setupRoutes(&mgr);

        std::cout << "Server running at http://localhost:8000\n";

        // Event loop to handle requests
        while (true) {
            mg_mgr_poll(&mgr, 1000); // Poll the manager for events (1000ms timeout)
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
    }

    // Cleanup and free resources
    mg_mgr_free(&mgr);
    return 0;
}
