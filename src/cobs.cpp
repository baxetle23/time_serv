#include "../include/cobs.h"
#include "../include/modbus.h"

COBS::COBS(int fd_write_, int fd_read_) : fd_write(fd_write_), fd_read(fd_read_) {
}

uint8_t COBS::Decode(std::vector<uint8_t>::iterator start,std::vector<uint8_t>::iterator end, std::vector<uint8_t> &decodedData) {
    decodedData.clear();
    std::vector<uint8_t> encodedData(start + 1, end);
    size_t encodedDataPos = 0;
    while (encodedDataPos < encodedData.size()) {
       size_t numElementsInBlock = encodedData[encodedDataPos] - 1;
       encodedDataPos++;
       if((encodedDataPos+numElementsInBlock) > encodedData.size()){
           return 1;
       }
       // Copy across all bytes within block
       for (size_t i = 0; i < numElementsInBlock; i++) {
           if (encodedDataPos == encodedData.size()) {
               // End of packet found!
               while((decodedData.back()!=0x0) && (decodedData.size())){
                   decodedData.pop_back();
               }
               break;
           }
           decodedData.push_back(encodedData[encodedDataPos]);
           encodedDataPos++;
       }
       if (encodedDataPos == encodedData.size()) {
           // End of packet found!
           break;
       }
       // We only add a 0x00 byte to the decoded data
       // IF the num. of elements in block was less than 254.
       // If num. elements in block is max (254), then we know that
       // the block was created due to it reaching maximum size, not because
       // a 0x00 was found
       if(numElementsInBlock < 0xFE) {
           decodedData.push_back(0x00);
       }
    }
    return 0;
}

void COBS::Encode(std::vector<uint8_t>& rawData) {
    rawData.insert(rawData.begin(), 0);
    auto zero = rawData.insert(rawData.begin() + 1, 0);
    auto it = rawData.begin() + 2;
    while (it != rawData.end()) {
        if (*it == 0x00) {
            *zero = it - zero;
            zero = it;
        } else {
            if (it - zero == 254) {
                *zero = 255;
                zero = rawData.insert(++it, 0);
            }
        }
        ++it;
    }
    *zero = it - zero;
    rawData.push_back(0);
}

uint8_t COBS::SendEncodeModBusFrame(Modbus&& request) {
    size_t bytes_to_write;
    Encode(request.bytes);
    bytes_to_write = write(fd_write, &(request.bytes[0]), request.bytes.size());
    #ifdef DEBUG
        long int ttime;
        ttime = time(NULL);
        std::cout << ctime(&ttime) << " cobs.cpp DATA WRITE TO PIPE pid = " << getpid() << " ; request id = " << (int)request.bytes[2] << " bytes: ";
        for(size_t i = 0; i <  request.bytes.size(); ++i) {
            std::cout << (int)request.bytes[i] << " ";
        }
        std::cout << std::endl;
    #endif
    if (bytes_to_write != request.bytes.size()) {
        perror("cobs.cpp SendEncodeModBusFrame (error write to pipe)");
        return 1;
    }
    return 0;
}

Modbus COBS::GetDecodeModBusFrame() {
    std::vector<uint8_t> decodedData;
    uint8_t buffer[SIZE_BUFFER];
    int byte_count;
    std::vector<uint8_t>::iterator it_start;
    std::vector<uint8_t>::iterator it_end;
    while (1) {    
        switch (state)
        {
            case FIND_START: {
                it_start = std::find(remainder_.begin(), remainder_.end(), 0x00);
                if (it_start != remainder_.end()) {
                    state = FIND_END;
                } else {
                    byte_count = read(fd_read, buffer, SIZE_BUFFER);
                    if (byte_count < 0) {
                        perror("cobs.cpp GetDecodeModBusFrame (error read to pipe)");
                    }
                    #ifdef DEBUG
                        long int ttime;
                        ttime = time(NULL);
                        std::cout << ctime(&ttime) << " cobs.cpp DATA READ1 TO PIPE pid = " << getpid() << " bytes: ";
                        for(int i = 0; i < byte_count; ++i) {
                            std::cout << (int)buffer[i] << " ";
                        }
                        std::cout << std::endl;
                    #endif
                    for(int i = 0; i < byte_count; ++i) {
                        remainder_.push_back(buffer[i]);
                    }
                }
                break;
            }
            case FIND_END: {
                it_end = std::find(it_start + 1, remainder_.end(), 0x00);
                if (it_end != remainder_.end()) {
                    Decode(it_start, it_end, decodedData);
                    state = FIND_START;
                    remainder_.erase(it_start, it_end + 1);
                    return Modbus{decodedData};
                } else {
                    byte_count = read(fd_read, buffer, SIZE_BUFFER);
                    if (byte_count < 0) {
                        perror("cobs.cpp GetDecodeModBusFrame (error read to pipe)");
                    }
                    #ifdef DEBUG
                        long int ttime;
                        ttime = time(NULL);
                        std::cout << ctime(&ttime) << "cobs.cpp DATA READ2 TO PIPE pid = " << getpid() << " bytes: ";
                        for(int i = 0; i < byte_count; ++i) {
                            std::cout << (int)buffer[i] << " ";
                        }
                        std::cout << std::endl;
                    #endif
                    for(int i = 0; i < byte_count; ++i) {
                        remainder_.push_back(buffer[i]);
                    }
                }
                break;
            }
        }
    }
}
