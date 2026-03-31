/****************************************************************************** 
 * File:                | hw_driver.c 
 * Author:              | Michael P.
 * Revision:            | B1
 * Revision date:       | 2026-03-31
 * ****************************************************************************  
 * Revision history:
 * ****************************************************************************
 * Rev. | Date          | Comments
 * A    | 2025-11-03    | Initial draft
 * A1   | 2025-11-24    | Added enable, disable comparator function
 * A2   | 2026-02-09    | Changed location of BOR settings from init_clock()
 *                      | to init_bor()
 * B    | 2026-03-14    | HW driver modified due to change to PIC16F1764
 * B1   | 2026-03-31    | Issue, if T0 prescaler IS NOT activated
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
    init_comparator1();
    init_dac3();
    init_timer0();
    init_timer1_gate_single_pulse_toggle();
    /* init_timer2(); */
    /* __delay_us used instead of timer2 for timeout */
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
    
    LATCbits.LATC0 = 0U;
    LATCbits.LATC1 = 0U;
    LATCbits.LATC2 = 0U;
    LATCbits.LATC3 = 0U;
    LATCbits.LATC4 = 0U;
    LATCbits.LATC5 = 0U;
    
    /* TRISx registers (data direction) */
    TRISAbits.TRISA0 = 1U; /* Input */ 
    TRISAbits.TRISA1 = 1U; /* Input */
    TRISAbits.TRISA2 = 0U; /* Output */
    TRISAbits.TRISA4 = 0U; /* Output */
    
    TRISCbits.TRISC0 = 0U; /* Output */
    TRISCbits.TRISC1 = 1U; /* Input */
    TRISCbits.TRISC2 = 0U; /* Output */
    TRISCbits.TRISC3 = 0U; /* Output */
    TRISCbits.TRISC4 = 0U; /* Output */
    TRISCbits.TRISC5 = 0U; /* Output */
    
    /* ANSELx registers (analog select) */
    ANSELAbits.ANSA0 = 0U; /* Digital I/O */
    ANSELAbits.ANSA1 = 0U; /* Digital I/O */
    ANSELAbits.ANSA2 = 0U; /* Digital I/O */
    ANSELAbits.ANSA4 = 0U; /* Digital I/O */
    
    ANSELCbits.ANSC0 = 0U; /* Digital I/O */
    ANSELCbits.ANSC1 = 1U; /* Analog input */
    ANSELCbits.ANSC2 = 0U; /* Digital I/O */
    ANSELCbits.ANSC3 = 0U; /* Digital I/O */

    /* Weak pull-ups are enabled by individual WPUx latch values */
    OPTION_REGbits.nWPUEN = 0U;
    
    /* WPUx registers (weak pull-ups) */
    WPUAbits.WPUA0 = 0U; /* Pull-up disabled */
    WPUAbits.WPUA1 = 0U; /* Pull-up disabled */ 
    WPUAbits.WPUA2 = 0U; /* Pull-up disabled */
    WPUAbits.WPUA3 = 0U; /* Pull-up disabled */
    WPUAbits.WPUA4 = 0U; /* Pull-up disabled */
    WPUAbits.WPUA5 = 0U; /* Pull-up disabled */

    WPUCbits.WPUC0 = 0U; /* Pull-up disabled */
    WPUCbits.WPUC1 = 0U; /* Pull-up disabled */
    WPUCbits.WPUC2 = 0U; /* Pull-up disabled */
    WPUCbits.WPUC3 = 0U; /* Pull-up disabled */
    WPUCbits.WPUC4 = 0U; /* Pull-up disabled */
    WPUCbits.WPUC5 = 0U; /* Pull-up disabled */ 
            
    /* ODx registers (Open-Drain / Push-Pull) */
    ODCONAbits.ODA0 = 0U; /* Push-pull */
    ODCONAbits.ODA1 = 0U; /* Push-pull */
    ODCONAbits.ODA2 = 0U; /* Push-pull */
    ODCONAbits.ODA4 = 0U; /* Push-pull */
    ODCONAbits.ODA5 = 0U; /* Push-pull */
    
    ODCONCbits.ODC0 = 0; /* Push-pull */
    ODCONCbits.ODC1 = 0; /* Push-pull */
    ODCONCbits.ODC2 = 0; /* Push-pull */
    ODCONCbits.ODC3 = 0; /* Push-pull */
    ODCONCbits.ODC4 = 0; /* Push-pull */
    ODCONCbits.ODC5 = 0; /* Push-pull */
   
    /* SLRCONx registers (Slew rate control) */
    SLRCONAbits.SLRA0 = 0U; /* Maximum slew rate */
    SLRCONAbits.SLRA1 = 0U; /* Maximum slew rate */
    SLRCONAbits.SLRA2 = 0U; /* Maximum slew rate */
    SLRCONAbits.SLRA4 = 0U; /* Maximum slew rate */
    SLRCONAbits.SLRA5 = 0U; /* Maximum slew rate */
    
    SLRCONCbits.SLRC0 = 0U; /* Maximum slew rate */
    SLRCONCbits.SLRC1 = 0U; /* Maximum slew rate */
    SLRCONCbits.SLRC2 = 0U; /* Maximum slew rate */
    SLRCONCbits.SLRC4 = 0U; /* Maximum slew rate */
    SLRCONCbits.SLRC5 = 0U; /* Maximum slew rate */
    
    /* INLVLx registers (ST/TTL input) */
    INLVLAbits.INLVLA0 = 1U; /* ST input */
    INLVLAbits.INLVLA1 = 1U; /* ST input */
    INLVLAbits.INLVLA2 = 1U; /* ST input */
    INLVLAbits.INLVLA3 = 1U; /* ST input */
    INLVLAbits.INLVLA4 = 1U; /* ST input */
    INLVLAbits.INLVLA5 = 1U; /* ST input */
    
    INLVLCbits.INLVLC0 = 1U; /* ST input */
    INLVLCbits.INLVLC1 = 1U; /* ST input */
    INLVLCbits.INLVLC2 = 1U; /* ST input */
    INLVLCbits.INLVLC3 = 1U; /* ST input */
    INLVLCbits.INLVLC4 = 1U; /* ST input */
    INLVLCbits.INLVLC5 = 1U; /* ST input */
    
    /* IOCx registers (interrupt-on-change) */ 
    
    /* IOCAP: INTERRUPT-ON-CHANGE PORTA POSITIVE EDGE REGISTER */
    IOCAPbits.IOCAP0 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCAPbits.IOCAP1 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCAPbits.IOCAP2 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCAPbits.IOCAP3 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCAPbits.IOCAP4 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCAPbits.IOCAP5 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    
    /* IOCCP: INTERRUPT-ON-CHANGE PORTC POSITIVE EDGE REGISTER */
    IOCCPbits.IOCCP0 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCCPbits.IOCCP1 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCCPbits.IOCCP2 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCCPbits.IOCCP3 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCCPbits.IOCCP4 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCCPbits.IOCCP5 = 0U; /* interrupt-On-Change is disabled for the associated pin */
            
    /* IOCAN: INTERRUPT-ON-CHANGE PORTA NEGATIVE EDGE REGISTER */
    IOCANbits.IOCAN0 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCANbits.IOCAN1 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCANbits.IOCAN2 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCANbits.IOCAN3 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCANbits.IOCAN4 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCANbits.IOCAN5 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    
    /* IOCCN: INTERRUPT-ON-CHANGE PORTC NEGATIVE EDGE REGISTER */
    IOCCNbits.IOCCN0 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCCNbits.IOCCN1 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCCNbits.IOCCN2 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCCNbits.IOCCN3 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCCNbits.IOCCN4 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    IOCCNbits.IOCCN5 = 0U; /* interrupt-On-Change is disabled for the associated pin */
    
    /* IOCAF: INTERRUPT-ON-CHANGE PORTA FLAG REGISTER */
    IOCAFbits.IOCAF0 = 0U;
    IOCAFbits.IOCAF1 = 0U;
    IOCAFbits.IOCAF2 = 0U;
    IOCAFbits.IOCAF3 = 0U;
    IOCAFbits.IOCAF4 = 0U;
    IOCAFbits.IOCAF5 = 0U;
    
    /* IOCCF: INTERRUPT-ON-CHANGE PORTC FLAG REGISTER */
    IOCCFbits.IOCCF0 = 0U;
    IOCCFbits.IOCCF1 = 0U;
    IOCCFbits.IOCCF2 = 0U;
    IOCCFbits.IOCCF3 = 0U;
    IOCCFbits.IOCCF4 = 0U;
    IOCCFbits.IOCCF5 = 0U;
     
    /* Unlock PPS */
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;

    /* Map TX to RA4 */
    RA4PPS = 0x16;
    
    /* Map C1OUT to RA2 */
    RA2PPS = 0x18;

    
    /* Lock PPS */
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1;
}

