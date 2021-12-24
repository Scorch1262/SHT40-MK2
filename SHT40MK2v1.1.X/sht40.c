#define FCY 16000000UL

#include "xc.h"
#include <assert.h>
#include <stdbool.h>
#include <libpic30.h>
#include <stdint.h>
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/i2c1.h"
#include "sht40.h"

#if defined(__dsPIC33E__)
	#include <p33exxxx.h>
#elif defined(__dsPIC33F__)
	#include <p33fxxxx.h>
#elif defined(__dsPIC30F__)
	#include <p30fxxxx.h>
#elif defined(__PIC24E__)
	#include <p24exxxx.h>
#elif defined(__PIC24H__)
	#include <p24hxxxx.h>
#elif defined(__PIC24F__) || defined(__PIC24FK__)
	#include <p24fxxxx.h>
#endif

#define SHT40_ADDR    0x44

I2C1_MESSAGE_STATUS status;                                             // 

uint8_t Slave_Address;                                                  // definiert "Slave_Address" 

uint8_t  writeBuffer[3] = {0x00, 0x00, 0x00};                           // definiert "writeBuffer"
uint8_t  readBuffer[10] = {0x00, 0x00, 0x00};                            // definiert "readBuffer" 
uint16_t retryTimeOut1;                                                 // definiert "retryTimeOut1" 
uint16_t retryTimeOut2;                                                 // definiert "retryTimeOut2" 

void SHT40_Init(void){                                                  // "SHT40_Init"
    Slave_Address = SHT40_ADDR;                                         // schreibt "SHT40_ADDR" in "Slave_Address"    
  
    writeBuffer[0] = 0xFD;                                              // schreibt "0xFD" in "writeBuffer[0]"
    I2C1_MasterWrite(&writeBuffer[0], 1, Slave_Address, &status);       // schreibt 1 Byte (writeBuffer[0]) an I2C1
    i2c1_message_pending_100();                                         // ruft "i2c1_message_pending_100" auf 
    __delay_ms(10);                                                     // warte 10ms
}                                                                       // 

void SHT40_Read(void){                                                  // "SHT40_Read"
    Slave_Address = SHT40_ADDR;                                         // schreibt "SHT40_ADDR" in "Slave_Address"    
  
    writeBuffer[0] = 0xFD;                                              // schreibt "0xFD" in "writeBuffer[0]"
    I2C1_MasterWrite(&writeBuffer[0], 1, Slave_Address, &status);       // schreibt 1 Byte (writeBuffer[0]) an I2C1
    i2c1_message_pending_100();                                         // ruft "i2c1_message_pending_100" auf   
    __delay_ms(10);                                                     // warte 10ms
    I2C1_MasterRead(&readBuffer[0], 6, Slave_Address, &status);         // liest von I2C1 in 6 Byte (readBuffer[0]) 
    i2c1_message_pending_100();                                         // ruft "i2c1_message_pending_100" auf
}                                                                       // 


float SHT40_temp_GetValue(void){                                        // "SHT40_temp_GetValue"  
    float    tempDataAnz;                                               // definiert "tempDataAnz"
    uint16_t buffer;                                                    // definiert "buffer"
    
    SHT40_Read();                                                       // ruft "SHT40_Read" auf
    buffer = readBuffer[0] * 256 + readBuffer[1];                       // 
    tempDataAnz = buffer;                                               // schreibt inhalt von "buffer" in "tempDataAnz"
    tempDataAnz = -45 + 175 * tempDataAnz/65535;                        // 
    
    return tempDataAnz;                                                 // gibt "tempDataAnz" zurück     
}                                                                       // 

float SHT40_humy_GetValue(void){                                        // "SHT40_humy_GetValue" 
    float    pressDataAnz;                                              // definiert "pressDataAnz"
    uint16_t buffer;                                                    // definiert "buffer"
    
    SHT40_Read();                                                       // ruft "SHT40_Read" auf
    buffer = readBuffer[3] * 256 + readBuffer[4];                       // 
    pressDataAnz = buffer;                                              // schreibt inhalt von "buffer" in "pressDataAnz"
    pressDataAnz = -6 + 125 * pressDataAnz/65535;                       // 
    
    if(pressDataAnz > 100){                                             // wenn "pressDataAnz" größer als "100" ist
        pressDataAnz = 100;                                             // "pressDataAnz" gleich 100
    }                                                                   // 
    if (pressDataAnz < 0){                                              // wenn "pressDataAnz" kleiner als "100" ist
        pressDataAnz = 0;                                               // "pressDataAnz" gleich 0
    }                                                                   // 
    
    return pressDataAnz;                                                // gibt "pressDataAnz" zurück     
}                                                                       // 

void i2c1_message_pending_100(void){                                    // "i2c1_message_pending_100"
    retryTimeOut1 = 0;                                                  // setzt "retryTimeOut1" auf 0
    retryTimeOut2 = 0;                                                  // setzt "retryTimeOut2" auf 0
    while(status == I2C1_MESSAGE_PENDING){                              // solange "status" gleich "I2C1_MESSAGE_PENDING" ist
        if (retryTimeOut2 == 1000){                                      // wenn "retryTimeOut2" gleich 100 ist
            break;                                                      // Abbruch der Schleife
        }else{                                                          // sonst
            retryTimeOut2++;                                            // "retryTimeOut2" +1
        }                                                               // 
    }
    while(status != I2C1_MESSAGE_FAIL){                                 // solange "status" nicht gleich "I2C1_MESSAGE_FAIL" ist
        if (retryTimeOut1 == 1000){                                      // wenn "retryTimeOut1" gleich 100 ist
            break;                                                      // Abbruch der Schleife
        }else{                                                          // sonst
            retryTimeOut1++;                                            // "retryTimeOut1" +1
        }                                                               // 
    }                                                                   // 
}                                                                       // 