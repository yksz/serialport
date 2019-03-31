#include "serialevent.h"
#include <assert.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

enum {
    kSerialCapacity = 10
};

static struct {
    SerialPort ports[kSerialCapacity];
    int size;
    int maxfd;
} s_serials;

static SerialDataReceivedHandler s_onReceived;

int serialevent_start(void)
{
    fd_set readfds;
    int ready;
    int i;

    assert(s_serials.size != 0);

    if (s_serials.size == 0) {
        return -1;
    }

    while (1) {
        FD_ZERO(&readfds);
        for (i = 0; i < s_serials.size; i++) {
            FD_SET(s_serials.ports[i].fd, &readfds);
        }
        ready = select(s_serials.maxfd + 1, &readfds, NULL, NULL, NULL);
        if (ready == -1) {
            return -1;
        }
        for (i = 0; i < s_serials.size; i++) {
            if (FD_ISSET(s_serials.ports[i].fd, &readfds)) {
                s_onReceived(&s_serials.ports[i]);
            }
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
    int maxfd = 0;
    int fd;
    int i;

    assert(serial != NULL);
    assert(s_serials.size < kSerialCapacity);

    if (s_serials.size >= kSerialCapacity) {
        return -1;
    }

    s_serials.ports[s_serials.size] = *serial;
    s_serials.size++;

    for (i = 0; i < s_serials.size; i++) {
        fd = s_serials.ports[i].fd;
        if (fd > maxfd) {
            maxfd = fd;
        }
    }
    s_serials.maxfd = maxfd;
    return 0;
}
