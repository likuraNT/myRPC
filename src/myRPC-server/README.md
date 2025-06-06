# myRPC Server  

A simple RPC server for executing commands on Unix systems with user authentication.  

## Features  

- Supports two operation modes: STREAM (TCP) and DGRAM (UDP)  
- User authentication via configuration file  
- Logs all events to the system log  
- Secure command execution with output isolation  

## Requirements  

- Linux/Unix system  
- C compiler (gcc/clang)  
- `libmysyslog` library for logging  

## Installation  

1. Copy server files to `/etc/myRPC/`  
2. Configure the files:  
   - `/etc/myRPC/myRPC.conf` – server settings  
   - `/etc/myRPC/users.conf` – list of authorized users  
3. Compile the program:  
   ```bash  
   gcc -o myrpc_server myrpc_server.c -I/path/to/headers -L/path/to/libs -lmysyslog
   ```
## Configuration

Example myRPC.conf:
```ini
port = 8080  
socket_type = stream  
```
Example users.conf:
```plaintext

# Authorized users  
root  
admin  
user1
```
