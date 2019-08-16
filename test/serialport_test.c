#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32) || defined(_WIN64)
 #include <windows.h>
#else
 #include <unistd.h>
 #define Sleep(n) usleep((n) * 1000)
#endif /* defined(_WIN32) || defined(_WIN64) */
#include "serialport.h"

int main(int argc, char** argv)
{
    char* portname;
    int baudrate;
    SerialPort serial = {0};
    char rbuf[8192];
    char wbuf[] = "Hello\n";
    int nbytes;

    if (argc <= 2) {
        printf("usage: %s <portname> <baudrate>\n", argv[0]);
        return 1;
    }
    portname = argv[1];
    baudrate = atoi(argv[2]);

    if (SerialPort_open(&serial, portname, baudrate) != 0) {
        perror("Could not open");
        return 1;
    }
    while (1) {
        Sleep(1000);

        memset(rbuf, 0, sizeof(rbuf));
        nbytes = SerialPort_read(&serial, rbuf, sizeof(rbuf) - 1);
        if (nbytes < 0) {
            perror("Could not read");
            continue;
        }
        printf("r: %s", rbuf);

        nbytes = SerialPort_write(&serial, wbuf, sizeof(wbuf) - 1);
        if (nbytes < 0) {
            perror("Could not write");
            continue;
        }
        printf("w: %d\n", nbytes);
    }
    SerialPort_close(&serial);
    return 0;
}
