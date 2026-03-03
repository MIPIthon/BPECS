/****************************************************************************** 
 * File:                | configuration_bits.h   
 * Author:              | Michael P.
 * Revision:            | A
 * Revision date:       | 2026-02-09
 * ****************************************************************************  
 * Revision history:
 * ****************************************************************************
 * Rev. | Date          | Comments
 * A    | 2025-11-25    | Initial draft
 ******************************************************************************/

#ifndef CONFIGURATION_BITS_H
#define	CONFIGURATION_BITS_H

/*------ INCLUDES ------------------------------------------------------------*/

/*------ MACROS AND CONSTANTS ------------------------------------------------*/

/*------ CONFIGURATION BITS --------------------------------------------------*/

/*------ CONFIGURATION 1 -----------------------------------------------------*/

/* Oscillator Selection 
 * -> External high speed oscillator connected to CLKIN pin */
#pragma config FOSC = ECH
/* Watchdog Timer Enable 
 * -> WDT disabled */
#pragma config WDTE = OFF
/* Power-up Timer Enable 
 * -> PWRT disabled */
#pragma config PWRTE = OFF
/* MCLR Pin Function Select 
 * -> MCLR/VPP pin function is MCLR */
#pragma config MCLRE = ON
/* Flash Program Memory Code Protection 
 * -> Program memory code protection is disabled */
#pragma config CP = OFF    
/* Brown-out Reset Enable 
 * -> Brown-out Reset enabled */
#pragma config BOREN = ON
/* Clock Out Enable
 * -> CLKOUT function is disabled. 
 * I/O or oscillator function on the CLKOUT pin */
#pragma config CLKOUTEN = OFF   

/*------ CONFIGURATION 2 -----------------------------------------------------*/

/* Flash Memory Self-Write Protection 
 * -> Write protection off */
#pragma config WRT = OFF        
/* PLL Enable 
 * -> 4x PLL disabled */
#pragma config PLLEN = OFF
/* Stack Overflow/Underflow Reset Enable 
 * -> Stack Overflow or Underflow will cause a Reset */
#pragma config STVREN = ON
/* Brown-out Reset Voltage Selection 
 * -> Brown-out Reset Voltage (Vbor), low trip point selected. */
#pragma config BORV = LO
/* Low-Voltage Programming Enable 
 * -> Low-voltage programming enabled */
#pragma config LVP = ON
/* Low Power Brown-out Reset enable bit 
 * -> LPBOR is disabled */
#pragma config LPBOREN = OFF   

/*------ GLOBAL VARIABLES ----------------------------------------------------*/

/*------ TYPE DEFINITIONS ----------------------------------------------------*/

/*------ FUNCTION DECLARATIONS -----------------------------------------------*/

#endif /* CONFIGURATION_BITS_H */
