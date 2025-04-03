#include <msp430.h> 
//-------------------------------------------------------------------------------
// Matthew Phillips
// Prof. Cory Mettler
// EELE 371
// 20 March 2024
//
// Lab 15.1;
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//      AD2 Voltage
//-------------------------------------------------------------------------------
// 500 mV = 745;        745/(2^12)*3.3 = 0.600219727
// 1    V = 1377;       1277/(2^12)*3.3 = 1.028833008
// 2.3  V = 2978;       2978/(2^12)*3.3 = 2.399267578
// 3    V = 3877;       3877/(2^12)*3.3 = 3.12355957
//-------------------------------------------------------------------------------

float ADCvalue;

void analog_temp_init(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
    PM5CTL0 &= ~LOCKLPM5;       // TURN ON GPIO

    //-- SETUP TIMER --
    TB0CTL |= TBCLR;
    TB0CTL |= TBSSEL__SMCLK;     // Source SMCLK
    TB0CTL |= MC__UP;
    TB0CCR0 = 1000;
    TB0CCR1 = 250;

    //-- Configure ADC --
    ADCCTL0 &= ~ADCSHT;         // Clear ADCSHT from def. of ADSHT = 01
    ADCCTL0 |= ADCSHT_2;        // Conversion Cycles = 16 (ADCSHT = 10)
    ADCCTL0 |= ADCON;           // Turn ADC ON

    ADCCTL1 |= ADCSSEL_2;       // ADC Clock Source = SMCLK
    ADCCTL1 |= ADCSHP;          // Sample signal source = sampling timer

    ADCCTL2 &= ~ADCRES;         // Clear ADCRES from def. of ADCRES = 01
    ADCCTL2 |= ADCRES_2;        // Resolution 12 bit

    ADCMCTL0|= ADCINCH_2;       // ADC Input Chanel = A2 (P1.2)

    ADCIE |= ADCIE0;            // Enable ADC Conv Complete IRQ
    __enable_interrupt();       // Enable interrupts

	// while(1){
	//     ADCCTL0 |= ADCENC | ADCSC;
	//     __bis_SR_register(GIE | LPM0_bits);

	// }

	// return 0;
}

float analog_temp_get_temp(){
    return ADCvalue;
}

#pragma vector = ADC_VECTOR
__interrupt void ADC_ISR (void){
    __bic_SR_register_on_exit(LPM0_bits);   // Wake CPU
    ADCvalue = ADCMEM0;
}
// ----- END ADC ISR -----