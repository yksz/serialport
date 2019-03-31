#include "serialport.h"
#include <assert.h>
#include <stdio.h>

enum {
    kDefaultBaudRate = CBR_9600
};

static void setBaudRate(DCB* dcb, unsigned int baudRate)
{
    DWORD speed;
    switch (baudRate) {
        case 110:
            speed = CBR_110;
            break;
        case 300:
            speed = CBR_300;
            break;
        case 600:
            speed = CBR_600;
            break;
        case 1200:
            speed = CBR_1200;
            break;
        case 2400:
            speed = CBR_2400;
            break;
        case 4800:
            speed = CBR_4800;
            break;
        case 9600:
            speed = CBR_9600;
            break;
        case 19200:
            speed = CBR_19200;
            break;
        case 38400:
            speed = CBR_38400;
            break;
        case 57600:
            speed = CBR_57600;
            break;
        case 115200:
            speed = CBR_115200;
            break;
        case 256000:
            speed = CBR_256000;
            break;
        default:
            assert(0 && "Unsupported baudRate");
            speed = kDefaultBaudRate;
            break;
    }
    dcb->BaudRate = speed;
}

int SerialPort_open(SerialPort* serial, const char* portName, unsigned int baudRate)
{
    char newPortName[16] = {0};
    DCB dcb = {0};

    assert(serial != NULL);
    assert(portName != NULL);

    sprintf_s(newPortName, sizeof(16), "\\\\.\\%s", portName);
    serial->fd = CreateFile(newPortName, GENERIC_READ | GENERIC_WRITE,
            0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (serial->fd == INVALID_HANDLE_VALUE) {
        return -1;
    }
    memset(&serial->overlapped, 0, sizeof(OVERLAPPED));
    GetCommState(serial->fd, &dcb);
    setBaudRate(&dcb, baudRate);
    return !SetCommState(serial->fd, &dcb);
}

int SerialPort_close(SerialPort* serial)
{
    assert(serial != NULL);

    return !CloseHandle(serial->fd);
}

int SerialPort_read(SerialPort* serial, char* buf, size_t len)
{
    int nbytes;
    BOOL ok;

    assert(serial != NULL);
    assert(buf != NULL);

    ok = ReadFile(serial->fd, buf, len, &nbytes, &serial->overlapped);
    if (!ok && GetLastError() != ERROR_IO_PENDING) {
        return -1;
    }
    return nbytes;
}

int SerialPort_write(SerialPort* serial, const char* buf, size_t len)
{
    int nbytes;
    BOOL ok;

    assert(serial != NULL);
    assert(buf != NULL);

    ok = WriteFile(serial->fd, buf, len, &nbytes, &serial->overlapped);
    if (!ok && GetLastError() != ERROR_IO_PENDING) {
        return -1;
    }
    return nbytes;
}

int SerialPort_flush(SerialPort* serial)
{
    assert(serial != NULL);

    return !FlushFileBuffers(serial->fd);
}
