/****************************************************************************** 
 * File:                | hw_driver.h   
 * Author:              | Michael P.
 * Revision:            | A
 * Revision date:       | 2025-03-11
 * ****************************************************************************  
 * Revision history:
 * ****************************************************************************
 * Rev. | Date          | Comments
 * A    | 2025-11-03    | Initial draft
 * A1   | 2025-11-24    | Added enable, disable comparator function
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

/****************************************************************************** 
 * Initialization 
 ******************************************************************************/
void init_hw(void);

void init_clock(void);
void init_bor(void);
void init_pins(void);
void init_comparator(void);
void init_dac(void);
void init_timer0(void);
void init_timer1_gate_single_pulse_toggle(void);
void init_timer1(void);
void init_timer2(void);

/****************************************************************************** 
 * Comparator
 ******************************************************************************/
void enable_comparator(void);
void disable_comparator(void);
uint8_t get_comparator_state(void);

/****************************************************************************** 
 * DAC
 ******************************************************************************/
void set_dac(uint8_t);
uint8_t get_dac(void);

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