void init_comparator1() {
    
    /* Hysteresis enabled
     * Comparator enabled
     * Polarity not inverted 
     * Asynchronous 
     * Output 
     * Zero latency filter enabled
     */
    CM1CON0bits.HYS = 1U;
    CM1CON0bits.ON = 1U;
    CM1CON0bits.POL = 0U;
    CM1CON0bits.SYNC = 0U;
    CM1CON0bits.OUT = 0U;
    CM1CON0bits.ZLF = 1U;
    
    /* CINTN no_intFlag 
     * CINTP no_intFlag
     */
    CM1CON1bits.INTN = 0U;
    CM1CON1bits.INTP = 0U;
    
    /* CNCH CIN1-
     * CPCH DAC3 
     */
    CM1NSELbits.NCH = 1U;
    CM1PSELbits.PCH = 12U; 
}

void init_dac3(void) 
{
    /* Negative source select: VSS
     * Positive source select: VDD
     * DAC enabled
     * DAC Voltage output disabled 
     */
    
    DAC3CON0bits.NSS = 0U;
    DAC3CON0bits.PSS = 0U;
    DAC3CON0bits.EN = 1U;
    DAC3CON0bits.OE1 = 0U;
    
    /* DAC3REF = 16; Vp = 3.3V; Vn = 0V */ 
    /* DACx_output = (Vp-Vn)*(DAC3R/(2^5))+Vn
     * DACx_output = Vp*(DAC3R/(2^5))
     * DACx_output = 1.65V
     */
    
    DAC3REF = 16U;
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
    T1CONbits.CS = 0U;
    /* Timer1 input clock prescale select bits 1:1 */
    T1CONbits.CKPS = 0U;
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
    T1GCONbits.GGO_nDONE = 0U;
    /* Timer1 Gate Source Select bits - Timer0 overflow output (T0_overflow) */
    T1GCONbits.GSS = 1U; 
}

