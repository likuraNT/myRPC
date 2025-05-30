#include "../../include/myRPC-client/myRPC-client.h"

void show_help();

int main(int argc, char* argv[]) 
{
    FILE* out = stdout;
    FILE* err = stderr;
    char* command = NULL; 
    char* host_ip = NULL;
    char request[BUFFER_SIZE + 1]; 
    int based_socket = 1; // using based stream socket
    int option_ind = 0;
    int sockfd = 0;
    int result = 0;
    int port = 25565;

    const char* short_options = "c:h:p:sd";

    const struct option long_options[] = 
    {
        {"command",     required_argument,  NULL, 'c'},
        {"host",        required_argument,  NULL, 'h'},
        {"port",        required_argument,  NULL, 'p'},
        {"stream",      no_argument,        NULL, 's'},
        {"dgram",       no_argument,        NULL, 'd'},
        {"help",        no_argument,        NULL,   0},
        {NULL,          0,                  NULL,   0}
    };

    const struct passwd* pw = getpwuid(getuid());
    char* username = pw->pw_name;

    struct sockaddr_in sAddr;
    memset(&sAddr, 0, sizeof(sAddr)); 

    while((result = getopt_long(argc, argv, short_options, long_options, &option_ind))!=-1)
    {
        switch(result)
        {
            case 'c':
                command = optarg;
                break;
            case 'h':
                host_ip = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 's':
                break; // now we're using stream socket by default
            case 'd':
                based_socket = 0; //choosing d-gram socket
                break;
            default:
                show_help();
                break;
        }
    }

    if ((!command || !host_ip || !port) && argc >= 1) 
    {
        fprintf(err, "You should use required args\n");
        show_help();
        return 1;
    }

    snprintf(request, BUFFER_SIZE + 1, "%s: %s", username, command);

    mysyslog("Trying to connect server.", INFO, 0, 0, "/var/log/myrpc.log");


    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(port);


    if (based_socket) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            mysyslog("Failed to creating socket", ERROR, 0, 0, "var/log/myrpc.log");
            fprintf(err, "Failed to creating socket");
            return 1;
        } else {
            if (connect(sockfd, (struct sockaddr*)&sAddr, sizeof(sAddr) < 0)) //casting sAddr and connecting to server
            {
                mysyslog("Failed to connect", ERROR, 0, 0, "/var/log/myrpc.log");
                fprintf(err, "Failed to connect");
                close(sockfd);
                return 1;
            }
            
            mysyslog("Successfully connected", INFO, 0, 0, "/var/log/myrpc.log");

            send(sockfd, request, strlen(request), 0);

            char response[BUFFER_SIZE + 1];
            int rv = recv(sockfd, response, BUFFER_SIZE + 1, 0);
            response[rv] = '\0';
            fprintf(out, "Response is: %s\n", response);

            mysyslog("Received server response", INFO, 0, 0, "/var/log/myrpc.log");
        }
    } else {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            mysyslog("Failed to creating socket", ERROR, 0, 0, "var/log/myrpc.log");
            fprintf(err, "Failed to creating socket");
            return 1;
        } else {
            sendto(sockfd, request, strlen(request), 0, (struct sockaddr*)&sAddr, sizeof(sAddr));
        
            socklen_t len = sizeof(sAddr);
            char response[BUFFER_SIZE + 1];
            int rv = recvfrom(sockfd, response, BUFFER_SIZE + 1, 0, (struct sockaddr*)&sAddr, &len);
            response[rv] = '\0';
            fprintf(out, "Response is: %s\n", response);

            mysyslog("Received server response", INFO, 0, 0, "/var/log/myrpc.log");
        }        
    }
    close(sockfd);
    return 0;
}

void show_help() 
{
    printf("Usage: ./myRPC-client [options]\n");
    printf("Options:\n");
    printf("    -c, --command \"bash-command\"  Executing command\n");
    printf("    -h, --host \"ipv4\"             Host IP address\n");
    printf("    -p, --port PORT                 Host port\n");
    printf("    -s, --stream                    Use stream socket\n");
    printf("    -d, --dgram                     Use datagram socket\n");
    printf("        --help                      Display this again\n");
}