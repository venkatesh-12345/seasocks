
#include "seasocks/PageHandler.h"
#include "seasocks/PrintfLogger.h"
#include "seasocks/Server.h"
#include "seasocks/WebSocket.h"
#include "seasocks/StringUtil.h"
#include "mysql.cpp"
#include <memory>
#include <set>
#include <string>

using namespace seasocks;

namespace {

struct Handler : WebSocket::Handler {
    std::set<WebSocket*> _cons;

    void onConnect(WebSocket* con) override {
        _cons.insert(con);
        send(con->credentials()->username + " has joined");
    }
    void onDisconnect(WebSocket* con) override {
        _cons.erase(con);
        send(con->credentials()->username + " has left");
    }

    void onData(WebSocket* con, const char* data) override {
        send(con->credentials()->username + ": " + data);
    }

    void send(const std::string& msg) {
        for (auto* con : _cons) {
            con->send(msg);
        }
    }
};

struct MyAuthHandler : PageHandler {
    std::shared_ptr<Response> handle(const Request& request) override {
        request.credentials()->username = formatAddress(request.getRemoteAddress());
        return Response::unhandled(); // cause next handler to run
    }
};

}

int main(int /*argc*/, const char* /*argv*/[]) {
    Server server(std::make_shared<PrintfLogger>());
    server.addPageHandler(std::make_shared<MyAuthHandler>());
    server.addWebSocketHandler("/chat", std::make_shared<Handler>());
    server.serve("src/ws_chatroom_web", 9000);
    return 0;
}
