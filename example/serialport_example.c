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
    char buf[8192];
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
        memset(buf, 0, sizeof(buf));
        nbytes = SerialPort_read(&serial, buf, sizeof(buf) - 1);
        if (nbytes < 0) {
            perror("Could not read");
            break;
        }
        printf("%s", buf);
        Sleep(1000);
    }
    SerialPort_close(&serial);
    return 0;
}
