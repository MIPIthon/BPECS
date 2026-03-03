/****************************************************************************** 
 * File:                | hw_driver.c 
 * Author:              | Michael P.
 * Revision:            | A2
 * Revision date:       | 2026-02-09
 * ****************************************************************************  
 * Revision history:
 * ****************************************************************************
 * Rev. | Date          | Comments
 * A    | 2025-11-03    | Initial draft
 * A1   | 2025-11-24    | Added enable, disable comparator function
 * A2   | 2026-02-09    | Changed location of BOR settings from init_clock()
 *                      | to init_bor()
 ******************************************************************************/

/*------ INCLUDES ------------------------------------------------------------*/
#include "hw_driver.h"


/*------ FUNCTIONS -----------------------------------------------------------*/


/****************************************************************************** 
 * Initialization
 ******************************************************************************/

void init_hw(void) {
    init_clock();
    init_bor();
    init_pins();
    init_comparator();
    init_dac();
    init_timer0();
    init_timer1_gate_single_pulse_toggle();
    init_timer2();
}

void init_clock(void) {
    /* Clock determined by FOSC<1:0> */
    OSCCONbits.SCS = 0U;
    
    /* 16 MHz internal HF oscillator - DON'T CARE*/
    OSCCONbits.IRCF = 15U;
    
    /* PLL OFF */
    OSCCONbits.SPLLEN = 0U;
    
    /* Oscillator calibration */
    OSCTUNEbits.TUN = 0U;
 }

void init_bor(void) {
    /* Software Brown-out Reset Enable bit
     * If BOREN<1:0> in Configuration Words = 01:
     * 1 = BOR is enabled
     * 0 = BOR is disabled
     * If BOREN<1:0> in Configuration Words != 01:
     * SBOREN is read/write, but has no effect on the BOR */
    BORCONbits.SBOREN = 1U;
    
    /* Brown-out Reset Fast Start bit
     * If BOREN <1:0> = 10 (Disabled in Sleep) 
     * or BOREN<1:0> = 01 (Under software control):
     * 1 = Band gap is forced on always (covers Sleep/wake-up/operating cases)
     * 0 = Band gap operates normally and may turn off
     * If BOREN<1:0> = 11 (Always On) or BOREN<1:0> = 00 (Always Off):
     * BORFS is read/write, but has no effect on the BOR */
    BORCONbits.BORFS = 1U;
}

