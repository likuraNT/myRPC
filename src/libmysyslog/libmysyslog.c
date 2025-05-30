#include "../../include/libmysyslog/libmysyslog.h"

int mysyslog(const char* msg, int level, int driver, int format, const char* path)
{
    FILE* logF = fopen(path, "a");
    if (logF == NULL) return -1;

    time_t cur;
    time(&cur);

    char* timestmp = ctime(&cur);
    timestmp[strlen(timestmp) - 1] = '\0';
    const char* slevel = NULL;

    switch (level)
    {
        case DEBUG:
            slevel = "DEBUG";
            break;
        case INFO:
            slevel = "INFO";
            break;
        case WARN:
            slevel = "WARN";
            break;
        case ERROR:
            slevel = "ERROR";
            break;
        case CRITICAL:
            slevel = "CRITICAL";
            break;
        default:
            slevel = "UNDEFINED";
            break;
    }

    if (format == 0) { //txt formatting
        fprintf(logF, "%s %s %d %s\n", timestmp, slevel, driver, msg);
    } else {
        fprintf(logF, "{\"timestamp\":\"%s\",\"logging_level\":\"%s\",\"driver\":%d,\"message\":\"%s\"}\n", timestmp, slevel, driver, msg);
    }

    fclose(logF);
    return 0;
}