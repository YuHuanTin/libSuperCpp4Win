#include <iostream>
#include "enhance/buffer/Buffer.h"


int main() {

    uint8_t data[] = {0x4d, 0x53, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdd, 0x67, 0xc6, 0x64, 0x81, 0xd3, 0x87, 0x6e};
    Buffer p{data, 16};

    uint8_t data8[8]{0};
    p.read<8>(data8);

    auto timeStamp = p.read<uint32_t>();
    auto rand = p.read<uint32_t>();

    std::cout << "timeStamp: " << timeStamp << "\nrand: " << rand << '\n';
    std::cout << std::boolalpha << p.eof();


    return 0;
}