#ifndef SERIALPORT_H
#define SERIALPORT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stddef.h>
#if defined(_WIN32) || defined(_WIN64)
 #include <windows.h>
#else
 #include <termios.h>
#endif /* defined(_WIN32) || defined(_WIN64) */

typedef struct serialport {
#if defined(_WIN32) || defined(_WIN64)
    HANDLE fd;
    OVERLAPPED overlapped;
#else
    int fd;
    struct termios io;
#endif /* defined(_WIN32) || defined(_WIN64) */
} SerialPort;

/**
 * @param[in] serial
 * @param[in] portName
 * @param[in] baudRate
 * @return 0 on success or a non-zero value on error
 */
int SerialPort_open(SerialPort* serial, const char* portName, unsigned int baudRate);

/**
 * @param[in] serial
 * @return 0 on success or a non-zero value on error
 */
int SerialPort_close(SerialPort* serial);

/**
 * @param[in] serial
 * @param[in] buf
 * @param[in] len
 * @return the number of bytes read on success or -1 on error
 */
int SerialPort_read(SerialPort* serial, char* buf, size_t len);

/**
 * @param[in] serial
 * @param[in] buf
 * @param[in] len
 * @return the number of bytes write on success or -1 on error
 */
int SerialPort_write(SerialPort* serial, const char* buf, size_t len);

/**
 * @param[in] serial
 * @return 0 on success or a non-zero value on error
 */
int SerialPort_flush(SerialPort* serial);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* SERIALPORT_H */
