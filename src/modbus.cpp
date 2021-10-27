#include "../include/modbus.h"

Modbus::Modbus(uint8_t request_id, uint8_t type_function, uint8_t reg_num, uint8_t reg_count) :
    Modbus(request_id, type_function, reg_num, reg_count, {}){
}

Modbus::Modbus(std::vector<uint8_t> bytes) {
    if (CheckCRC(bytes)) {
        data_.id = bytes[0];
        data_.type_function = bytes[1];
        data_.reg_num = bytes[2];
        data_.size_data = bytes[3];
        if (data_.size_data) {
            for(size_t i = 0; i < data_.size_data; ++i) {
                data_.bytes_.push_back(bytes[4 + i]);
            }
        }
    }  else {
        perror(("modbus CRC don't check, pid = " + std::to_string(getpid())).data());
    }
 }


Modbus::Modbus(uint8_t request_id, uint8_t type_function, uint8_t reg_num, uint8_t reg_count, const std::vector<uint8_t>& data) {
    bytes.push_back(request_id);
    bytes.push_back(type_function); 
    bytes.push_back(reg_num); 
    bytes.push_back(reg_count);
    for(uint8_t byte : data) {
        bytes.push_back(byte);
    }
    bytes.push_back(GetCRC(bytes));
}

uint8_t Modbus::GetCRC(const std::vector<uint8_t>& bytes) const {
    return std::accumulate(bytes.begin(), bytes.end(), 0) % 256;
}

bool Modbus::CheckCRC(const std::vector<uint8_t>& bytes) const {
    return bytes.back() == std::accumulate(bytes.begin(), bytes.end() - 1, 0) % 256;
}
