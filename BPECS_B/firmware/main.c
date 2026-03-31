/****************************************************************************** 
 * File:                | main.c 
 * Author:              | Michael P.
 * Revision:            | B1
 * Revision date:       | 2026-03-31
 * ****************************************************************************  
 * Revision history:
 * ****************************************************************************
 * Rev. | Date          | Comments
 * A    | 2025-11-03    | Initial draft
 * A1   | 2025-11-24    | Pinning added
 * A2   | 2026-02-09    | Pinning updated to comply with 
 *                        "battery_powered_eddy_current_sensor_rev_A_...
 *                        output_job_final_2026-01-22"
 *                        Description updated
 * B    | 2026-03-13    | Firmware for revision B hardware
 *                       "battery_powered_eddy_current_sensor_rev_B...
 *                        _output_job_final_2026-03-03"
 * B1   | 2026-03-31    | Timer2 no longer used for the timeout generation
 *
 ******************************************************************************/

/* **************************************************************************** 
 * Pinning
 * **************************************************************************** 
 * 
 *                            --------------------                                    
 *                        1 -|VDD              VSS|- 14             
 *               CLKIN -> 2 -|RA5/CLK  RA0/ICSPDAT|- 13 <-> PICKIT3                                
 *             UART_TX <- 3 -|RA4      RA1/ICSPCLK|- 12 <-> PICKIT3
 *             PICKIT3 -> 4 -|RA3/MCLR   RA2/T0CKI|- 11 -> C1OUT (to LC tank)                         
 *                LED4 <- 5 -|RC5              RC0|- 10 -> LSEL (select L)
 *                LED3 <- 6 -|RC4              RC1|-  9 <- C1IN1- (from LC tank)                                  
 *                LED2 <- 7 -|RC3              RC2|-  8 -> LED1                                                   
 *                            --------------------                
 *
 * **************************************************************************** 
 * Programming: ICSPDAT, ICSPCLK
 * **************************************************************************** 
 * 
 ******************************************************************************
 * Peripherials used: DAC, UART, comparator, Timer0, Timer1
 * ****************************************************************************
 *
 * ***************************************************************************** 
 * DAC
 * **************************************************************************** 
 * - DAC: VDD/2 biasing for LC oscillator
 * 
 * **************************************************************************** 
 * UART
 * **************************************************************************** 
 * - UART: For debugging purposes to get raw measurement values 
 * 
 * **************************************************************************** 
 * Comparator
 * ****************************************************************************
 * - Comparator: Integral part of the LC oscillator with push-pull output
 *               and integrated hysteresis
 * 
 * **************************************************************************** 
 * Timer modules
 * **************************************************************************** 
 * - Timer0 module: 8-bit counter with clock input T0CKI
 * - Timer1 module: 16-bit counter with T0 overflow gate (single toggle mode)
 * 
 * **************************************************************************** 
 * GPIOs
 * **************************************************************************** 
 * - Digital input:  
 * RA4 (external 16 MHz clock input), RC1 (comparator negative input)
 *
 * - Digital output: 
 * RC0 (LSEL), RC2 (LED1), RC3 (LED2), RC4 (LED3), RC5 (LED4)
 * RA4 (UART-TX), RA2 (C1OUT)
 *
 * ****************************************************************************
 * Settings
 * ****************************************************************************
 * - DAC output set to VDD/2 -> connected to CxVP)
 * - CxIN1- connected to tank circuit (LC oscillator) -> connected to CxVN
 ******************************************************************************/

#include "configuration_bits.h"
#include "detector.h"
#include "uart_hw.h"
#include "moving_average.h"

#define THRES_MEDIUM 10
#define THRES_HIGH 20

/*------ MAIN APPLICATION ----------------------------------------------------*/

int main(void)
{
    init_hw();
    init_hw_uart(BAUD38400_BRG);
    
    LSEL_low(); /* L1 selected */
    // LSEL_high(); /* L2 selected */
    
    LEDx_high(); /* All LEDs off */
    
    
    hw_uart_uint8_ascii_frame frame_uint8 = {
        .data = 0,
        .delimiter = '\n',
        .add_leading_zeros = ON,
        .use_delimiter = ON
    };
    
    hw_uart_uint16_ascii_frame frame_uint16 = {
        .data = 0,
        .delimiter = '\n',
        .add_leading_zeros = ON,
        .use_delimiter = ON
    };
    
    hw_uart_int16_ascii_frame frame_int16 = {
        .data = 0,
        .delimiter = '\n',
        .add_leading_zeros = ON,
        .use_delimiter = ON
    };
    
    
    measured_data raw_1 = {
        .data = 0,
        .err = 0
    };
    
    measured_data raw_2 = {
        .data = 0,
        .err = 0
    };
    
    /*
    grad_detector_data peak = {
        .curr_sum = 0,
        .delta = 0,
        .index = 0,
        .last_sum = 0,
        .neg_thres = -20,
        .p = NO,
        .pos_thres = 20,
        .state = INVALID
    };
    */
    
    /* init_grad_detector_data(&peak, 0u); */
    
    moving_sum_int16 delta_arr;
    init_moving_sum_data_int16(&delta_arr, 0u);
    
    int16_t delta = 0;
    int16_t delta_avrg = 0;
    int16_t delta_avrg_off = 4;
    
    delta_arr.data_state = VALID;
    
    
    
    while(1)
    {
        /* Get counts of first inductor */
        measure_task_sel(&raw_1, LSEL_low);
        
        /* Get counts of second inductor */
        measure_task_sel(&raw_2, LSEL_high);
        
        /*
        frame_uint16.data = raw_1.data; 
        send_hw_uart_uint16_ascii(&frame_uint16);
        
        frame_uint8.data = raw_1.err; 
        send_hw_uart_uint8_ascii(&frame_uint8); 
     
        frame_uint16.data = raw_2.data; 
        send_hw_uart_uint16_ascii(&frame_uint16);
        
        frame_uint8.data = raw_2.err; 
        send_hw_uart_uint8_ascii(&frame_uint8); 
        */
       
        /* Calculate delta of first and second inductor */
        delta = (int16_t)(raw_1.data-raw_2.data);

        /* Add value in moving sum filter */
        /* add_moving_sum_data_int16(&delta_arr, delta); */
        
        if (delta_arr.data_state == VALID) {
            /* delta_avrg = (int16_t)get_moving_sum_result_int16(&delta_arr); */
            delta_avrg = delta; 

            /* Compensate for a small offset between the two inductors */
            delta_avrg -= delta_avrg_off; 
            
            if ((raw_1.err!=0)||(raw_2.err!=0)) {
                /* Error state */
                delta_avrg = 9999;
            }
               
            if (delta_avrg > -THRES_MEDIUM && delta_avrg < THRES_MEDIUM) {
                LED_sig_state_low();
            } else if (delta_avrg <= -THRES_HIGH) {
                LED_sig_state_neg_high();

            } else if (delta_avrg >= THRES_HIGH) {
                LED_sig_state_pos_high();

            } else if (delta_avrg <= -THRES_MEDIUM) {
                LED_sig_state_neg_medium();

            } else { // delta_avrg >= THRES_MEDIUM 
                LED_sig_state_pos_medium();
            }
            
            frame_int16.data = delta_avrg; 
            send_hw_uart_int16_ascii(&frame_int16);    
        } else {
            /* DO NOTHING! */
        }


    }    
}
