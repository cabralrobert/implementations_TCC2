/*******************************************************************************
 * Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman
 *
 * Permission is hereby granted, free of charge, to anyone
 * obtaining a copy of this document and accompanying files,
 * to do whatever they want with them without any restriction,
 * including, but not limited to, copying, modification and redistribution.
 * NO WARRANTY OF ANY KIND IS PROVIDED.
 *
 * This example sends a valid LoRaWAN packet with payload "Hello, world!", that
 * will be processed by The Things Network server.
 *
 * Note: LoRaWAN per sub-band duty-cycle limitation is enforced (1% in g1, 
*  0.1% in g2). 
 *
 * Change DEVADDR to a unique address! 
 * See http://thethingsnetwork.org/wiki/AddressSpace
 *
 * Do not forget to define the radio type correctly in config.h, default is:
 *   #define CFG_sx1272_radio 1
 * for SX1272 and RFM92, but change to:
 *   #define CFG_sx1276_radio 1
 * for SX1276 and RFM95.
 *
 *******************************************************************************/

#include <mbed.h>
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

#if 1
#define SINGLE_CHANNEL_GATEWAY //force it to use 902.3 MHz only
#define TRANSMIT_INTERVAL 10 //in seconds, TOO OFTEN MAY GET YOUR TRAFFIC IGNORED
#include "test_node_secrets.h"
static const u1_t APPEUI[8]  = MY_APPEUI;
static const u1_t DEVEUI[8]  = MY_DEVEUI;
static const u1_t DEVKEY[16] = MY_NWKSKEY;
static const u1_t ARTKEY[16] = MY_APPSKEY;
static const u4_t DEVADDR = MY_DEVADDR;
#else
#define TRANSMIT_INTERVAL 120
#warning YOU SHOULD SET CREATE AN APP AND A NODE IDENTITY (use a personal node)
#warning SEE http://staging.thethingsnetwork.org/wiki/Backend/ttnctl/QuickStart
// LoRaWAN Application identifier (AppEUI)
// Not used in this example
static const u1_t APPEUI[8]  = { 0x02, 0x00, 0x00, 0x00, 0x00, 0xEE, 0xFF, 0xC0 };

// LoRaWAN DevEUI, unique device ID (LSBF)
// Not used in this example
static const u1_t DEVEUI[8]  = { 0x42, 0x42, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };

// LoRaWAN NwkSKey, network session key 
// Use this key for The Things Network
static const u1_t DEVKEY[16] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };

// LoRaWAN AppSKey, application session key
// Use this key to get your data decrypted by The Things Network
static const u1_t ARTKEY[16] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };

// LoRaWAN end-device address (DevAddr)
// See http://thethingsnetwork.org/wiki/AddressSpace
#warning FIXME You MUST SET A CUSTOM ADDRESS
static const u4_t DEVADDR = 0x03FF0001 ; // <-- Change this address for every node!

#endif

//////////////////////////////////////////////////
// APPLICATION CALLBACKS
//////////////////////////////////////////////////

// provide application router ID (8 bytes, LSBF)
void os_getArtEui (u1_t* buf) {
    memcpy(buf, APPEUI, 8);
}

// provide device ID (8 bytes, LSBF)
void os_getDevEui (u1_t* buf) {
    memcpy(buf, DEVEUI, 8);
}

// provide device key (16 bytes)
void os_getDevKey (u1_t* buf) {
    memcpy(buf, DEVKEY, 16);
}

uint8_t mydata[] = "Hello";
static osjob_t sendjob;

// Pin mapping
//lmic_pinmap pins; 
/*= {
  .nss = 10,
  .rxtx = 7, // Not connected on RFM92/RFM95
  .rst = 9,  // Needed on RFM92/RFM95
  .dio = {2, 5, 6},
};
*/
void onEvent (ev_t ev) {
    //debug_event(ev);

    switch(ev) {
      case EV_TXCOMPLETE:
          if(LMIC.dataLen) { // data received in rx slot after tx
              printf("Data Received!\n\r");
          }
          break;
       default:
          break;
    }
}

unsigned int xmit_count = 0;
void do_send(osjob_t* j){
  printf("\rTime: %lu\n", (unsigned int) (millis() / 1000));
        printf("\rSend, txCnhl: %u Opmode check: ", LMIC.txChnl);
    if (LMIC.opmode & (1 << 7)) {
        printf("\rOP_TXRXPEND, not sending\n\r");
    } else {
      printf("ok\n\r");
      // Prepare upstream data transmission at the next possible time.
//      LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
      char buf[32];
      LMIC.frame[0] = 'R';
      LMIC.frame[1] = 'o';
      LMIC.frame[2] = 'b';
      LMIC.frame[3] = 'e';
      LMIC.frame[4] = 'r';
      LMIC.frame[5] = 't';
//      sprintf(buf, "", xmit_count++);
      LMIC_clrTxData();
      LMIC_setTxData2(1, LMIC.frame, 6, 0);
    //  printf("sending %s\n", buf);
    }
    // Schedule a timed job to run at the given timestamp (absolute system time)
    os_setTimedCallback(j, os_getTime()+sec2osticks(TRANSMIT_INTERVAL), do_send);
         
}

void setup() {
  printf("Starting, will wait 1 second\n\r");
  while (millis() < 1000);
  printf("setting up\n\r");
  fprintf(stderr, "stderr test\n\r");
  os_init();
  LMIC_reset();
  LMIC_setSession (0x1, DEVADDR, (uint8_t*)DEVKEY, (uint8_t*)ARTKEY);
  LMIC_setAdrMode(0);
  LMIC_setLinkCheckMode(0);
  LMIC_disableTracking ();
  LMIC_stopPingable();
  LMIC_setDrTxpow(DR_SF7,14);
    
  fflush(stdout);
  #ifdef SINGLE_CHANNEL_GATEWAY
  //Serial.println("Disabling all channels but 0 (902.3 MHz) for single-channel gateway compatibility\r");
  printf("\rDisabling all channels but 0 (902.3 MHz) for single-channel gateway compatibility\n\r");
  //int cha = 4;
  for (int i=1; i<64; i++){
      //if(cha != i)
          LMIC_disableChannel(i);
  }
  #endif
}

void loop() {

do_send(&sendjob);

while(1) {
  os_runloop_once();
  }
  }
  
int main(int argc, char **argv) {
    setup();
    while(1) loop();
    }
 
