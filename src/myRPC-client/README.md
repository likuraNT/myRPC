# myRPC-client

## How 2 install

1. GOTO ../libmysyslog and then check README.md
2. Building :
   ```sh
   gcc myRPC-client.c -o myRPC-client -lmysyslog
   ```

## How 2 use

```sh
./myRPC-client -c "command" -h host_IP -p port [--stream|--dgram]
```

### Params:
- `-c, --command` - to give an instruction to server.
- `-h, --host` - to set host-IP.
- `-p, --port` - to set host-port.
- `-s, --stream` - to use TCP.
- `-d, --dgram` - to use UDP.
- `--help` - shows help-menu
