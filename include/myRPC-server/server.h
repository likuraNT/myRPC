#pragma once

#include <pwd.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../libmysyslog/libmysyslog.h"

#define BUFFER_SIZE 4095

typedef struct {
    int port;
    char socket_type[16];
} config;

void handle_signal(int);

int allowed_user(const char*);

void command_execute(const char*, char*, char*);

config parser(const char*);