void init_pins(void) {
    /* LATx registers (output latch) */
    /* 0 => output driven to low */
    /* 1 => output driven to high */
    /* Only, if configured as digital output*/
    LATAbits.LATA0 = 0U;
    LATAbits.LATA1 = 0U;
    LATAbits.LATA2 = 0U;
    LATAbits.LATA4 = 0U;
    LATAbits.LATA5 = 0U;
    
    /* TRISx registers (data direction) */
    TRISAbits.TRISA0 = 0U; /* Output */ 
    TRISAbits.TRISA1 = 1U; /* Input */
    TRISAbits.TRISA2 = 0U; /* Output */
    TRISAbits.TRISA3 = 1U; /* Input */
    TRISAbits.TRISA4 = 1U; /* Input */
    TRISAbits.TRISA5 = 1U; /* Input */
    
    /* ANSELx registers (analog select) */
    ANSELAbits.ANSA0 = 0U; /* Digital I/O */
    ANSELAbits.ANSA1 = 0U; /* Digital I/O */
    ANSELAbits.ANSA2 = 0U; /* Digital I/O */
    ANSELAbits.ANSA4 = 0U; /* Digital I/O */

    /* Weak pull-ups are enabled by individual WPUx latch values */
    OPTION_REGbits.nWPUEN = 0U;
    
    /* WPUx registers (weak pull-ups) */
    WPUAbits.WPUA0 = 0U; /* Pull-up disabled */
    WPUAbits.WPUA1 = 0U; /* Pull-up disabled */ 
    WPUAbits.WPUA2 = 0U; /* Pull-up disabled */
    WPUAbits.WPUA3 = 0U; /* Pull-up disabled */
    WPUAbits.WPUA4 = 0U; /* Pull-up disabled */
    WPUAbits.WPUA5 = 0U; /* Pull-up disabled */

    /* ODx registers (Open-Drain / Push-Pull) */
    ODCONAbits.ODA0 = 0U; /* Push-pull */
    ODCONAbits.ODA1 = 0U; /* Push-pull */
    ODCONAbits.ODA2 = 0U; /* Push-pull */
    ODCONAbits.ODA4 = 0U; /* Push-pull */
    ODCONAbits.ODA5 = 0U; /* Push-pull */
   
    /* SLRCONx registers (Slew rate control) */
    SLRCONAbits.SLRA0 = 0U; /* Maximum slew rate */
    SLRCONAbits.SLRA1 = 0U; /* Maximum slew rate */
    SLRCONAbits.SLRA2 = 0U; /* Maximum slew rate */
    SLRCONAbits.SLRA4 = 0U; /* Maximum slew rate */
    SLRCONAbits.SLRA5 = 0U; /* Maximum slew rate */
    
    /* INLVLx registers (ST/TTL input) */
    INLVLAbits.INLVLA0 = 1U; /* ST input */
    INLVLAbits.INLVLA1 = 1U; /* ST input */
    INLVLAbits.INLVLA2 = 1U; /* ST input */
    INLVLAbits.INLVLA3 = 1U; /* ST input */
    INLVLAbits.INLVLA4 = 1U; /* ST input */
    INLVLAbits.INLVLA5 = 1U; /* ST input */

    /* APFCON registers (alternate pin function) */
    APFCONbits.RXDTSEL = 0U;    /* RX/DT function is on RA1 */
    APFCONbits.CWGASEL = 0U;    /* CWGOUTA function is on RA2 */
    APFCONbits.CWGBSEL = 0U;    /* CWGOUTB funcion is on RA0 */
    APFCONbits.T1GSEL = 0U;     /* T1G function is on RA4 */
    APFCONbits.TXCKSEL = 0U;    /* TX/CK function is on RA0 */
    APFCONbits.P2SEL = 0U;      /* PWM2 function is on RA0 */
    APFCONbits.P1SEL = 0U;      /* PWM1 function is on RA1 */
    
    /* IOCx registers (interrupt-on-change) */ 
    
    /* IOCAP: INTERRUPT-ON-CHANGE PORTA POSITIVE EDGE REGISTER */
    IOCAPbits.IOCAP0 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCAPbits.IOCAP1 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCAPbits.IOCAP2 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCAPbits.IOCAP3 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCAPbits.IOCAP4 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCAPbits.IOCAP5 = 0U; /* interrupt-On-Change is disabled for the associated pin */
            
    /* IOCAN: INTERRUPT-ON-CHANGE PORTA NEGATIVE EDGE REGISTER */
    IOCANbits.IOCAN0 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCANbits.IOCAN1 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCANbits.IOCAN2 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCANbits.IOCAN3 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCANbits.IOCAN4 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCANbits.IOCAN5 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    
    /* IOCAF: INTERRUPT-ON-CHANGE PORTA FLAG REGISTER */
    IOCAFbits.IOCAF0 = 0U;
    IOCAFbits.IOCAF1 = 0U;
    IOCAFbits.IOCAF2 = 0U;
    IOCAFbits.IOCAF3 = 0U;
    IOCAFbits.IOCAF4 = 0U;
    IOCAFbits.IOCAF5 = 0U;
}

