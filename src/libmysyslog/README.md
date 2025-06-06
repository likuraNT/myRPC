# libmysyslog

Library allows to work with syslog (dynamic lib `.so`).

## How 2 use
1. Clone repo:
```sh
git clone https://github.com/likuraNT/myRPC
```
2. Build it:
   ```sh
   sudo apt update
   sudo apt upgrade
   sudo apt-get install make
   make #(all - by default || deb || clean)
   ```

## Deb-packet
`.deb` packet have created in `../../deb/`:
```sh
sudo dpkg -i deb/libmysyslog_1.0-1_amd64.deb
```

## Using
```c
#include "libmysyslog.h"
```
Собирайте программу с линковкой библиотеки:
```sh
gcc main.c -o main -lmysyslog
```
## How 2 uninstall
```sh
sudo dpkg -r libmysyslog
```
