#include "app/analog_temp.h"
#include "heartbeat.h"
#include "intrinsics.h"
#include "keypad.h"
#include "RGB.h"
#include "i2c_master.h"
#include "analog_temp.h"
#include <msp430fr2355.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer
    i2c_master_init();
    analog_temp_init();

    // Disable low-power mode / GPIO high-impedance
    PM5CTL0 &= ~LOCKLPM5;
    heartbeat_init();
    keypad_init();
    // i2c_master_transmit(0x68, "hi");
    char previous = " ";
    char keypressed = " ";
    int already_unlocked = 0;
    // float temp = 0.0;
    char holding [5];
    char* temp_string = "hello";
    while(1) {
        // Main loop
        // if (keypressed != previous && keypressed != 0 && already_unlocked && keypad_is_unlocked()){
        //     switch(keypressed){
        //         case '1':
        //             i2c_master_transmit(0x40, "1", 1);
        //             // i2c_master_transmit(0x42, "1");
        //             break;
        //         case '2':
        //             i2c_master_transmit(0x40, "2", 1);
        //             // i2c_master_transmit(0x42, "2");
        //             break;
        //         case '3':
        //             i2c_master_transmit(0x40, "3", 1);
        //             // i2c_master_transmit(0x42, "3");
        //             break;
        //         case 'A':
        //             i2c_master_transmit(0x40, "A", 1);
        //             // i2c_master_transmit(0x42, "A");
        //             break;
        //         case '4':
        //             i2c_master_transmit(0x40, "4", 1);
        //             // i2c_master_transmit(0x42, "4");
        //             break;
        //         case '5':
        //             i2c_master_transmit(0x40, "5", 1);
        //             // i2c_master_transmit(0x42, "5");
        //             break;
        //         case '6':
        //             i2c_master_transmit(0x40, "6", 1);
        //             // i2c_master_transmit(0x42, "6");
        //             break;
        //         case 'B':
        //             i2c_master_transmit(0x40, "B", 1);
        //             // i2c_master_transmit(0x42, "B");
        //             break;
        //         case '7':
        //             i2c_master_transmit(0x40, "7", 1);
        //             // i2c_master_transmit(0x42, "7");
        //             break;
        //         case '8':
        //             i2c_master_transmit(0x40, "8", 1);
        //             // i2c_master_transmit(0x42, "8");
        //             break;
        //         case '9':
        //             i2c_master_transmit(0x40, "9", 1);
        //             // i2c_master_transmit(0x42, "9");
        //             break;
        //         case 'C':
        //             i2c_master_transmit(0x40, "C", 1);
        //             // i2c_master_transmit(0x42, "C");
        //             break;
        //         case '*':
        //             i2c_master_transmit(0x40, "*", 1);
        //             // i2c_master_transmit(0x42, "*");
        //             break;
        //         case '0':
        //             i2c_master_transmit(0x40, "0", 1);
        //             // i2c_master_transmit(0x42, "0");
        //             break;
        //         case '#':
        //             i2c_master_transmit(0x40, "#", 1);
        //             // i2c_master_transmit(0x42, "#");
        //             break;
        //         case 'D':
        //             i2c_master_transmit(0x40, "D", 1);
        //             // i2c_master_transmit(0x42, "D");
        //             break;
        //     }
        // } else if (keypad_is_unlocked()){
        //     already_unlocked = 1;
        // } else if (!keypad_is_unlocked() && keypressed=='D'){
        //     i2c_master_transmit(0x40, "D", 1);
        //     // i2c_master_transmit(0x42, "D");
        // }else {
        //     already_unlocked = 0;
        // }
        // keypressed = keypad_scan();
        
        i2c_master_transmit(0x40, "0", 1);
        float temp = 10;
        temp = analog_temp_get_temp();
        temp = (temp*9/5)+32;
        itoa((int)temp, holding, 10);
        i2c_master_transmit(0x40, holding, 2);
        i2c_master_transmit(0x40, ".", 1);
        temp = 100*(temp-(int)temp);
        itoa((int)temp, holding, 10);
        i2c_master_transmit(0x40, holding, 2);
        __delay_cycles(1000000);
    }
}

void itoa(int value, char* result, int base)
	{
	  // check that the base if valid
	  if (base < 2 || base > 36) { *result = '\0';}

	  char* ptr = result, *ptr1 = result, tmp_char;
	  int tmp_value;

	  do {
	    tmp_value = value;
	    value /= base;
	    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	  } while ( value );

	  // Apply negative sign
	  if (tmp_value < 0) *ptr++ = '-';
	  *ptr-- = '\0';
	  while(ptr1 < ptr) {
	    tmp_char = *ptr;
	    *ptr--= *ptr1;
	    *ptr1++ = tmp_char;
	  }

	}