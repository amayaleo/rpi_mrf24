#pragma once 

namespace DATA{
#pragma pack(push, 1)
    typedef struct MacAdress
    {
        //uint8_t ignore[3];

        uint8_t ignore;
    }MACADDRESS;


    typedef struct Buffer
    {
        uint64_t mac;
        uint8_t ignore[4];
        uint8_t head;
        uint16_t size;
        uint8_t data[107];
        
        

    }BUFFER;

#pragma pack(pop)
}