#include "serialevent.h"
#include <assert.h>
#include <stdio.h>

enum {
    kSerialCapacity = 10
};

static struct {
    SerialPort ports[kSerialCapacity];
    HANDLE events[kSerialCapacity];
    int size;
} s_serials;

static SerialDataReceivedHandler s_onReceived;

int serialevent_start(void)
{
    DWORD eventmask;
    DWORD ready;
    int index;
    int i;

    assert(s_serials.size != 0);

    if (s_serials.size == 0) {
        return -1;
    }

    for (i = 0; i < s_serials.size; i++) {
        memset(&s_serials.ports[i].overlapped, 0, sizeof(OVERLAPPED));
        s_serials.ports[i].overlapped.hEvent = s_serials.events[i];
        SetCommMask(s_serials.ports[i].fd, EV_RXCHAR);
    }

    while (1) {
        for (i = 0; i < s_serials.size; i++) {
            WaitCommEvent(s_serials.ports[i].fd, &eventmask, &s_serials.ports[i].overlapped);
        }
        ready = WaitForMultipleObjects(s_serials.size, s_serials.events, FALSE, INFINITE);
        switch (ready) {
            case WAIT_TIMEOUT:
                continue;

            case WAIT_FAILED:
                return -1;
        }
        index = ready - WAIT_OBJECT_0;
        if (index < s_serials.size && eventmask & EV_RXCHAR) {
            s_onReceived(&s_serials.ports[index]);
        }
    }
    return 0;
}

void serialevent_set(SerialDataReceivedHandler handler)
{
    s_onReceived = handler;
}

int serialevent_add(SerialPort* serial)
{
    assert(serial != NULL);
    assert(s_serials.size < kSerialCapacity);

    if (s_serials.size >= kSerialCapacity) {
        return -1;
    }

    s_serials.ports[s_serials.size] = *serial;
    s_serials.events[s_serials.size] = CreateEvent(NULL, TRUE, FALSE, NULL);
    s_serials.size++;
    return 0;
}
