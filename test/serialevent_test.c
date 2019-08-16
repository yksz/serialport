#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serialevent.h"

static char s_line[8192];

static void onReceived(SerialPort* serial)
{
    char rbuf[2] = {0};
    char wbuf[] = "Hello\n";
    int nbytes;

    nbytes = SerialPort_read(serial, rbuf, sizeof(rbuf) - 1);
    if (nbytes < 1) {
        return;
    }
    strcat(s_line, rbuf);
    if (rbuf[0] == '\n') {
        printf("r: %s", s_line);
        memset(s_line, 0, sizeof(s_line));

        nbytes = SerialPort_write(serial, wbuf, sizeof(wbuf) - 1);
        printf("w: %d\n", nbytes);
    }
}

int main(int argc, char** argv)
{
    char* portname;
    int baudrate;
    SerialPort serial = {0};

    if (argc <= 2) {
        printf("usage: %s <portname> <baudrate>\n", argv[0]);
        return 1;
    }
    portname = argv[1];
    baudrate = atoi(argv[2]);

    serialevent_set(onReceived);
    if (SerialPort_open(&serial, portname, baudrate) != 0) {
        perror("Could not open");
        return 1;
    }
    serialevent_add(&serial);
    serialevent_start();
    SerialPort_close(&serial);
    return 0;
}
