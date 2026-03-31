/****************************************************************************** 
 * File:                | detector.h   
 * Author:              | Michael P.
 * Revision:            | B1
 * Revision date:       | 2026-03-31
 * ****************************************************************************  
 * Revision history:
 * ****************************************************************************
 * Rev. | Date          | Comments
 * A    | 2026-12-09    | Initial draft
 * B    | 2026-03-14    | HW version B: Peak detection removed
 * B1   | 2026-03-31    | Constants T0_PRELOAD and TIMEOUT added
 ******************************************************************************/

#ifndef DETECTOR_H
#define	DETECTOR_H


/*------ INCLUDES ------------------------------------------------------------*/
#include "common.h"
#include "hw_driver.h"
#include <stdint.h>

/*------ MACROS AND CONSTANTS ------------------------------------------------*/
#define LOWER_LIMIT      30000u         /* equals approximately to 136 kHZ */
#define UPPER_LIMIT      55000u         /* equals approximately to 74 kHz */

#define TOOLOW_MASK      0b00000001
#define TOOHIGH_MASK     0b00000010
#define OVERFLOW_MASK    0b00000100
#define TIMEOUT_MASK     0b00001000

#define NSAMP            16u    /* Number of samples of moving sum calculation */
#define KSAMP            4u     /* Number of samples gap for gradient calculation */

#define T0_PRELOAD       250u   /* Timer0 preload value */
#define TIMEOUT          15000  /* (wait time) in us */

/*------ GLOBAL VARIABLES ----------------------------------------------------*/


/*------ TYPE DEFINITIONS ----------------------------------------------------*/
typedef enum peak {NO=0, NEG, POS} Peak;

typedef struct {
    uint16_t data;
    uint8_t err;
} measured_data;

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
void measure_task_sel(measured_data*, void (*)(void));

void grad_detector_task(grad_detector_data*, uint16_t);
void init_grad_detector_data(grad_detector_data*, uint16_t);

#endif /* DETECTOR_H */
