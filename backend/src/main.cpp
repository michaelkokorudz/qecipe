#include "routes.h" // Include routes header where you defined setupRoutes
#include "mongoose.h" // Include mongoose for server setup

int main() {
    // Initialize Mongoose manager
    mg_mgr mgr;
    mg_mgr_init(&mgr);

    // Set up routes
    setupRoutes(&mgr);

    // Start the server loop
    printf("Starting server on http://localhost:8000\n");
    while (true) {
        mg_mgr_poll(&mgr, 1000); // Poll events every second
    }

    // Cleanup
    mg_mgr_free(&mgr);
    return 0;
}
