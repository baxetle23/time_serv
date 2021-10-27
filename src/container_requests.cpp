#include "../include/container_requests.h"

ContainerRequests::ContainerRequests(int fd_write, int fd_read) : cobs(fd_write, fd_read){
}

uint8_t ContainerRequests::FindResponce(uint8_t request_id, modbus_rtu_t& result) {
    if (container_.count(request_id)) {
        result = container_.at(request_id).data_;
        container_.erase(request_id);
        return 0;
    }
    return 1;
}

uint8_t ContainerRequests::AddRequest(Modbus&& modbus) {
    ++id_current_;
    return cobs.SendEncodeModBusFrame(std::move(modbus));
}

uint8_t ContainerRequests::GetIdRequest() const {
    return id_current_;
}

void ContainerRequests::GetResponces() {
    while(1) {
        Modbus&& modbus = cobs.GetDecodeModBusFrame();
        container_.insert(std::make_pair(modbus.data_.id, std::move(modbus)));
    }
}
