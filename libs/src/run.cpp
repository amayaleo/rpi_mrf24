
#include <run.h>
#include <mrf24j40.h>

namespace MRF24J40{ 

Mrf24j mrf24j40_spi ;

Run::Run() 
{
    std::cout<<"Run()\n";
    mrf24j40_spi.init();
    mrf24j40_spi.interrupt_handler();
    mrf24j40_spi.set_pan(0xcafe);
    mrf24j40_spi.address16_write(0x6001); 

  // uncomment if you want to receive any packet on this channel
  mrf24j40_spi.set_promiscuous(true);
  
  // uncomment if you want to enable PA/LNA external control
  //mrf24j40_spi.set_palna(true);
  
  // uncomment if you want to buffer all PHY Payload
  mrf24j40_spi.set_bufferPHY(true);

  //attachInterrupt(0, interrupt_routine, CHANGE); // interrupt 0 equivalent to pin 2(INT0) on ATmega8/168/328
  //last_time = millis();

    //mrf24j40_spi.Transfer3bytes(0xE0C1);

    loop();
}

void Run::loop() {

    mrf24j40_spi.check_flags(&handle_rx, &handle_tx);
    unsigned long current_time = 1000000;
    if (current_time - last_time > tx_interval) {
        last_time = current_time;
        std::cout<<"txxxing...\n";
        mrf24j40_spi.send16(0x1234, "@BTCH");//send data//original//mrf24j40_spi.send16(0x4202, "abcd")
    }
}

void Run::interrupt_routine() {
    mrf24j40_spi.interrupt_handler(); // mrf24 object interrupt routine
}


void handle_rx() {
    
    std::cout<<"received a packet ";
    std::cout<<std::dec << mrf24j40_spi.get_rxinfo()->frame_length;
    
    std::cout<<" bytes long";
    
    if(mrf24j40_spi.get_bufferPHY()){
      std::cout<<"Packet data (PHY Payload):";
      for (int i = 0; i < mrf24j40_spi.get_rxinfo()->frame_length; i++) {
          std::cout<<mrf24j40_spi.get_rxbuf()[i];
      }
    }
    
    std::cout<<"\r\nASCII data (relevant data):\n";
    auto data_length =mrf24j40_spi.rx_datalength();
    std::cout << "data_length : "<<std::dec<<data_length<<"\n";
    for (int i = 0; i < data_length; i++) {

        std::cout<<std::hex<<mrf24j40_spi.get_rxinfo()->rx_data[i];
        std::cout<<".";
    }
    
    std::cout<<"\r\nLQI/RSSI = \n";
    std::cout<<std::hex<<mrf24j40_spi.get_rxinfo()->lqi;

    std::cout<<"/";
    std::cout<<std::hex<<mrf24j40_spi.get_rxinfo()->rssi;
    
}

void handle_tx() {
         if (mrf24j40_spi.get_txinfo()->tx_ok) {
             std::cout<<"TX went ok, got ack \n";
         } else {
             std::cout<<"TX failed after \n";
             std::cout<<mrf24j40_spi.get_txinfo()->retries;
             std::cout<<" retries\n";
         }
    }

    Run::~Run() {
        std::cout<<"~Run()\n";
    }
}