void init_comparator() {
    
    /* CHYS enabled; 
     * CON enabled; 
     * CPOL not inverted; 
     * CSYNC asynchronous; 
     * CSP hi_speed; 
     * COE COUT_pin; */ 
    CM1CON0bits.C1HYS = 1U;
    CM1CON0bits.C1ON = 1U;
    CM1CON0bits.C1POL = 0U;
    CM1CON0bits.C1SYNC = 0U;
    CM1CON0bits.C1SP = 1U;
    CM1CON0bits.C1OE = 1U;
    CM1CON0bits.C1OUT = 0U;
    
    /* CINTN no_intFlag; 
     * CINTP no_intFlag; 
     * CNCH CIN1-; 
     * CPCH DAC; */
    CM1CON1bits.C1INTN = 0U;
    CM1CON1bits.C1INTP = 0U;
    CM1CON1bits.C1NCH = 1U;
    CM1CON1bits.C1PCH = 1U; 
}

void init_dac(void) 
{
    /* DACPSS VDD; 
     * DACEN enabled; 
     * DACOE disabled; 
     */
    DACCON0bits.DACPSS = 0U;
    DACCON0bits.DACEN = 1U;
    DACCON0bits.DACOE = 0U;
    
    /* DACR 16; Vp = 5V; Vn = 0V */ 
    /* DACx_output = (Vp-Vn)*(DACR/(2^5))+Vn
     * DACx_output = Vp*(DACR/(2^5))
     * DACx_output = 2.5V
     */
    
    DACCON1 =  0x10;
}

void init_timer0(void) {
    /* Timer0 Clock Source Select Bit - Transition on T0CKI pin */
    OPTION_REGbits.TMR0CS = 1U;
    /* Timer0 Source Edge Select Bit - Increment on low-to-high transition on T0CKI pin */
    OPTION_REGbits.TMR0SE = 0U;
    /* Prescaler is assigned to the Timer0 module */
    OPTION_REGbits.PSA = 0U; 
    /* Prescaler is NOT assigned to the Timer0 module */
    //OPTION_REGbits.PSA = 1U;  
    /* Prescaler Rate Select bits */
    OPTION_REGbits.PS = T0_PRES_1_4;
}

void init_timer1_gate_single_pulse_toggle(void) {
    /* Timer1 clock source is the instruction clock (FOSC/4) */
    T1CONbits.TMR1CS = 0U;
    /* Timer1 input clock prescale select bits 1:1 */
    T1CONbits.T1CKPS = 0U;
    /* Synchronizes the asynchronous clock input with the system clock (FOSC) */
    T1CONbits.nT1SYNC = 0U;
    
    /* Stops Timer1 and  clears Timer1 gate flip-flop */
    T1CONbits.TMR1ON = 0U;
    
    /* Timer1 Gate Enable bit - controlled by T1 gate function */
    T1GCONbits.TMR1GE = 1U;
    /* Timer1 Gate Polarity bit - gate is high */
    T1GCONbits.T1GPOL = 1U;
    /* Timer1 Gate Toggle Mode bit - enabled */
    T1GCONbits.T1GTM = 1U;
    /* Timer1 Gate Single-Pulse Mode bit - enabled */
    T1GCONbits.T1GSPM = 1U;
    /* Timer1 Gate Single-Pulse Acquisition Status bit */
    T1GCONbits.T1GGO = 0U;
    /* Timer1 Gate Source Select bits - Timer0 overflow output (T0_overflow) */
    T1GCONbits.T1GSS = 1U; 
}

void init_timer1(void) {
    /* Timer1 clock source is the T1CLK1 pin (on the rising edge) */
    T1CONbits.TMR1CS = 2U;
    /* 1:1 prescale value */
    T1CONbits.T1CKPS = 0U;
    /* Synchronizes the asynchronous clock input with the system clock (FOSC) */
    T1CONbits.nT1SYNC = 0U;
    /* Stops Timer1 and  clears Timer1 gate flip-flop */
    T1CONbits.TMR1ON = 0U;
    
    /* Timer1 gate single-pulse acquisition has completed or has not been started */
    T1GCONbits.T1GGO = 0U;
    /* Timer1 gate is active-low (Timer1 counts when gate is low) */
    T1GCONbits.T1GPOL = 0U;
    /* Timer1 gate toggle mode is disabled and toggle flip-flop is cleared */ 
    T1GCONbits.T1GTM = 0U;
    /* Timer1 gate pin (T1G) */
    T1GCONbits.T1GSS = 0U;
    /* Timer1 counts regardless of Timer1 gate function */
    T1GCONbits.TMR1GE = 0U;
}

