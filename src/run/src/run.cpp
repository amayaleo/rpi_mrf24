#include <iostream>
#include <thread>
#include <vector>


#include <run/src/run.h>
#include <run/src/radio.h>
#include <others/src/msj.h>
#include <display/src/oled.h>


namespace RUN{

void Run_t::start()
{
        system("clear"); 
    
    //std::vector<std::thread> threadVect;
    

//std::cout << "\033[2J\033[H" << std::flush;
     try    {
// 
        // for (int i = 0; i < 4; ++i) {
            // threadVect.emplace_back([&mrf]() {
           // mrf->Run();
            // });
        //}
// 
        //Esperar a que todos los hilos terminen
        // for (auto& threadRun : threadVect) {
            // threadRun.join();
        // }    




            auto mrf { std::make_unique<MRF24J40::Radio_t>()};        // Inicializar hilos y ejecutar las clases en paralelo
            auto msj { std::make_unique<DEVICES::Msj_t>()};  
            auto display { std::make_unique<OLED::Oled_t>()};               
            //static auto oled { std::make_unique<OLED::Oled_t>() };    //inicializar una sola vez 
            std::thread thread1(&MRF24J40::Radio_t::Run, mrf.get());
            std::thread thread2(&DEVICES::Msj_t::Start, msj.get());
            std::thread thread3(&OLED::Oled_t::init, display.get());
               
            //Esperar a que todos los hilos terminen
            thread1.join();
            thread2.join();
            thread3.join();
            }
        catch(...){
                    std::cerr<<"\nerror :(\n";
        }
    }
}