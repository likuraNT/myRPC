# How 2 use myRPC

## Description

`myRPC` It is a remote command execution mechanism consisting of client and server applications. It includes the following components:
    **myRPC-client** — a console application for sending commands to the server.
    **myRPC-server** — a server application that receives and executes commands.

## Required elements

- GCC compiler
- installed `make`
- deb-packet support (`dpkg-deb`)

## Building

To build `myRPC-client` and `myRPC-server`, use the Makefile, which automates the compilation and creation of Debian packages.
```sh
make all
```
If you want to create deb-packets and also build two previous components - use:
```sh
make deb
```
To clean:
```sh
make clean
```


## Installing and configuring

After building and making deb-packets you can also install client and server by using `dpkg`:

```sh
sudo dpkg -i deb/myrpc-client_1.0-1_amd64.deb
sudo dpkg -i deb/myrpc-server_1.0-1_amd64.deb
sudo dpkg -i deb/libmysyslog_1.0-1_amd64.deb
```

### Server configuration

1. You should create a file  `/etc/myRPC/myRPC.conf` to configure server settings, e.g.:
```conf
sudo mkdir -p /etc/myRPC
echo -e "port=25565\nsocket_type=stream" | sudo tee /etc/myRPC/myRPC.conf
```

2. Create users-file `/etc/myRPC/users.conf`, which also known as "white list":
```conf
echo "username" | sudo tee /etc/myRPC/users.conf
```

### myRPC-client

Client application `myRPC-client` allows sending commands to server for execution, e.g.:

1. TCP - socket
```sh
myrpc-client -h 127.0.0.1 -p 25565 -s -c "date"
```
2. UDP-socket
```sh
myrpc-client -h 127.0.0.1 -p 25565 -d -c "date"
```

### myrpc-server

`myrpc-server` starting from server and listening to clients requests. To use:

```sh
sudo myrpc-server
```

## Logging and debugging

Check `/var/log/myrpc.log`
