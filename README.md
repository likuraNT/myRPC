<h1> Механизм удаленного вызова команд myRPC

Использует сокеты в качестве транспорта.

```
make all || deb || clean
```
then use:

```
sudo dpkg -i deb/myrpc-client_1.0-1_amd64.deb 
sudo dpkg -i deb/myrpc-server_1.0-1_amd64.deb
sudo dpkg -i deb/libmysyslog_1.0-1_amd64.deb
```

and then:

```
sudo myrpc-server
myrpc-client -h 127.0.0.1 -p 25565 -s -c "date"
```
make all - собирает программу, и завершает работу makefile
make deb - собирает программу, и создает деб-пакеты
make clean - удаляет собранные файлы

myrpc-client [OPTIONS], чтобы увидеть - 
```
myrpc-client
```