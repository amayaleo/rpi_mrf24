
#include <app/run.h>
#include <others/color.h>
#include <files/file.h>
#include <mrf24/mrf24j40.h>
#include <qr/qr.h>

namespace MRF24J40{ 

Mrf24j mrf24j40_spi ;

Run::Run() 
#ifdef ENABLE_INTERRUPT_MRF24
:   status          (true)
,   fs              { std::make_unique<FILESYSTEM::File_t>()}
    #ifdef ENABLE_DATABASE
,   database{std::make_unique<DATABASE::Database_t>()}
    #endif
#else
:   status          (false)
,   qr              { std::make_unique<QR::Qr_t>()}
#endif
,   gpio            {std::make_unique<GPIO::Gpio>(status)}
{
    #ifdef  DBG
        std::cout<<"Run()\n";
    #endif
    #ifdef ENABLE_INTERRUPT_MRF24
    
    #else
            qr->create(QR_CODE_SRT);
    #endif
            gpio->app();

    std::cout << "size msj : "<<std::dec<<sizeof(MSJ)<<"\n";
    mrf24j40_spi.init();
    mrf24j40_spi.interrupt_handler();
    mrf24j40_spi.set_pan(PAN_ID);
    // This is _our_ address
    #ifdef MACADDR16
        mrf24j40_spi.address16_write(ADDRESS); 
    #elif defined (MACADDR64)
        mrf24j40_spi.address64_write(ADDRESS_LONG);
    #endif

    // uncomment if you want to receive any packet on this channel
  //mrf24j40_spi.set_promiscuous(true);
  mrf24j40_spi.init_mrf();
  
    // uncomment if you want to enable PA/LNA external control
  mrf24j40_spi.set_palna(true);
  
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
    #ifdef MRF24_TRANSMITER_ENABLE
        std::cout<<"send16 () ... \n";
        #ifdef MACADDR64
            mrf24j40_spi.send64(ADDRESS_LONG_SLAVE, MSJ );
        #elif defined(MACADDR16)
            mrf24j40_spi.send16(ADDR_SLAVE, MSJ );//send data//original//mrf24j40_spi.send16(0x4202, "abcd")
        #endif
    #endif
    }
}

void Run::interrupt_routine() {
    mrf24j40_spi.interrupt_handler(); // mrf24 object interrupt routine
}

void handle_tx() {
    #ifdef MRF24_TRANSMITER_ENABLE
    const auto status = mrf24j40_spi.get_txinfo()->tx_ok;
         if (status) {
             std::cout<<"TX went ok, got ack \n";
         } else {
             std::cout<<"\nTX failed after \n";
             std::cout<<mrf24j40_spi.get_txinfo()->retries;
             std::cout<<" retries\n";
         }
    #endif     
    }

void handle_rx() {
    #ifdef MRF24_RECEIVER_ENABLE
    std::cout << " \nreceived a packet ... ";
    printf("0x%x\n",mrf24j40_spi.get_rxinfo()->frame_length);
    std::cout << " bytes long " ;
    
    if(mrf24j40_spi.get_bufferPHY()){
      std::cout << " Packet data (PHY Payload) :";
      for (int i = 0; i < mrf24j40_spi.get_rxinfo()->frame_length; i++) 
      {
          std::cout <<" "<<std::hex<< mrf24j40_spi.get_rxbuf()[i];
      }
    }
    SET_COLOR(SET_COLOR_CYAN_TEXT);
    std::cout<<"\r\nASCII data (relevant data) :\n";
    const int recevive_data_length = mrf24j40_spi.rx_datalength();
    std::cout << "\t\tdata_length : "<<std::dec<< recevive_data_length<<"\n\t";

    for (int i = 0; i < recevive_data_length; i++) {
        //std::cout<<std::hex<<mrf24j40_spi.get_rxinfo()->rx_data[i];
        printf("0x%x ",mrf24j40_spi.get_rxinfo()->rx_data[i]);
        
    }

    std::unique_ptr<FILESYSTEM::File_t> fs{std::make_unique<FILESYSTEM::File_t> ()};
    std::unique_ptr<QR::Qr_img_t> qr_img{std::make_unique<QR::Qr_img_t>()};

    fs->create(reinterpret_cast<const char *>(mrf24j40_spi.get_rxinfo()->rx_data));
    qr_img->create(reinterpret_cast<const char *>(mrf24j40_spi.get_rxinfo()->rx_data));

    SET_COLOR(SET_COLOR_GRAY_TEXT);
    SET_COLOR(SET_COLOR_BLUE_BACKGROUND);
   // printf("\nSIZEOF rx info : 0x%x\n ",sizeof(mrf24j40_spi.get_rxinfo()->rx_data));

    std::cout<<"\r\nLQI/RSSI = \n\t";
        //std::cout<<std::dec<<mrf24j40_spi.get_rxinfo()->lqi;
    printf("0x%X ",mrf24j40_spi.get_rxinfo()->lqi);
        //std::cout<<" ";
        //std::cout<<std::hex<<mrf24j40_spi.get_rxinfo()->rssi;
    printf("0x%X \n",mrf24j40_spi.get_rxinfo()->rssi);
    SET_COLOR(SET_COLOR_RED_TEXT);
    printf("LQI : %d ",mrf24j40_spi.get_rxinfo()->lqi);
    printf("RSSI : %d \n",mrf24j40_spi.get_rxinfo()->rssi);
    RST_COLOR() ;
    std::cout<<"\r\n";
    #endif
}


    Run::~Run() {
        #ifdef DBG
            std::cout<<"~Run()\n";
        #endif
    }
}


