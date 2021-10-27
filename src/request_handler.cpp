#include "../include/request_handler.h"
#include "../include/container_requests.h"
#include "../include/modbus.h"


RequestHandlerMaster::RequestHandlerMaster(int fd_write, int fd_read) : container(fd_write, fd_read) {
}

err_t RequestHandlerMaster::SendRequest(uint8_t type_function, uint8_t reg_num, uint8_t reg_count, uint8_t& responce_id) {
    #ifdef TIME
        //LOG_DURATION("send request id " + std::to_string(responce_id));
    #endif
    responce_id = container.GetIdRequest();
    return SendRequest(Modbus{responce_id, type_function, reg_num, reg_count});
}

err_t RequestHandlerMaster::SendRequest(uint8_t type_function, uint8_t reg_num, uint8_t reg_count, const std::vector<uint8_t>& data, uint8_t& responce_id) {
    #ifdef TIME
        //LOG_DURATION("send request id " + std::to_string(responce_id));
    #endif
    responce_id = container.GetIdRequest();
    return SendRequest(Modbus{responce_id, type_function, reg_num, reg_count, data});
}

err_t RequestHandlerMaster::SendRequest(Modbus&& modbus_) {
   return container.AddRequest(std::move(modbus_));
}

err_t RequestHandlerMaster::ResponceRequest(uint8_t request_id, modbus_rtu_t &responce) {
    return container.FindResponce(request_id, responce);
}

void RequestHandlerMaster::WaitResponces() {
    container.GetResponces();
}

RequestHandlerSlave::RequestHandlerSlave(int fd_write, int fd_read) : container_(fd_write, fd_read) {
}

uint8_t RequestHandlerSlave::SendResponce(Modbus&& modbus) {
    return container_.SendResponce(std::move(modbus));
}
uint8_t RequestHandlerSlave::Execute(modbus_rtu_t& request) {
    if (request.type_function == MODBUS_FUNC_READ) {
        request.bytes_.clear();
        for (size_t i = request.reg_num, j = 0; j < request.size_data; ++j, ++i) {
            request.bytes_.push_back(registr_file[i]);
        }
        SendResponce(Modbus{request.id, request.type_function, request.reg_num, request.size_data, request.bytes_});
    } else if (request.type_function == MODBUS_FUNC_WRITE) {
        for (size_t i = request.reg_num, j = 0; j < request.size_data; ++j, ++i) {
            registr_file[i] = request.bytes_[j];
        }
    }
    container_.RemoveRequest();
    return 0;
}
uint8_t RequestHandlerSlave::ExecuteRequest() {
    if (container_.RequestAvailable()) {
        #ifdef TIME
            //LOG_DURATION(("slave executed id = " + std::to_string(request.front().data_.id) + " pid = " + std::to_string(getpid())));
        #endif
        Execute(container_.NextRequest());
        return 0;
    } else {
        return 1;
    }
}
void RequestHandlerSlave::WaitRequest() {
    container_.WaitRequest();
}
