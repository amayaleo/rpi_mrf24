#pragma once
#include <memory>
#include <mrf24j40.h>

namespace MRF24J40{

    struct Run{
            explicit Run();
            ~Run();

            void loop();
            void interrupt_routine();
        private :
            std::unique_ptr<MRF24J40::Mrf24j> mrf24j40_spi {};
            unsigned long last_time{0};
            unsigned long tx_interval{0};
    };

            void handle_tx();
            void handle_rx();

}//end MRF24J40
