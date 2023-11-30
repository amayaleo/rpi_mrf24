#include <iostream>
#include <fstream>

#include <iomanip>
#include <chrono>
#include <sstream>
extern "C"
{
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdint.h>
}
#include <file.h>
#include <config.h>
#include <color.h>


namespace FILESYSTEM{

    File_t::File_t()
    : m_filename {LOG_FILENAME} , m_buffer{"@ABCDEF"} , m_size_data{m_buffer.size() * sizeof(char)}
    {

    }

    File_t::~File_t(){

    }


    unsigned char* File_t::loadFile(const char* filename){

            std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return nullptr;
    }

    packet_mrf24 packet;
    
    file.read(reinterpret_cast<char*>(&packet), sizeof(packet_mrf24));

    const auto& address_rx = packet.mac_address_rx;

    if (address_rx != ADDRESS_LONG_SLAVE) {

        std::cerr << "Es un MRF24 no  válido." << std::endl;
        file.close();
        return nullptr;
    }

size_t dataSize = packet.size;

    unsigned char* imgdata = new unsigned char[dataSize];

    //file.seekg(packet.data, std::ios::binary);// or std::ios::beg 

    file.read(reinterpret_cast<char*>(imgdata), dataSize);

    if (packet.panid != PAN_ID) {

        std::cerr << SET_COLOR_RED_TEXT << "PAN_ID no  válido." << std::endl;
        file.close();
        return nullptr;
    }

        file.close();
    return imgdata;
    }


    bool File_t::create(const char* tmp){
        const std::string name_files = "log/" + m_filename + tyme() +  ".bin";
        std::ofstream file(name_files, std::ios::binary);
        if (file.is_open()) {

            const auto bufferSize = strlen(tmp);
            file.write ( tmp ,  bufferSize);
                // Cerrar el archivo
            file.close();

            std::cout << "\nData written to the file successfully." << std::endl;
                  return true;
        } else {
            std::cerr << "\nNot open file." << std::endl;
                  return false;
        }

        return false;
    }

    const std::string  File_t::tyme()
    {

        auto now = std::chrono::system_clock::now();

        // Convertir el tiempo actual a una estructura de tiempo local
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime = *std::localtime(&currentTime);

        // Formatear la fecha y hora según tu especificación
        std::ostringstream oss;
            std::cout<<"\n";
        oss << std::put_time(&localTime, "%Y%m%d%H%M%S");
            std::cout<<"\n";
        std::string tyme = oss.str();
        return tyme;
    }

}