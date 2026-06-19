<h1>BPECS (Battery Powered Eddy Current Sensor)</h1>

The purpose of this project is to design a low-cost battery powered eddy current sensor intended for detection of objects made out of magnetic and conductive materials.

This project uses microcontroller by Microchip (8-Bit) PIC12/PIC16 to integrate most of the functionality in a single device.

<h2>HW revisions and description</h2>

**BPECS_A:**

![BPECS_A](/BPECS_A/images/BPECS_A.jpg)

      2L HASL PCB
      
      PIC12F1572
      
      16 MHz external oscillator (+/- 50 ppm)
      
      Output via single LED or UART, digital input
      
      With eddy current probe Ls=346 uH, Rs = 29 Ohm at 1 kHz

      Known issue: Calculated inductance is incorrect. 
      R10, R11 = 1.2K used as the losses are higher than expected (lower Q)
      C6, C7, C8, C9 = 8.2n used due to the too low inductance value 
      fres ~ 94.5 kHz
      Single LED to visualize a positive or negative gradient is not really optimal from a user perspective.

**BPECS_B:**

![BPECS_B](/BPECS_B/images/BPECS_B.jpg)

      4L ENIG PCB
      
      PIC16F1764
      
      16 MHz external oscillator (+/- 50 ppm)

      Output via four LEDs and UART

      Added analog multiplexer to measure two probes
      
      Without eddy current probe - uses two eddy current probes (from BPECS_A) Ls=346 uH, Rs = 29 Ohm at 1 kHz
    

<h2>Principle of operation</h2>

LC oscillator based on MCU internal push pull output comparator with integrated hysteresis. Frequency measurement via a reciprocal counter realized with timer0 overflow as gate signal for timer1 (timer1 in single shot toggle mode). Timer0 is operating as a frequency divider.

Detection algorithm (used for BPECS_A) is based on signal change rather than absolute value detection, hence the risk of false positive detection due to changing environmental conditions is reduced. C0G/NP0 capacitors for the tank circuit are still highly recommended.

The usage of an external oscillator instead of the internal RC oscillator greatly improves the precision and accuracy of the frequency measurement due to excessive jitter of the internal oscillator. Even though absolute accuracy is not required, noise and short term frequency instability are a problem, that can not be corrected via simple base line filtering. Hence the external oscillator is highly recommended. 

<h2>Current consumption</h2>

**BPECS_A:**

    Isup = 3 mA (LED off) 
    Isup = 4.25 mA (LED on)

    Probe: BPECS_A PCB probe with fres=94.5 kHz

 **BPECS_B:**

    Isup_5V = 3.84 mA (All LEDs off) 
    Isup_5V = 5.18 mA (One LED on) 
    Isup_5V = 6.50 mA (Two LEDs on)
    
    Isup_9V = 3.87 mA (All LEDs off) 
    Isup_9V = 5.18 mA (One LED on) 
    Isup_9V = 6.51 mA (Two LEDs on) 

    Probe: BPECS_A PCB probes with fres=127.7 kHz   

<h2>Gated Counter vs. Reciprocal Counter</h2>

| Description | fref = 4 MHz | fref = 16 MHz |
| --- | --- | --- |
| Update rate [Hz] | ![Reciprocal](/General/reciprocal_counter/fosc_4MHZ/gate_frequency.svg) | ![Reciprocal](/General/reciprocal_counter/fosc_16MHZ/gate_frequency.svg) |
| Resolution [Hz] | ![Reciprocal](/General/reciprocal_counter/fosc_4MHZ/resolution_hz.svg) | ![Reciprocal](/General/reciprocal_counter/fosc_16MHZ/resolution_hz.svg) |
| Relative resolution [%] | ![Reciprocal](/General/reciprocal_counter/fosc_4MHZ/relative_resolution_percent.svg) | ![Reciprocal](/General/reciprocal_counter/fosc_16MHZ/relative_resolution_percent.svg) |
| Relative resolution [ppm] | ![Reciprocal](/General/reciprocal_counter/fosc_4MHZ/relative_resolution_ppm.svg) | ![Reciprocal](/General/reciprocal_counter/fosc_16MHZ/relative_resolution_ppm.svg) |

| Reciprocal fref = 4 MHz | Reciprocal fref = 16 MHz | Gated Counter tgate = 10 ms |
| --- | --- | --- |
| fres = 25 Hz (at 100 kHz, N=1024) | fres = 0.6 Hz (at 100 kHz, N=1024) | fres = 100 Hz |
| fgate = 100 Hz | fgate = 100 Hz | fgate = 100 Hz |

