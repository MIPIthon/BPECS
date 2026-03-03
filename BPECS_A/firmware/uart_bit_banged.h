/****************************************************************************** 
 * File:                | uart_bit_banged.h   
 * Author:              | Michael P.
 * Revision:            | A2
 * Revision date:       | 2026-02-09
 * ****************************************************************************  
 * Revision history:
 * ****************************************************************************
 * Rev. | Date          | Comments
 * A    | 2025-11-03    | Initial draft
 * A1   | 2025-11-24    | RA1 used for SW UART
 * A2   | 2026-02-09    | Functions and data types renamed with prefix SW
 ******************************************************************************/

#ifndef UART_BIT_BANGED_H
#define	UART_BIT_BANGED_H

/* Note: If the _XTAL_FREQ is wrong, the BAUDRATE will be incorrect! */
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 16000000U
#endif


/*------ INCLUDES ------------------------------------------------------------*/
#include <stdint.h>
#include <xc.h>

/*------ MACROS AND CONSTANTS ------------------------------------------------*/

/*
 The following bit times apply
 (1) 1200 baud => 833 us
 (2) 2400 baud => 417 us
 (3) 4800 baud => 208 us
 (4) 9600 baud => 104 us => 1 Start bit + 8 Data bits + 1 Stop bit 
                         => 10 bits per digit => approx. 5 ms for 5 digits 
*/

#define BAUD9600 90U /* Already manually adapted to account for overhead! */
#define BAUD4800 208U
#define BAUD2400 417U
#define BAUD1200 833U

#define UINT16_DIGITS 5U

#define ASCII_MINUS 45U
#define ASCII_OFF 48U

extern const uint16_t exp_arr[UINT16_DIGITS];

/*------ GLOBAL VARIABLES ----------------------------------------------------*/

/*------ TYPE DEFINITIONS ----------------------------------------------------*/

typedef enum parity {NONE, EVEN, ODD, MARK, SPACE} Parity;
typedef enum stopbit {ONE, TWO} Stopbit;
typedef enum state {OFF, ON} State;

typedef struct {
    uint8_t payload; /* Always 8 bit */
    Parity par;
    Stopbit stop;
} sw_uart_byte_frame; 

typedef struct {
    uint16_t data;
    uint8_t delimiter;
    void (*high)(void);
    void (*low)(void);
    void (*bit_time)(void);
    Parity par;
    Stopbit stop;
    State add_leading_zeros;
    State use_delimiter;
} sw_uart_uint16_ascii_frame;

typedef struct {
    int16_t data;
    uint8_t delimiter;
    void (*high)(void);
    void (*low)(void);
    void (*bit_time)(void);
    Parity par;
    Stopbit stop;
    State add_leading_zeros;
    State use_delimiter;
} sw_uart_int16_ascii_frame;

/*------ FUNCTION DECLARATIONS -----------------------------------------------*/

static inline void gpio_high(void) { LATAbits.LATA0 = 1; }
static inline void gpio_low(void)  { LATAbits.LATA0 = 0; }

static inline void bit_time_BAUD9600(void)  { __delay_us(BAUD9600); }
static inline void bit_time_BAUD4800(void)  { __delay_us(BAUD4800); }
static inline void bit_time_BAUD2400(void)  { __delay_us(BAUD2400); }
static inline void bit_time_BAUD1200(void)  { __delay_us(BAUD1200); }

void send_sw_uart_byte(sw_uart_byte_frame *, void (*)(void), void (*)(void), void (*)(void));
void send_sw_uart_uint16_ascii(sw_uart_uint16_ascii_frame *);
void send_sw_uart_int16_ascii(sw_uart_int16_ascii_frame *);

#endif /* UART_BIT_BANGED_H */


