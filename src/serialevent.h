#ifndef SERIALEVENT_H
#define SERIALEVENT_H

#include "serialport.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void (*SerialDataReceivedHandler)(SerialPort* serial);

/**
 * Start the event loop to receive bytes from serial ports.
 */
int serialevent_start(void);

/**
 * Set the event handler called when data is received from a serial port.
 *
 * @param[in] handler
 */
void serialevent_set(SerialDataReceivedHandler handler);

/**
 * Add a serial port for watching whether data is arrived.
 *
 * @param[in] serial
 */
int serialevent_add(SerialPort* serial);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* SERIALEVENT_H */
