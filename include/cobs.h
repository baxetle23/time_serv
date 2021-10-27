#pragma once
#include <vector>
#include <cstring>
#include <cstdint>
#include <unistd.h>
#include <algorithm>

#define SIZE_BUFFER 1024
#define FIND_START 0
#define FIND_END 1

class Modbus;

class COBS {
private:
    std::vector<uint8_t> remainder_;
    uint8_t state = FIND_START;
    int fd_write;
    int fd_read;

    uint8_t Decode(std::vector<uint8_t>::iterator start, std::vector<uint8_t>::iterator end, std::vector<uint8_t> &decodedData);
    void Encode(std::vector<uint8_t> &rawData);
public:
    COBS() = delete;
    explicit COBS(int fd_write, int fd_read);
    uint8_t SendEncodeModBusFrame(Modbus&& request);
    Modbus GetDecodeModBusFrame();
};
