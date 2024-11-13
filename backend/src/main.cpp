#include "../external/mongoose/mongoose.h"
#include "routes.h"

int main() {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);

    mg_http_listen(&mgr, "http://localhost:8000", [](mg_connection* c, int ev, void* ev_data, void* fn_data) {
        if (ev == MG_EV_HTTP_MSG) {
            mg_http_reply(c, 200, "Content-Type: text/plain\r\n", "Hello, Mongoose!");
        }
    }, nullptr);

    printf("Starting web server on http://localhost:8000\n");
    for (;;) mg_mgr_poll(&mgr, 1000);
    mg_mgr_free(&mgr);
    return 0;
}
