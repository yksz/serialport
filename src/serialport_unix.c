#include "serialport.h"
#include <assert.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

enum {
    kDefaultBaudRate = B9600
};

static void setBaudRate(struct termios* io, unsigned int baudRate)
{
    speed_t speed;
    switch (baudRate) {
        case 110:
            speed = B110;
            break;
        case 300:
            speed = B300;
            break;
        case 600:
            speed = B600;
            break;
        case 1200:
            speed = B1200;
            break;
        case 2400:
            speed = B2400;
            break;
        case 4800:
            speed = B4800;
            break;
        case 9600:
            speed = B9600;
            break;
        case 19200:
            speed = B19200;
            break;
        case 38400:
            speed = B38400;
            break;
        case 57600:
            speed = B57600;
            break;
        case 115200:
            speed = B115200;
            break;
        case 230400:
            speed = B230400;
            break;
        default:
            assert(0 && "Unsupported baudRate");
            speed = kDefaultBaudRate;
            break;
    }
    cfsetispeed(io, speed);
    cfsetospeed(io, speed);
}

int SerialPort_open(SerialPort* serial, const char* portName, unsigned int baudRate)
{
    assert(serial != NULL);
    assert(portName != NULL);

    serial->fd = open(portName, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (serial->fd < 0) {
        return -1;
    }

    serial->io.c_iflag = 0;
    serial->io.c_oflag = 0;
    serial->io.c_cflag = CS8 | CLOCAL | CREAD;
    serial->io.c_lflag = 0;
    serial->io.c_cc[VMIN] = 0;
    serial->io.c_cc[VTIME] = 0;
    setBaudRate(&serial->io, baudRate);
    tcsetattr(serial->fd, TCSAFLUSH, &serial->io);
    return 0;
}

int SerialPort_close(SerialPort* serial)
{
    assert(serial != NULL);

    return close(serial->fd);
}

int SerialPort_read(SerialPort* serial, char* buf, size_t len)
{
    assert(serial != NULL);
    assert(buf != NULL);

    return read(serial->fd, buf, len);
}

int SerialPort_write(SerialPort* serial, const char* buf, size_t len)
{
    assert(serial != NULL);
    assert(buf != NULL);

    return write(serial->fd, buf, len);
}

int SerialPort_flush(SerialPort* serial)
{
    assert(serial != NULL);

    return tcflush(serial->fd, TCOFLUSH);
}
