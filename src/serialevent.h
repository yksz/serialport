#ifndef SERIALEVENT_H
#define SERIALEVENT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "serialport.h"

typedef void (*SerialDataReceivedHandler)(SerialPort* serial);

int  serialevent_start(void);
void serialevent_set(SerialDataReceivedHandler handler);
int  serialevent_add(SerialPort* serial);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* SERIALEVENT_H */