void init_timer1(void) {
    /* Timer1 clock source is the T1CLK1 pin (on the rising edge) */
    T1CONbits.CS = 2U;
    /* 1:1 prescale value */
    T1CONbits.CKPS = 0U;
    /* Synchronizes the asynchronous clock input with the system clock (FOSC) */
    T1CONbits.nT1SYNC = 0U;
    /* Stops Timer1 and  clears Timer1 gate flip-flop */
    T1CONbits.TMR1ON = 0U;
    
    /* Timer1 gate single-pulse acquisition has completed or has not been started */
    T1GCONbits.GGO_nDONE = 0U;
    /* Timer1 gate is active-low (Timer1 counts when gate is low) */
    T1GCONbits.T1GPOL = 0U;
    /* Timer1 gate toggle mode is disabled and toggle flip-flop is cleared */ 
    T1GCONbits.T1GTM = 0U;
    /* Timer1 gate pin (T1G) */
    T1GCONbits.GSS = 0U;
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
 * Comparator 1
 ******************************************************************************/

void enable_comparator1(void) 
{
    CM1CON0bits.C1ON = 1U;
}

void disable_comparator1(void) 
{
    CM1CON0bits.C1ON = 0U;
}

uint8_t get_comparator1_state(void) {
    return CMOUTbits.MC1OUT;
}

/****************************************************************************** 
 * 5 Bit DAC3
 ******************************************************************************/

void set_dac3(uint8_t inputData) {
    DAC3REF =  inputData;
}

uint8_t get_dac3(void) {
    return DAC3REF;
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