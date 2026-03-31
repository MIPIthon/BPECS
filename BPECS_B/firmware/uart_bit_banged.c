/****************************************************************************** 
 * File:                | uart_bit_banged.c  
 * Author:              | Michael P.
 * Revision:            | A2
 * Revision date:       | 2026-02-09
 * ****************************************************************************  
 * Revision history:
 * ****************************************************************************
 * Rev. | Date          | Comments
 * A    | 2025-11-03    | Initial draft
 * A1   | 2025-11-24    | RA1 used for SW UART
 * A2   | 2026-02-09    | Functions and datatypes renamed with prefix SW
 ******************************************************************************/

/*------ INCLUDES ------------------------------------------------------------*/
#include "uart_bit_banged.h"

const uint16_t exp_arr[UINT16_DIGITS] = {10000U, 1000U, 100U, 10U, 1U};
    
/*------ FUNCTIONS -----------------------------------------------------------*/

void send_sw_uart_byte(sw_uart_byte_frame *frame, void (*high)(void), void (*low)(void), void (*bit_time)(void)) {
    uint8_t ones_cntr = 0;

    /* Start bit (LOW) */
    low();
    bit_time();

    /* Data bits (LSB first) */
    for (uint8_t i = 0; i < 8; i++) {
        if ((frame->payload >> i) & 0x01) {
            high();
            ones_cntr++;
        } else {
            low();
        }
        bit_time();
    }

    /* Parity bit */
    switch (frame->par) {
        case MARK:  high(); bit_time(); break;
        case SPACE: low(); bit_time(); break;
        case EVEN:  (ones_cntr % 2) ? high() : low(); bit_time(); break;
        case ODD:   (ones_cntr % 2) ? low()  : high(); bit_time(); break;
        default: break; /* NONE */
    }

    /* Stop bit(s) (HIGH) */
    high();
    bit_time();
    
    if (frame->stop == TWO) {
        bit_time();
    }
}

void send_sw_uart_uint16_ascii(sw_uart_uint16_ascii_frame *frame) {
    /* Initialize with zero, will be filled later */
    sw_uart_byte_frame byte_frame = {0x00, frame->par, frame->stop};
    uint8_t digit = 0U;
    uint8_t leading_zero = 1U;
    uint16_t data = frame->data;
    
    /* Convert uint16_t to ASCII */
    for (uint8_t i = 0; i < UINT16_DIGITS; i++) {
        /* Start with the largest digit */
        digit = (uint8_t)(data/exp_arr[i]);
        
        if (ON == frame->add_leading_zeros) {
            byte_frame.payload = digit+ASCII_OFF;
            send_sw_uart_byte(&byte_frame, frame->high, frame->low, frame->bit_time);
        } else if ((0U != digit)||(0U == leading_zero)) {
            byte_frame.payload = digit+ASCII_OFF;
            send_sw_uart_byte(&byte_frame, frame->high, frame->low, frame->bit_time);
            leading_zero = 0U;
        } else if (i == UINT16_DIGITS-1) {
            /* Send zero for the last digit anyway */
            byte_frame.payload = digit+ASCII_OFF;
            send_sw_uart_byte(&byte_frame, frame->high, frame->low, frame->bit_time);
        }
        /* Remainder of integer division */
        data = data%exp_arr[i];
    }
    
    /* Optional: Add delimiter */
    if (ON == frame->use_delimiter) {
        byte_frame.payload = frame->delimiter;
        send_sw_uart_byte(&byte_frame, frame->high, frame->low, frame->bit_time);
    }
}

void send_sw_uart_int16_ascii(sw_uart_int16_ascii_frame *frame) {
    /* Initialize with zero, will be filled later */
    sw_uart_byte_frame byte_frame = {0x00, frame->par, frame->stop};
    uint8_t digit = 0U;
    uint8_t leading_zero = 1U;
    uint16_t data = 0U;
    
    /* Add sign of int16 */
    if (frame->data < 0) {
        byte_frame.payload = ASCII_MINUS; 
        send_sw_uart_byte(&byte_frame, frame->high, frame->low, frame->bit_time);
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
        digit = (uint8_t)(data/exp_arr[i]);
        
        if (ON == frame->add_leading_zeros) {
            byte_frame.payload = digit+ASCII_OFF;
            send_sw_uart_byte(&byte_frame, frame->high, frame->low, frame->bit_time);
        } else if ((0U != digit)||(0U == leading_zero)) {
            byte_frame.payload = digit+ASCII_OFF;
            send_sw_uart_byte(&byte_frame, frame->high, frame->low, frame->bit_time);
            leading_zero = 0U;
        } else if (i == UINT16_DIGITS-1) {
            /* Send zero for the last digit anyway */
            byte_frame.payload = digit+ASCII_OFF;
            send_sw_uart_byte(&byte_frame, frame->high, frame->low, frame->bit_time);
        }
        /* Remainder of integer division */
        data = data%exp_arr[i];
    }
    
    /* Optional: Add delimiter */
    if (ON == frame->use_delimiter) {
        byte_frame.payload = frame->delimiter;
        send_sw_uart_byte(&byte_frame, frame->high, frame->low, frame->bit_time);
    }
}