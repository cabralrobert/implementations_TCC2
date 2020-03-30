/*********************************************************************
 *
 * Copy this to "test_node_secrets.h" and fill your private details
 *
 * See 
 *
 * http://staging.thethingsnetwork.org/wiki/Backend/ttnctl/QuickStart
 *
 * In particular you need a local executable for ttnctl
 *
 * once you have that, ttnctl-HOST-CPU devices devices info DEVADDR  
 *
 * will give you the node detials to fill in below 
 *
 **********************************************************************/

// LoRaWAN Application identifier (AppEUI)
#define MY_APPEUI { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x02, 0x22, 0x3D }

// LoRaWAN DevEUI, unique device ID (LSBF)
#define MY_DEVEUI { 0x00, 0x15, 0xA6, 0x20, 0x86, 0x30, 0x7B, 0x09 }

//NwkSKey
#define MY_NWKSKEY { 0xDF, 0x7B, 0x0D, 0x26, 0x24, 0x4E, 0x0F, 0xAC, 0x8E, 0x28, 0x70, 0xE9, 0xD5, 0x89, 0xC6, 0x72 }
           
//AppSKey
#define MY_APPSKEY { 0x86, 0x49, 0xDF, 0x5A, 0xBD, 0xD6, 0x97, 0x88, 0x63, 0xF0, 0x0A, 0xA9, 0x67, 0x4F, 0x6D, 0x9F }

//put 32-bit hex value here as a single number, run together, no commans
#define MY_DEVADDR 0x26031D45 

