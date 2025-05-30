#pragma once

#include <pwd.h>
#include <unistd.h>
#include <getopt.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../libmysyslog/libmysyslog.h"

#define BUFFER_SIZE 4095

void show_help();   