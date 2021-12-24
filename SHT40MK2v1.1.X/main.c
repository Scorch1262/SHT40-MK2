#define FCY 16000000UL 
#include <assert.h>
#include <stdbool.h>
#include "mcc_generated_files/system.h"
#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>
#include <xc.h>
#include <string.h>
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

float temperatur;                                       // definiert "temperatur"
float lufthumy;                                         // defienert "lufthumy"
    
/*
                         Main application
 */
int main(void){                                         // "main"
    SYSTEM_Initialize();                                // ruft "SYSTEM_Initialize" auf
    __delay_ms(500);                                    // warte 500ms
    
    while (1){                                          // Endloschleife
        temperatur = SHT40_temp_GetValue();             // schreibt ergebnis von "SHT40_temp_GetValue" in "temperatur"
        lufthumy = SHT40_humy_GetValue();               // schreibt ergebnis von "SHT40_humy_GetValue" in "lufthumy"
        printf("Temperatur = %0.2fC   ",temperatur);    // schreibt "Temperatur = %.2fC" an UART  
        printf("Luftfeuchtigkeit = %4.2f%%\r\n",lufthumy);  // schreibt "Luftfeuchtigkeit = %4.2f%%\r\n" an UART  
        __delay_ms(500);                                // warte 500ms
    }                                                   // 
}                                                       // 
    
