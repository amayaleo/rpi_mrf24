#pragma once 

namespace DATA{

    typedef struct MacAdress
    {
        //uint8_t ignore[3];

        uint8_t ignore;
    }MACADDRESS;


    typedef struct Buffer
    {
    MACADDRESS ignore;   
        uint64_t mac;
        uint8_t head;
        uint16_t size;
        uint8_t data[107];

    }BUFFER;


}