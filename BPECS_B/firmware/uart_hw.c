/****************************************************************************** 
 * File:                | uart_hw.c  
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

/*------ INCLUDES ------------------------------------------------------------*/
#include "uart_hw.h"

const uint16_t exp_arr_uint16[UINT16_DIGITS] = {10000U, 1000U, 100U, 10U, 1U};
const uint8_t exp_arr_uint8[UINT8_DIGITS] = {100U, 10U, 1U};

/*------ FUNCTIONS -----------------------------------------------------------*/

void init_hw_uart(uint16_t baudrate) {
    /* Baud rate formula */
    /* for SYNC = 0U, BRG16 = 1U, BRGH = 1U */
    /* FOSC/(4(N+1)) */
    SPBRGH = 0U;
    SPBRGH |= (baudrate >> 8);
    SPBRGL = 0U;
    SPBRGL |= (baudrate & 0x00ff);
    
    /* Don't care, asychronous mode */
    TXSTAbits.CSRC = 0U;
    /* 8-bit transmission (8-bit data) */
    TXSTAbits.TX9 = 0U;
    /* Transmit is enabled */    
    TXSTAbits.TXEN = 1U;
    /* Asynchronous mode */
    TXSTAbits.SYNC = 0U;
    /* Sync Break transmission completed */
    TXSTAbits.SENDB = 0U;
    /* High baud rate select bit - High speed */
    TXSTAbits.BRGH = 1U;
    /* Ninth bit of transmit data - don't care */
    TXSTAbits.TX9D = 0U;
    
    /* Serial port is enabled*/
    RCSTAbits.SPEN = 1U;
    
    /* Transmits non-inverted data to the TX/CK pin */
    BAUDCONbits.SCKP = 0U;
    
    /* 16-Bit Baud Rate Generator */
    BAUDCONbits.BRG16 = 1U; 
}

void send_hw_uart_byte(uint8_t byte) {
    /* Wait for buffer to be empty */
    while(TRMT == 0);
    /* Send the data */
    TXREG = byte;     
}

void send_hw_uart_uint8_ascii(hw_uart_uint8_ascii_frame* frame) {
    /* Initialize with zero, will be filled later */
    uint8_t digit = 0U;
    uint8_t leading_zero = 1U;
    uint8_t data = frame->data;
    
    /* Convert uint8_t to ASCII */
    for (uint8_t i = 0; i < UINT8_DIGITS; i++) {
        /* Start with the largest digit */
        digit = (uint8_t)(data/exp_arr_uint8[i]);
        
        if (ON == frame->add_leading_zeros) {
            send_hw_uart_byte(digit+ASCII_OFF);
        } else if ((0U != digit)||(0U == leading_zero)) {
            send_hw_uart_byte(digit+ASCII_OFF);
            leading_zero = 0U;
        } else if (i == UINT8_DIGITS-1) {
            /* Send zero for the last digit anyway */
            send_hw_uart_byte(digit+ASCII_OFF);
        }
        /* Remainder of integer division */
        data = data%exp_arr_uint8[i];
    }
    
    /* Optional: Add delimiter */
    if (ON == frame->use_delimiter) {
        send_hw_uart_byte(frame->delimiter);
    }
}

void send_hw_uart_int8_ascii(hw_uart_int8_ascii_frame* frame) {
    /* Initialize with zero, will be filled later */
    uint8_t digit = 0U;
    uint8_t leading_zero = 1U;
    uint8_t data = 0U;
    
    /* Add sign of int8_t */
    if (frame->data < 0) {
        digit = ASCII_MINUS; 
        send_hw_uart_byte(digit);
        if (frame->data == INT8_MIN) {
            data = (uint8_t)128;
        } else {
            data = (uint8_t)(-frame->data);
        }
    } else {
        data = (uint8_t)(frame->data);
    }

    /* Convert uint8_t to ASCII */
    for (uint8_t i = 0; i < UINT8_DIGITS; i++) {
        /* Start with the largest digit */
        digit = (uint8_t)(data/exp_arr_uint8[i]);
        
        if (ON == frame->add_leading_zeros) {
            send_hw_uart_byte(digit+ASCII_OFF);
        } else if ((0U != digit)||(0U == leading_zero)) {
            send_hw_uart_byte(digit+ASCII_OFF);
            leading_zero = 0U;
        } else if (i == UINT8_DIGITS-1) {
            /* Send zero for the last digit anyway */
            send_hw_uart_byte(digit+ASCII_OFF);
        }
        /* Remainder of integer division */
        data = data%exp_arr_uint8[i];
    }
    
    /* Optional: Add delimiter */
    if (ON == frame->use_delimiter) {
        send_hw_uart_byte(frame->delimiter);
    }
}

void send_hw_uart_uint16_ascii(hw_uart_uint16_ascii_frame *frame) {
    /* Initialize with zero, will be filled later */
    uint8_t digit = 0U;
    uint8_t leading_zero = 1U;
    uint16_t data = frame->data;
    
    /* Convert uint16_t to ASCII */
    for (uint8_t i = 0; i < UINT16_DIGITS; i++) {
        /* Start with the largest digit */
        digit = (uint8_t)(data/exp_arr_uint16[i]);
        
        if (ON == frame->add_leading_zeros) {
            send_hw_uart_byte(digit+ASCII_OFF);
        } else if ((0U != digit)||(0U == leading_zero)) {
            send_hw_uart_byte(digit+ASCII_OFF);
            leading_zero = 0U;
        } else if (i == UINT16_DIGITS-1) {
            /* Send zero for the last digit anyway */
            send_hw_uart_byte(digit+ASCII_OFF);
        }
        /* Remainder of integer division */
        data = data%exp_arr_uint16[i];
    }
    
    /* Optional: Add delimiter */
    if (ON == frame->use_delimiter) {
        send_hw_uart_byte(frame->delimiter);
    }
}

void send_hw_uart_int16_ascii(hw_uart_int16_ascii_frame *frame) {
    /* Initialize with zero, will be filled later */
    uint8_t digit = 0U;
    uint8_t leading_zero = 1U;
    uint16_t data = 0U;
    
    /* Add sign of int16_t */
    if (frame->data < 0) {
        digit = ASCII_MINUS; 
        send_hw_uart_byte(digit);
        if (frame->data == INT16_MIN) {
            data = (uint16_t)32768;
        } else {
            data = (uint16_t)(-frame->data);
        }
    } else {
        data = (uint16_t)(frame->data);
    }

    /* Convert uint16_t to ASCII */
    for (uint8_t i = 0; i < UINT16_DIGITS; i++) {
        /* Start with the largest digit */
        digit = (uint8_t)(data/exp_arr_uint16[i]);
        
        if (ON == frame->add_leading_zeros) {
            send_hw_uart_byte(digit+ASCII_OFF);
        } else if ((0U != digit)||(0U == leading_zero)) {
            send_hw_uart_byte(digit+ASCII_OFF);
            leading_zero = 0U;
        } else if (i == UINT16_DIGITS-1) {
            /* Send zero for the last digit anyway */
            send_hw_uart_byte(digit+ASCII_OFF);
        }
        /* Remainder of integer division */
        data = data%exp_arr_uint16[i];
    }
    
    /* Optional: Add delimiter */
    if (ON == frame->use_delimiter) {
        send_hw_uart_byte(frame->delimiter);
    }
}