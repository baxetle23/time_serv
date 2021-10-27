#pragma once
#include <unordered_map>
#include <memory>
#include <utility>
#include <cstdint>
#include "cobs.h"
#include "modbus.h"

class Modbus;

class ContainerRequests {
private:
   std::unordered_map<uint8_t, Modbus> container_;
   uint32_t id_current_ = 1;
   COBS cobs;

public:
   ContainerRequests() = delete;
   explicit ContainerRequests(int fd_write, int fd_read);
   uint8_t GetIdRequest() const;
   uint8_t FindResponce(uint8_t request_id, modbus_rtu_t& result);
   uint8_t AddRequest(Modbus&& modbus);
   void GetResponces();
};

