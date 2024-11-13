#include "mongoose.h"
#include "routes.h"
#include <cstdio>

void http_event_handler(mg_connection* conn, int ev, void* ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        auto* hm = static_cast<mg_http_message*>(ev_data);
        mg_http_reply(conn, 200, "Content-Type: text/plain\r\n", "Hello, Mongoose!");
    }
}

int main() {
    // Initialize the Mongoose event manager
    mg_mgr mgr;
    mg_mgr_init(&mgr);

    // Start listening for HTTP connections on port 8000
    mg_connection* c = mg_http_listen(&mgr, "http://localhost:8000", http_event_handler, nullptr);

    // Check if the listener connection was created successfully
    if (c == nullptr) {
        fprintf(stderr, "Error starting server on http://localhost:8000\n");
        mg_mgr_free(&mgr);
        return 1;
    }
    printf("Starting web server on http://localhost:8000\n");

    // Poll events in an infinite loop
    for (;;) mg_mgr_poll(&mgr, 1000);

    // Free resources
    mg_mgr_free(&mgr);
    return 0;
}
