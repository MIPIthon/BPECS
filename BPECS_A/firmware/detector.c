/****************************************************************************** 
 * File:                | detector.c   
 * Author:              | Michael P.
 * Revision:            | A
 * Revision date:       | 2026-02-09
 * ****************************************************************************  
 * Revision history:
 * ****************************************************************************
 * Rev. | Date          | Comments
 * A    | 2026-12-09    | Initial draft
 ******************************************************************************/

/*------ INCLUDES ------------------------------------------------------------*/
#include "detector.h"

    
/*------ FUNCTIONS -----------------------------------------------------------*/

void measure_task(measured_data* mdata) {
    
    mdata->err = 0u;
    
    /* Start timer 2 */
    stop_timer2();
    set_timer2_tmr(0u);
    set_timer2_pr(100u); /* measurement time: 16 ms */
    clear_timer2_overflow();
    start_timer2();
    
    /* Start counter 1 */
    stop_timer1();
    set_timer1_value(0U);
    clear_timer1_overflow();
    start_timer1();
    
    /* Set timer0 to value close to overflow to reduce time */
    /* of a measurement cycle. */
    
    set_timer0_value(255U);   
    /* T1 Gate Go */
    start_timer1_single_pulse_mode();
     
    /* Check, if measurement time expired. */
    while(0u == get_timer2_overflow());
    
    /* Stop both timer and counter */
    stop_timer1();
    stop_timer2();

    /* Get counter value from timer1 */
    mdata->data = get_timer1_value();
    
    /* Error handling */
    /* Timer1 / counter1 - */
    if (1u == get_timer1_overflow()) {
        mdata->err |= OVERFLOW_MASK;
    }
    /* Timer1 / counter1 - single toggle aquisition still not finished */
    if (1u == get_timer1_single_pulse_mode_status()) {
        mdata->err |= TIMEOUT_MASK;
    }
    /* Range checks */
    if (mdata->data > UPPER_LIMIT) {
        mdata->err |= TOOHIGH_MASK;
    }
    if (mdata->data < LOWER_LIMIT) {
        mdata->err |= TOOLOW_MASK;
    }
}

void peak_detector_task(peak_detector_data* pdata, uint16_t value) {
    
    int32_t value_scaled = (int32_t)value*SCALE;
    
    /* Initialize baseline and filtered
     * with value to accelerate convergence */
    if (0 == pdata->index) {
        pdata->baseline = value_scaled;
        pdata->filtered = value_scaled;
    }
    
    /* Update slow baseline */
    pdata->baseline += (int32_t)(value_scaled-pdata->baseline)/pdata->k_baseline;
    /* Update filtered values */
    pdata->filtered += (int32_t)(value_scaled-pdata->filtered)/pdata->k_filtered;
    
    if (INVALID == pdata->state) {
        pdata->index++;
        if (pdata->index >= pdata->startup_samples) {
            pdata->state = VALID;
        }
    } else {  
        /* Calculate delta */
        pdata->delta = (pdata->filtered-pdata->baseline)/SCALE;
        
        if (pdata->delta <= pdata->neg_thres) {
            pdata->p = NEG;
        } else if (pdata->delta >= pdata->pos_thres) {
            pdata->p = POS;
        } else {
            pdata->p = NO;
        }
    }
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