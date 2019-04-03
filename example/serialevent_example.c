#include <stdio.h>
#include <stdlib.h>
#include "serialevent.h"

static void onReceived(SerialPort* serial)
{
    char buf[8192] = {0};
    int nbytes;

    nbytes = SerialPort_read(serial, buf, sizeof(buf) - 1);
    if (nbytes > 0) {
        printf("%s", buf);
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
