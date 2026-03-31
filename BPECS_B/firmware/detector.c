/****************************************************************************** 
 * File:                | detector.c   
 * Author:              | Michael P.
 * Revision:            | B1
 * Revision date:       | 2026-03-31
 * ****************************************************************************  
 * Revision history:
 * ****************************************************************************
 * Rev. | Date          | Comments
 * A    | 2026-12-09    | Initial draft
 * B    | 2026-03-14    | HW version B: Peak detection removed
 * B1   | 2026-03-31    | Timer2 no longer used for timeout of measure_task()
 ******************************************************************************/

/*------ INCLUDES ------------------------------------------------------------*/
#include "detector.h"

    
/*------ FUNCTIONS -----------------------------------------------------------*/

void measure_task(measured_data* mdata) {
    
    mdata->err = 0u;
    
    /* Start counter 1 */
    stop_timer1();
    set_timer1_value(0U);
    clear_timer1_overflow();

    /* Set timer0 to value close to overflow to reduce time */
    /* of a measurement cycle. */
    set_timer0_value(T0_PRELOAD);
    
    /* Start timer1 and set T1 Gate Go */
    start_timer1();
    start_timer1_single_pulse_mode();
     
    /* Wait maximum expected time */
    __delay_us(TIMEOUT);
    
    /* Timer1 / counter1 - single toggle aquisition still not finished */
    if (1u == get_timer1_single_pulse_mode_status()) {
        mdata->err |= TIMEOUT_MASK;
    }
    
    /* Stop timer1 */
    stop_timer1();

    /* Get counter value from timer1 */
    mdata->data = get_timer1_value();
    
    /* Error handling */
    /* Timer1 / counter1 - */
    if (1u == get_timer1_overflow()) {
        mdata->err |= OVERFLOW_MASK;
    }
    /* Range checks */
    if (mdata->data > UPPER_LIMIT) {
        mdata->err |= TOOHIGH_MASK;
    }
    if (mdata->data < LOWER_LIMIT) {
        mdata->err |= TOOLOW_MASK;
    }
}

void measure_task_sel(measured_data* mdata, void (*sel)(void)) {
    sel();
    __delay_us(100);
    measure_task(mdata);
}


void grad_detector_task(grad_detector_data* gdata, uint16_t value) {
    if ((gdata->index) >= (NSAMP+KSAMP)) {
        /* Roll-over with index 0 */
        gdata->index = 0;
        /* The array is only considered to be valid, if the 
         * array is full, otherwise any further calculation might be wrong */
        gdata->state = VALID;
    }
    
    /* Add value to array */
    gdata->data_arr[gdata->index] = value;
    
    /* Set sums to zero */
    gdata->curr_sum = 0;
    gdata->last_sum = 0;
    gdata->delta = 0;
    
    /* Start index */
    int16_t start_index = (int16_t)(gdata->index)-(int16_t)NSAMP+1;
    if (start_index < 0) start_index += (int16_t)(NSAMP+KSAMP);
    
    /* Calculate current sum */
    for (uint8_t i=0; i<NSAMP; i++) {
        uint16_t idx = (uint16_t)(start_index+i)%(NSAMP+KSAMP);
        gdata->curr_sum += gdata->data_arr[idx];
    }
    
    /* Start index */
    start_index = (int16_t)(gdata->index)-(int16_t)NSAMP-(int16_t)KSAMP+1;
    if (start_index < 0) start_index += (int16_t)(NSAMP+KSAMP);
    
    /* Calculate last sum */
    for (uint8_t i=0; i<NSAMP; i++) {
        uint16_t idx = (uint16_t)(start_index+i)%(NSAMP+KSAMP);
        gdata->last_sum += gdata->data_arr[idx];
    }
    
    /* Calculate delta */
    gdata->delta = (int32_t)(gdata->curr_sum-gdata->last_sum);
    
    /* Determine peak state */
    if (gdata->delta <= gdata->neg_thres) {
        gdata->p = NEG;
    } else if (gdata->delta >= gdata->pos_thres) {
        gdata->p = POS;
    } else {
        gdata->p = NO;
    }
  
    /* Increment index */
    gdata->index++;
}

void init_grad_detector_data(grad_detector_data* gdata, uint16_t value) {
    for (uint8_t i = 0; i < NSAMP+KSAMP; i++) {
        gdata->data_arr[i] = value;
    }
    gdata->curr_sum = 0;
    gdata->last_sum = 0;
    gdata->delta = 0;
    gdata->index = 0;
    gdata->state = INVALID;
    gdata->p = NO;
}