#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <cstdint>
#include <cstring>
#include <unistd.h>
#include <numeric>

struct modbus_rtu_t {
    uint8_t                 id = 0;
    uint8_t                 type_function;
    uint8_t                 reg_num;
    uint8_t                 size_data;
    std::vector<uint8_t>    bytes_;
};

class Modbus {
public:
    std::vector<uint8_t> bytes;
    modbus_rtu_t data_;
    Modbus() = delete;
    explicit Modbus(uint8_t request_id, uint8_t type_function, uint8_t reg_num, uint8_t reg_count);
    explicit Modbus(uint8_t request_id, uint8_t type_function, uint8_t reg_num, uint8_t reg_count, const std::vector<uint8_t>& data);
    explicit Modbus(std::vector<uint8_t> bytes_);

    uint8_t GetCRC(const std::vector<uint8_t>& bites) const;
    bool CheckCRC(const std::vector<uint8_t>& bites) const;
};
