#pragma once

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0
#define INFO 1
#define WARN 2
#define ERROR 3
#define CRITICAL 4 

int mysyslog(const char*, int, int, int, const char*);
