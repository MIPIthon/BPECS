/****************************************************************************** 
 * File:                | moving_average.h   
 * Author:              | Michael P.
 * Revision:            | B
 * Revision date:       | 2026-03-14
 * ****************************************************************************  
 * Revision history:
 * ****************************************************************************
 * Rev. | Date          | Comments
 * A    | 2025-11-03    | Initial draft
 * A1   | 2026-02-09    | Added constants for addition number of samples
 *                      | Added support for int16_t
 * B    | 2026-03-14    | Added moving sum calculation
 ******************************************************************************/

#ifndef MOVING_AVERAGE_H
#define	MOVING_AVERAGE_H


/*------ INCLUDES ------------------------------------------------------------*/
#include <stdint.h>
#include "common.h"

/*------ MACROS AND CONSTANTS ------------------------------------------------*/
#define N_SAMP_02 2U
#define N_SAMP_04 4U
#define N_SAMP_08 8U
#define N_SAMP_16 16U
#define N_SAMP_32 32U
#define N_SAMP_64 64U

#define NUMBER_OF_SAMPLES N_SAMP_32

/*------ GLOBAL VARIABLES ----------------------------------------------------*/

/*------ TYPE DEFINITIONS ----------------------------------------------------*/
typedef struct {
    uint16_t data_arr[NUMBER_OF_SAMPLES];
    uint8_t index;
    Status data_state;
} moving_average_uint16;

typedef struct {
    int16_t data_arr[NUMBER_OF_SAMPLES];
    uint8_t index;
    Status data_state;
} moving_average_int16;

typedef struct {
    int16_t data_arr[NUMBER_OF_SAMPLES];
    uint8_t index;
    Status data_state;
} moving_sum_int16;
    
/*------ FUNCTION DECLARATIONS -----------------------------------------------*/

void init_moving_average_data_uint16(moving_average_uint16 *, uint16_t);
void add_moving_average_data_uint16(moving_average_uint16 *, uint16_t);
uint16_t get_moving_average_result_uint16(moving_average_uint16 *);

void init_moving_average_data_int16(moving_average_int16 *, int16_t);
void add_moving_average_data_int16(moving_average_int16 *, int16_t);
int16_t get_moving_average_result_int16(moving_average_int16 *);

void init_moving_sum_data_int16(moving_sum_int16 *, int16_t);
void add_moving_sum_data_int16(moving_sum_int16 *, int16_t);
int32_t get_moving_sum_result_int16(moving_sum_int16 *);

#endif /* MOVING_AVERAGE_H */