void init_timer2(void) {
    /* FOSC/4 = 4 MHz => T = 250 ns 
     * after prescaler T = 16 us
     * and after postscaler T = 160us 
     * using PR2 time adjustable between 0 and 40800 us */
    
    T2CONbits.T2OUTPS = T2_POSTS_1_10;
    T2CONbits.T2CKPS =  T2_PRES_1_64;
        
}

/****************************************************************************** 
 * Comparator
 ******************************************************************************/

void enable_comparator(void) 
{
    CM1CON0bits.C1ON = 1U;
}

void disable_comparator(void) 
{
    CM1CON0bits.C1ON = 0U;
}

uint8_t get_comparator_state(void) {
    return CMOUTbits.MC1OUT;
}

/****************************************************************************** 
 * DAC
 ******************************************************************************/

void set_dac(uint8_t inputData) {
    DACCON1 =  inputData;
}

uint8_t get_dac(void) {
    return DACCON1;
}

/****************************************************************************** 
 * Timer0
 ******************************************************************************/

uint8_t get_timer0_value(void) {
    return TMR0;
}

void set_timer0_value(uint8_t value) {
    TMR0 = value;
}

void clear_timer0_overflow(void) {
    INTCONbits.TMR0IF = 0U;
}

uint8_t get_timer0_overflow(void) {
    return (INTCONbits.TMR0IF);
}

void set_timer0_overflow(void) {
    INTCONbits.TMR0IF = 0U;
}

/****************************************************************************** 
 * Timer1
 ******************************************************************************/

void start_timer1(void) {
    T1CONbits.TMR1ON = 1U;
}

void stop_timer1(void) {
    T1CONbits.TMR1ON = 0U;
}

void set_timer1_value(uint16_t value) {
    if (1U == T1CONbits.nT1SYNC) {
	    uint8_t state = T1CONbits.TMR1ON;	
		
        T1CONbits.TMR1ON = 0U;
        
        TMR1H = (uint8_t)(value >> 8U);
        TMR1L = (uint8_t)value;

        T1CONbits.TMR1ON = state;
    }
    else
    {
        TMR1H = (uint8_t)(value >> 8U);
        TMR1L = (uint8_t)value;
    }
}

uint8_t get_timer1_single_pulse_mode_status() {
    return T1GCONbits.T1GGO_nDONE;
}


void start_timer1_single_pulse_mode() {
    T1GCONbits.T1GGO_nDONE = 1U;
}

uint16_t get_timer1_value(void) {
    uint16_t value;
    uint8_t high_byte;
    uint8_t low_byte;
    
    low_byte = TMR1L;
    high_byte = TMR1H;    
    value = ((uint16_t)high_byte << 8U) | low_byte;

    return value;
}

void clear_timer1_overflow(void) {
    PIR1bits.TMR1IF = 0U;
}

uint8_t get_timer1_overflow(void) {
    return (PIR1bits.TMR1IF);
}

/****************************************************************************** 
 * Timer2
 ******************************************************************************/

void start_timer2(void) {
    T2CONbits.TMR2ON = 1U;
}

void stop_timer2(void) {
    T2CONbits.TMR2ON = 0U;
}

void clear_timer2_overflow(void) {
    PIR1bits.TMR2IF = 0U;
}

uint8_t get_timer2_overflow(void) {
    return (PIR1bits.TMR2IF);
}

void set_timer2_tmr(uint8_t value) {
    TMR2 = value;
}

void set_timer2_pr(uint8_t value) {
    PR2 = value;
}