/****************************************************************************** 
 * File:                | hw_driver.h   
 * Author:              | Michael P.
 * Revision:            | B
 * Revision date:       | 2026-03-14
 * ****************************************************************************  
 * Revision history:
 * ****************************************************************************
 * Rev. | Date          | Comments
 * A    | 2025-11-03    | Initial draft
 * A1   | 2025-11-24    | Added enable, disable comparator function
 * B    | 2026-03-14    | Modified HW driver to address change to PIC16F1764
 ******************************************************************************/

#ifndef HW_DRIVER_H
#define	HW_DRIVER_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 16000000U
#endif


/*------ INCLUDES ------------------------------------------------------------*/
#include <stdint.h>
#include <xc.h>

/*------ MACROS AND CONSTANTS ------------------------------------------------*/
#define T2_POSTS_1_1 0U
#define T2_POSTS_1_2 1U
#define T2_POSTS_1_3 2U
#define T2_POSTS_1_4 3U
#define T2_POSTS_1_5 4U
#define T2_POSTS_1_6 5U
#define T2_POSTS_1_7 6U
#define T2_POSTS_1_8 7U
#define T2_POSTS_1_9 8U
#define T2_POSTS_1_10 9U
#define T2_POSTS_1_11 10U
#define T2_POSTS_1_12 11U
#define T2_POSTS_1_13 12U
#define T2_POSTS_1_14 13U
#define T2_POSTS_1_15 14U
#define T2_POSTS_1_16 15U

#define T2_PRES_1_1 0U
#define T2_PRES_1_4 1U
#define T2_PRES_1_16 2U
#define T2_PRES_1_64 3U

#define T0_PRES_1_1 0U
#define T0_PRES_1_2 0U
#define T0_PRES_1_4 1U
#define T0_PRES_1_8 2U
#define T0_PRES_1_16 3U
#define T0_PRES_1_32 4U
#define T0_PRES_1_64 5U
#define T0_PRES_1_128 6U
#define T0_PRES_1_256 7U

/*------ GLOBAL VARIABLES ----------------------------------------------------*/

/*------ TYPE DEFINITIONS ----------------------------------------------------*/
    
/*------ FUNCTION DECLARATIONS -----------------------------------------------*/

static inline void LED1_high(void) { LATCbits.LATC2 = 1; }
static inline void LED1_low(void)  { LATCbits.LATC2 = 0; }

static inline void LED2_high(void) { LATCbits.LATC3 = 1; }
static inline void LED2_low(void)  { LATCbits.LATC3 = 0; }

static inline void LED3_high(void) { LATCbits.LATC4 = 1; }
static inline void LED3_low(void)  { LATCbits.LATC4 = 0; }

static inline void LED4_high(void) { LATCbits.LATC5 = 1; }
static inline void LED4_low(void)  { LATCbits.LATC5 = 0; }

static inline void LEDx_high(void) { LED1_high(); LED2_high(); LED3_high(); LED4_high(); }
static inline void LEDx_low(void) { LED1_low(); LED2_low(); LED3_low(); LED4_low(); }

static inline void LED_sig_state_neg_high(void) { LED1_low(); LED2_low(); LED3_high(); LED4_high(); }
static inline void LED_sig_state_neg_medium(void) { LED1_high(); LED2_low(); LED3_high(); LED4_high(); }
static inline void LED_sig_state_low(void) { LEDx_high(); }
static inline void LED_sig_state_pos_medium(void) { LED1_high(); LED2_high(); LED3_low(); LED4_high(); }
static inline void LED_sig_state_pos_high(void) { LED1_high(); LED2_high(); LED3_low(); LED4_low(); }
 
static inline void LSEL_high(void) { LATCbits.LATC0 = 1; }
static inline void LSEL_low(void)  { LATCbits.LATC0 = 0; }

/****************************************************************************** 
 * Initialization 
 ******************************************************************************/
void init_hw(void);

void init_clock(void);
void init_bor(void);
void init_pins(void);
void init_comparator1(void);
void init_dac3(void);
void init_timer0(void);
void init_timer1_gate_single_pulse_toggle(void);
void init_timer1(void);
void init_timer2(void);

/****************************************************************************** 
 * Comparator1
 ******************************************************************************/
void enable_comparator1(void);
void disable_comparator1(void);
uint8_t get_comparator_state1(void);

/****************************************************************************** 
 * 5 Bit DAC3
 ******************************************************************************/
void set_dac3(uint8_t);
uint8_t get_dac3(void);

/****************************************************************************** 
 * Timer0
 ******************************************************************************/
uint8_t get_timer0_value(void);
void set_timer0_value(uint8_t);
void clear_timer0_overflow(void);
uint8_t get_timer0_overflow(void);
void set_timer0_overflow(void);

/****************************************************************************** 
 * Timer1
 ******************************************************************************/
void start_timer1(void);
void stop_timer1(void);
void set_timer1_value(uint16_t);
uint8_t get_timer1_single_pulse_mode_status(void);
void start_timer1_single_pulse_mode(void);
uint16_t get_timer1_value(void);
void clear_timer1_overflow(void);
uint8_t get_timer1_overflow(void);

/****************************************************************************** 
 * Timer2
 ******************************************************************************/
void start_timer2(void);
void stop_timer2(void);
void clear_timer2_overflow(void);
uint8_t get_timer2_overflow(void);
void set_timer2_tmr(uint8_t);
void set_timer2_pr(uint8_t);

#endif /* HW_DRIVER_H */
