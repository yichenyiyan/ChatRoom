#include <stdio.h>

#include <iostream>

#include "chat_server.h"


int main(void) {
    chatServer* server = new chatServer();
    server->main_loop();
    delete server;
    return 0;
}