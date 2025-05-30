#include "../../include/myRPC-server/server.h"

volatile sig_atomic_t stop_signal;

void handle_signal(int sig);

int allowed_user(const char* username);

void command_execute(const char* command, char* stdout_file, char* stderr_file);

config parser(const char* filename);

int main(void)
{
    FILE* err = stderr;

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    config cfg = parser("/etc/myRPC/myRPC.conf");


    int port = cfg.port;
    int based_socket = (strcmp(cfg.socket_type, "stream") == 0);

    mysyslog("Starting server", INFO, 0, 0, "/var/log/myrpc.log");

    int sockfd;
    if (based_socket) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
    } else {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    }

    if (sockfd < 0) {
        mysyslog("Failed to create socket", ERROR, 0, 0, "/var/log/myrpc.log");
        fprintf(err, "Failed to create socket");
        return 1;
    }

    int opt = 1;

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0) {
        mysyslog("Failed to setsockopt", ERROR, 0, 0, "/var/log/myrpc.log");
        fprintf(err, "Failed to setsockopt");
        close(sockfd);
        return 1;
    }

    struct sockaddr_in sAddr, cAddr;
    memset(&sAddr, 0, sizeof(sAddr));
    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = INADDR_ANY;
    sAddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&sAddr, sizeof(sAddr)) < 0) {
        mysyslog("Failed to bind", ERROR, 0, 0, "/var/log/myrpc.log");
        fprintf(err, "Failed to bind");
        close(sockfd);
        return 1;
    }
    if (based_socket) {
        listen(sockfd, 5);
        mysyslog("Server listening (stream)", INFO, 0, 0, "/var/log/myrpc.log");
    } else {
        mysyslog("Server listening (datagram)", INFO, 0, 0, "/var/log/myrpc.log");
    }

    while (!stop_signal) 
    {
        char buffer[BUFFER_SIZE + 1];
        int rv = 0;

        if (based_socket) {
            socklen_t len = sizeof(cAddr);
            int connfd = accept(sockfd, (struct sockaddr*)&cAddr, &len);
            if (connfd < 0) {
                mysyslog("Failed to accept", ERROR, 0, 0, "/var/log/myrpc.log");
                fprintf(err, "Failed to accept");
                continue;
            }

            rv = recv(connfd, buffer, BUFFER_SIZE + 1, 0);
            if (rv <= 0) {
                close(connfd);
                continue;
            }
            buffer[rv] = '\0';

            mysyslog("Received request", INFO, 0, 0, "/var/log/myrpc.log");

            // format: username:command
            char *username = strtok(buffer, ":");
            char *command = strtok(NULL, "");
            if (command) {
                while (*command == ' ')
                    ++command;
            }

            char response[BUFFER_SIZE + 1];

            if (allowed_user(username)) {
                mysyslog("Allowed user", INFO, 0, 0, "/var/log/myrpc.log");

                char stdout_file[] = "/tmp/myRPC_XXXXXX.stdout";
                char stderr_file[] = "/tmp/myRPC_XXXXXX.stderr";
                mkstemp(stdout_file);
                mkstemp(stderr_file);

                command_execute(command, stdout_file, stderr_file);

                FILE *file = fopen(stdout_file, "r");
                if (file) {
                    size_t read_bytes = fread(response, 1, BUFFER_SIZE + 1, file);
                    response[read_bytes] = '\0';
                    fclose(file);
                    mysyslog("Command executed successfully", INFO, 0, 0, "/var/log/myrpc.log");
                } else {
                    strcpy(response, "Error reading file");
                    mysyslog("Error reading file", ERROR, 0, 0, "/var/log/myrpc.log");
                }
                remove(stdout_file);
                remove(stderr_file);

            } else {
                snprintf(response, BUFFER_SIZE + 1, "User '%s' isn't allowed", username);
                mysyslog("User not allowed", WARN, 0, 0, "/var/log/myrpc.log");
            }

            send(connfd, response, strlen(response), 0);
            close(connfd);
        } else {
            socklen_t len = sizeof(cAddr);
            rv = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&cAddr, &len);
            buffer[rv] = '\0';

            mysyslog("Received request", INFO, 0, 0, "/var/log/myrpc.log");

            // format: username:command
            char *username = strtok(buffer, ":");
            char *command = strtok(NULL, "");
            if (command) {
                while (*command == ' ')
                    ++command;
            }

            char response[BUFFER_SIZE + 1];
            if (allowed_user(username)) {
                mysyslog("Allowed user", INFO, 0, 0, "/var/log/myrpc.log");

                char stdout_file[] = "/tmp/myRPC_XXXXXX.stdout";
                char stderr_file[] = "/tmp/myRPC_XXXXXX.stderr";
                mkstemp(stdout_file);
                mkstemp(stderr_file);

                command_execute(command, stdout_file, stderr_file);

                FILE *file = fopen(stdout_file, "r");
                if (file) {
                    size_t read_bytes = fread(response, 1, BUFFER_SIZE + 1, file);
                    response[read_bytes] = '\0';
                    fclose(file);
                    mysyslog("Command executed successfully", INFO, 0, 0, "/var/log/myrpc.log");
                } else {
                    strcpy(response, "Error reading file");
                    mysyslog("Error reading file", ERROR, 0, 0, "/var/log/myrpc.log");
                }
                remove(stdout_file);
                remove(stderr_file);

            } else {
                snprintf(response, BUFFER_SIZE + 1, "User '%s' isn't allowed", username);
                mysyslog("User not allowed", WARN, 0, 0, "/var/log/myrpc.log");
            }
            sendto(sockfd, response, strlen(response), 0, (struct sockaddr*)&cAddr, sizeof(cAddr));
        }
    }
    close(sockfd);
    mysyslog("Server stopped", INFO, 0, 0, "/var/log/myrpc.log");
    return 0;
}

void handle_signal(int sig) {stop_signal = sig; }

int allowed_user(const char* username) 
{
    FILE* file = fopen("/etc/myRPC/users.conf", "r");
    if (!file) 
    {
        mysyslog("Failed to open users cfg", ERROR, 0, 0, "/var/log/myrpc.log");
        fprintf(stderr, "Failed to open users cfg");
        return 1;
    }
    char line[256];
    int allwd = 0;

    while(fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0';
        
        if (line[0] == '#' || strlen(line) == 0) continue;

        if (strcmp(line, username) == 0) 
        {
            allwd = 1; 
            break;
        }
    }
    fclose(file);
    return allwd;
}

void command_execute(const char* command, char* stdout_file, char* stderr_file)
{
    char cmd[BUFFER_SIZE+1];
    snprintf(cmd, BUFFER_SIZE + 1, "%s >%s 2>%s", command, stdout_file, stderr_file);
    system(cmd);
}

config parser(const char* filename)
{
    config cfg;
    cfg.port = 25565;
    strcpy(cfg.socket_type, "stream");

    FILE* file = fopen(filename, "r");
    if (!file)
    {
        mysyslog("Failed to open cfg file", ERROR, 0, 0, "/var/log/myrpc.log");
        fprintf(stderr, "Failed to open cfg file");
        return cfg;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0';

        if (line[0] == '#' || strlen(line) == 0) continue;

        char* key = strtok(line, "=");
        char* value = strtok(NULL,"");

        if (strcmp(key, "port") == 0) {
            cfg.port = atoi(value);
        } else if (strcmp(key, "socket_type") == 0) {
            strcpy(cfg.socket_type, value);
        }
    }

    fclose(file);
    return cfg;
}