With the given hardware based on the circuits of BPECS_A and BPECS_B, we can only use fcy = fosc/4 = 16 MHz/4 = 4 MHz instead of 16 MHz due to the fact Timer0 synchronizes the signal to fcy. Timer0 (8 bit counter) prescaler equals 4, which means the used N equals 1024.

<h2>Modes of operation</h2>

**BPECS_A:** Frequency counter using the internal or external oscillator. 
    
    -> Fixed threshold based detection and offset determined once (=> not recommended)
    
    -> Fixed threshold and offset determined on power-up (=> not recommended) 
    
    -> Fixed threshold and offset cancelled by user (=> not recommended) 

    -> Fixed threshold, moving average filter and offset correction ("long" term drift) via gradient calculation (=> recommended)
    Drawback: User needs to move detector over object to detect it (conductive and magnetic can only be differentiated, via approach or withdrawing from the target.)

**BPECS_B:** Frequency counter using the internal or external oscillator. 

    -> Fixed threshold based detection and offset determined once (=> not recommended)
    
    -> Fixed threshold and offset determined on power-up (=> not recommended) 

    -> Fixed threshold, moving average filter and offset correction ("long" term drift) via gradient calculation (=> recommended)
    Drawback: User needs to move detector over object to detect it (conductive and magnetic can only be differentiated via approaching or withdrawing from the target.)

    -> Fixed threshold, correct long term drift (and e.g. capacitor drift) by difference between two probes.  (=> recommended)


<h2>Targets </h2>

![Targets](/BPECS_A/documentation/targets/targets_overview.jpg)

<h2>Detection performance</h2>

**BPECS_A:** using the following algorithm "Fixed threshold, moving average filter and offset correction ("long" term drift) via gradient calculation (=> recommended)"
    
    T1: 0 up to 10 mm (small iron nail - 1.2 mm diameter and 30 mm length)
    
    T2: 0 up to 40 mm (10 cent coin)
    
    T3: 0 up to 40 mm (1 cent coin)

**BPECS_B:** using the following algorithm "Fixed threshold, correct long term drift by difference between two probes  (=> recommended)"
    
    T1: 0 up to 10 mm (small iron nail - 1.2 mm diameter and 30 mm length)
    
    T2: 0 up to 40 mm (10 cent coin)
    
    T3: 0 up to 40 mm (1 cent coin)

![Targets](/BPECS_B/documentation/BPECS_B_differential_mode_operation.gif)

Revision A and B achieve the same sensitivity:

    -> same coil system
    -> reciprocal counter implemented in the same way and same oscillator type used

<h2>Future changes</h2>

    1) Add different colored LED bar (>=8 LEDs) controlled via a shift register to visualize the slope of the signal. 
    This allows the user to distinguish between primarily magnetic (permeability >> 1) or conductive (conductivity >> 0) objects.
    
    2) Add MCP1640 boost converter to allow operation from single AA or AAA cell. 
    Boost converter could be switched via reed switch.
    OR keep current implementation: Linear regulator, 9V battery and add a switch.

    3) Implementation of the reciprocal counter using the Timer0 Overflow is a weak point as it limits the resolution 
    due to the fact the external clock signal (T0) from the comparator (LC oscillator) is always synchronized to the instruction
    clock Fcy, hence only Fcy (4 MHz) instead of Fosc (16 MHz) is a meaningful choice as a clock input for Timer1 to
    measure the time between two overflows of T0. Timer0 is required as a frequency divider.
    
    -> Option 1: External frequency divider (e.g. SN74HC4040DR)
    -> Option 2: Use PIC12F1612 instead of PIC12F1572 => Timer6 used as prescaler (divider) for SMT1 (24 Bit counter). 
    Can be asynchronous, hence resolution advantage compared to the previous implementation (16 MHz instead of 4 MHz). 
    The 24-Bit counter value helps to utilize the increased resolution. 
    
    Advantages: No additional external components, frequency divider is SW configurable 
    (prescaler options 1:1, 1:2, 1:4, 1:8, 1:16, 1:32, 1:64 and 1:128), 
    in addition the PR6 register can be adjusted between 1 and 255. 
    The total number of periods can be minimum 1*1 and up to 128*255 maximum. 
    
    Disadvantages: Higher price, no HW-UART
          
    4) Add passive piezo buzzer as an alternative to the LED
