#ifndef CONNSTATE_H
#define CONNSTATE_H

enum class EConnState :uint8_t {
    NOT_CONNECTED = 0x00U,
    CONNECTED = 0x01U,
    PROCESSING = 0x02U,
    FINISHED = 0x03U,
};

#endif // CONNSTATE_H
