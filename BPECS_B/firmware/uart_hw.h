/****************************************************************************** 
 * File:                | uart_hw.h   
 * Author:              | Michael P.
 * Revision:            | A
 * Revision date:       | 2026-02-09
 * ****************************************************************************  
 * Revision history:
 * ****************************************************************************
 * Rev. | Date          | Comments
 * A    | 2025-11-27    | Initial draft
 * A1   | 2026-02-09    | Added support for int16_t
 ******************************************************************************/

#ifndef UART_HW_H
#define	UART_HW_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 16000000U
#endif

/*------ INCLUDES ------------------------------------------------------------*/
#include <stdint.h>
#include <xc.h>

/*------ MACROS AND CONSTANTS ------------------------------------------------*/

/* For FOSC = 16 MHz */
#define BAUD9600_BRG    415U
#define BAUD19200_BRG   207U
#define BAUD38400_BRG   103U
#define BAUD160000_BRG   24U
#define BAUD200000_BRG   19U
#define BAUD250000_BRG   15U
#define BAUD1000000_BRG   3U
#define BAUD2000000_BRG   1U

#define UINT16_DIGITS     5U
#define UINT8_DIGITS      3U

#define ASCII_MINUS      45U
#define ASCII_OFF        48U

extern const uint8_t exp_arr_uint8[UINT8_DIGITS];
extern const uint16_t exp_arr_uint16[UINT16_DIGITS];

/*------ GLOBAL VARIABLES ----------------------------------------------------*/

/*------ TYPE DEFINITIONS ----------------------------------------------------*/
typedef enum state {OFF, ON} State;

typedef struct {
    uint8_t data;
    uint8_t delimiter;
    State add_leading_zeros;
    State use_delimiter;
} hw_uart_uint8_ascii_frame;

typedef struct {
    int8_t data;
    uint8_t delimiter;
    State add_leading_zeros;
    State use_delimiter;
} hw_uart_int8_ascii_frame;

typedef struct {
    uint16_t data;
    uint8_t delimiter;
    State add_leading_zeros;
    State use_delimiter;
} hw_uart_uint16_ascii_frame;

typedef struct {
    int16_t data;
    uint8_t delimiter;
    State add_leading_zeros;
    State use_delimiter;
} hw_uart_int16_ascii_frame;

/*------ FUNCTION DECLARATIONS -----------------------------------------------*/
void init_hw_uart(uint16_t);
void send_hw_uart_byte(uint8_t);
void send_hw_uart_uint8_ascii(hw_uart_uint8_ascii_frame*);
void send_hw_uart_int8_ascii(hw_uart_int8_ascii_frame*);
void send_hw_uart_uint16_ascii(hw_uart_uint16_ascii_frame*);
void send_hw_uart_int16_ascii(hw_uart_int16_ascii_frame *);

#endif /* UART_HW_H */
