#pragma once
#include <vector>
#include <cstdint>

#define MODBUS_FUNC_READ  0x2
#define MODBUS_FUNC_WRITE 0xf

extern std::vector<uint8_t> registr_file;