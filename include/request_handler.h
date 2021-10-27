#pragma once
#include <iostream>
#include <queue>
#include <algorithm>
#include "reg_file.h"
#include "modbus.h"
#include "log_duration.h"
#include "container_requests.h"
#include "fifo.h"

typedef uint8_t err_t;

class Modbus;
struct modbus_rtu_t;

class RequestHandlerMaster {
private:
    ContainerRequests container;
    err_t SendRequest(Modbus&& modbus_);
public:
    RequestHandlerMaster() = delete;
    explicit RequestHandlerMaster(int fd_write, int fd_read);
    err_t SendRequest(uint8_t type_function, uint8_t reg_num, uint8_t reg_count, uint8_t& responce_id);
    err_t SendRequest(uint8_t type_function, uint8_t reg_num, uint8_t reg_count, const std::vector<uint8_t>& data,  uint8_t& responce_id);
    err_t ResponceRequest(uint8_t request_id, modbus_rtu_t& responce);
    void WaitResponces();
};

class RequestHandlerSlave {
private:
    Fifo container_;
    uint8_t Execute(modbus_rtu_t& request);
    uint8_t SendResponce(Modbus&& modbus_);
public:
    RequestHandlerSlave() = delete;
    explicit RequestHandlerSlave(int fd_write, int fd_read);
    uint8_t ExecuteRequest();
    void WaitRequest();
};
