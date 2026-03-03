/****************************************************************************** 
 * File:                | detector.h   
 * Author:              | Michael P.
 * Revision:            | A
 * Revision date:       | 2026-02-09
 * ****************************************************************************  
 * Revision history:
 * ****************************************************************************
 * Rev. | Date          | Comments
 * A    | 2026-12-09    | Initial draft
 ******************************************************************************/

#ifndef DETECTOR_H
#define	DETECTOR_H


/*------ INCLUDES ------------------------------------------------------------*/
#include "common.h"
#include "hw_driver.h"
#include <stdint.h>

/*------ MACROS AND CONSTANTS ------------------------------------------------*/
#define LOWER_LIMIT      30000u     /* equals approximately 136 kHZ */
#define UPPER_LIMIT      60000u     /* equals approximately 68 kHz */

#define TOOLOW_MASK      0b00000001
#define TOOHIGH_MASK     0b00000010
#define OVERFLOW_MASK    0b00000100
#define TIMEOUT_MASK     0b00001000

#define SCALE            1024u     
#define NSAMP            16u /* Number of samples of moving sum calculation */
#define KSAMP            4u /* Number of samples gap for gradient calculation */

/*------ GLOBAL VARIABLES ----------------------------------------------------*/


/*------ TYPE DEFINITIONS ----------------------------------------------------*/
typedef enum peak {NO=0, NEG, POS} Peak;

typedef struct {
    uint16_t data;
    uint8_t err;
} measured_data;

typedef struct {
    int32_t baseline;
    int32_t filtered;
    int32_t delta;
    uint16_t k_baseline;
    uint8_t k_filtered; 
    int8_t neg_thres;
    int8_t pos_thres;
    Status state;
    uint8_t index;
    uint8_t startup_samples;
    Peak p;
} peak_detector_data;

typedef struct {
    uint16_t data_arr[NSAMP+KSAMP];
    uint8_t index;
    uint32_t curr_sum;
    uint32_t last_sum;
    int32_t delta;
    int8_t neg_thres;
    int8_t pos_thres;
    Status state;
    Peak p;
} grad_detector_data;

/*------ FUNCTION DECLARATIONS -----------------------------------------------*/
void measure_task(measured_data*);
void peak_detector_task(peak_detector_data*, uint16_t);
void grad_detector_task(grad_detector_data*, uint16_t);
void init_grad_detector_data(grad_detector_data*, uint16_t);

#endif /* DETECTOR_H */
