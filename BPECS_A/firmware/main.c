/****************************************************************************** 
 * File:                | main.c 
 * Author:              | Michael P.
 * Revision:            | A3
 * Revision date:       | 2026-02-09
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
 ******************************************************************************/

/* **************************************************************************** 
 * Pinning
 * **************************************************************************** 
 * 
 *                            --------------------                                    
 *                        1 -|VDD              VSS|- 8                                 
 *               CLKIN -> 2 -|RA5      RA0/ICSPDAT|- 7 <-> PICKIT3 -> DO/UART-TX                                  
 *    (from LC) C1IN1- -> 3 -|RA4      RA1/ICSPCLK|- 6 <-> PICKIT3 <- DI                            
 *            PICKIT3 <-> 4 -|RA3/_MCLR        RA2|- 5 -> C1OUT (to LC)                              
 *                            --------------------                
 *
 * **************************************************************************** 
 * Used: VDD, VSS, _MCLR, CLKIN, C1IN1, C1OUT, DO (ICSPDAT), DI (ICSPCLK)
 * Programming: ICSPDAT, ICSPCLK
 * **************************************************************************** 
 * RA0: AN0, DAC1OUT, C1IN+, PWM2, TX/CK, CWG1B, IOC, ICPDAT/ICDDAT
 * RA1: AN1, VREF+, C1IN0+, PWM1, RX/DT, IOC, ICSPCLK/ICDCLK
 * RA2: AN2, C1OUT, T0CKI, PWM3, CWG1FLT/CWG1A, IOC INT
 * RA3: T1G, IOC, MCLR/VPP
 * RA4: AN3, C1IN1-, T1G, PWM2, TX/CK, CWG1B, IOC, CLKOUT
 * RA5: T1CKI, PWM1, RX/DT, CWG1A, IOC, CLKIN
 * 
 ******************************************************************************
 * Peripherials used: DAC, UART, comparator, Timer0, Timer1, Timer2
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
 * - Timer2 module: 8-bit timer used for timeout
 * 
 * **************************************************************************** 
 * GPIOs
 * **************************************************************************** 
 * - Digital input: RA1 (DI, if needed), RA4 (C1IN-), RA5 (CLKIN)
 * - Digital output: RA0 (UART-TX or DO), RA2 (C1OUT)
 *
 * ****************************************************************************
 * Settings
 * ****************************************************************************
 * - DAC output set to VDD/2 -> connected to CxVP)
 * - CxIN1- connected to tank circuit (LC oscillator) -> connected to CxVN
 * - Comparator output available at pin RA2 (C1OUT) and connected internally to
 *   T0CKI
 ******************************************************************************/

#include "configuration_bits.h"
#include "detector.h"
//#include "uart_hw.h"
#include "uart_bit_banged.h"
#include "moving_average.h"

/*------ MAIN APPLICATION ----------------------------------------------------*/

int main(void)
{
    
    init_hw();
    //init_hw_uart(BAUD38400_BRG);
    
    /*
    sw_uart_uint16_ascii_frame frame = {
        .data = 0,
        .delimiter = '\n',
        .high = gpio_high,
        .low = gpio_low,
        .bit_time = bit_time_BAUD9600,
        .par = NONE,
        .stop = ONE,
        .add_leading_zeros = ON,
        .use_delimiter = ON
    };
    */
    
    /*
    hw_uart_uint16_ascii_frame frame_uint16 = {
        .data = 0,
        .delimiter = '\n',
        .add_leading_zeros = ON,
        .use_delimiter = ON
    };
    
    hw_uart_uint8_ascii_frame frame_uint8 = {
        .data = 0,
        .delimiter = '\n',
        .add_leading_zeros = ON,
        .use_delimiter = ON
    };
    */
    
    measured_data raw = {
        .data = 0,
        .err = 0
    };
    
    /*
    peak_detector_data peak = {
        .baseline = 0,
        .filtered = 0,
        .delta = 0,
        .k_baseline = 1024,
        .k_filtered = 16,
        .neg_thres = -4,
        .pos_thres = 4,
        .state = INVALID,
        .index = 0,
        .startup_samples = 10,
        .p = NONE
    };
    */
    
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
    
    init_grad_detector_data(&peak, 0u);
        
    //moving_average_uint16 data;
    //init_moving_average_data_uint16(&data, 0x0000);
    
    while(1)
    {
        measure_task(&raw);
        //frame_uint16.data = raw.data;
        //frame_uint8.data = raw.err;
        //send_hw_uart_uint16_ascii(&frame_uint16);
        //send_hw_uart_uint8_ascii(&frame_uint8);
        //peak_detector_task(&peak, raw.data);
        grad_detector_task(&peak, raw.data);
        
        if (peak.p == NO) {
            gpio_low(); 
        } else {
            gpio_high();
        }
    }    
}
