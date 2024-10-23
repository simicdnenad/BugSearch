#ifndef CONNSTATE_H
#define CONNSTATE_H

enum class EConnState :uint8_t {
    FILES_NOT_SELECTED = 0x00U,
    NOT_CONNECTED = 0x01U,
    CONNECTED = 0x02U,
    PROCESSING = 0x03U,
    FINISHED = 0x04U
};

#endif // CONNSTATE_H
