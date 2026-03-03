/****************************************************************************** 
 * File:                | moving_average.h   
 * Author:              | Michael P.
 * Revision:            | A1
 * Revision date:       | 2026-02-09
 * ****************************************************************************  
 * Revision history:
 * ****************************************************************************
 * Rev. | Date          | Comments
 * A    | 2025-11-03    | Initial draft
 * A1   | 2026-02-09    | Added constants for addition number of samples
 *                      | Added support for int16_t
 ******************************************************************************/

/*------ INCLUDES ------------------------------------------------------------*/
#include "moving_average.h"

    
/*------ FUNCTIONS -----------------------------------------------------------*/

/* Function to calculate moving average with UINT16 */

void init_moving_average_data_uint16(moving_average_uint16 *data, uint16_t value) {
    for (uint8_t i = 0; i < NUMBER_OF_SAMPLES; i++) {
        data->data_arr[i] = value;
    }
    data->data_state = INVALID;
    data->index = 0;
}

void add_moving_average_data_uint16(moving_average_uint16 *data, uint16_t value) {
    data->data_arr[data->index] = value;
    data->index += 1;
    if (data->index >= NUMBER_OF_SAMPLES) {
        data->index = 0;
        data->data_state = VALID;
    }
}

uint16_t get_moving_average_result_uint16(moving_average_uint16 *data) {
    uint32_t temp = 0;
    for (uint8_t i=0; i < NUMBER_OF_SAMPLES; i++) {
        temp += data->data_arr[i];
    }
    return (uint16_t)(temp/NUMBER_OF_SAMPLES);
}

/* Function to calculate moving average with INT16 */

void init_moving_average_data_int16(moving_average_int16 *data, int16_t value) {
    for (uint8_t i = 0; i < NUMBER_OF_SAMPLES; i++) {
        data->data_arr[i] = value;
    }
    data->data_state = INVALID;
    data->index = 0;
}

void add_moving_average_data_int16(moving_average_int16 *data, int16_t value) {
    data->data_arr[data->index] = value;
    data->index += 1;
    if (data->index >= NUMBER_OF_SAMPLES) {
        data->index = 0;
        data->data_state = VALID;
    }
}

int16_t get_moving_average_result_int16(moving_average_int16 *data) {
    int32_t temp = 0;
    for (uint8_t i=0; i < NUMBER_OF_SAMPLES; i++) {
        temp += data->data_arr[i];
    }
    return (int16_t)(temp/NUMBER_OF_SAMPLES);
}
