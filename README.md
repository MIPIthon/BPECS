<h1>Project Title: BPECS (Battery Powered Eddy Current Sensor)</h1>

The purpose of this project is to design a low-cost battery powered eddy current sensor intended for metal detection of objects made out of magnetic and conductive materials.

This project uses microcontroller by Microchip (8-Bit) PIC12/PIC16 to integrate most of the functionality in a single device.

<h2>HW revisions and description</h2>

**BPECS_A:**
    
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

      4L ENIG PCB
      
      PIC16F1764
      
      16 MHz external oscillator (+/- 50 ppm)
      
      Output via four LEDs and UART

      Added analog multiplexer to measure two probes
      
      Without eddy current probe
    

<h2>Principle of operation</h2>

LC oscillator based on MCU internal push pull output comparator with integrated hysteresis. Frequency measurement via a reciprocal counter realized with timer0 overflow as gate signal for timer1 (timer1 in signel shot toggle mode). 

Detection algorithm is based on signal change rather than absolute value detection, hence the risk of false positive detection due to changing environmental conditions is reduced. C0G/NP0 capacitors for the tank circuit are still highly recommended.

The usage of an external oscillator instead of the internal RC oscillator greatly improves the precision and accuracy of the frequency measurement due to excessive jitter of the internal oscillator. Even though absolute accuracy is not required, noise and short term frequency instability are a problem, that can not be corrected via simple base line filtering. Hence the external oscillator is highly recommended. 

<h2>Current consumption</h2>

**BPECS_A:**

    Isup = 3 mA (LED off) 
    
    Isup = 4.25 mA (LED on)

    Probe: BPECS_A PCB probe with fres=94.5 kHz

<h2>Modes of operation </h2>

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

    -> Fixed threshold, correct long term drift by difference between two probes  (=> recommended)

<h2>Detection performance</h2>
    
    **BPECS_A:** using the following algorithm "Fixed threshold, moving average filter and offset correction ("long" term drift) via gradient calculation (=> recommended)"
    
    T1: 0 up to 10 mm (small iron nail - 1.2 mm diameter and 30 mm length)
    
    T2: 0 up to 40 mm (10 cent coin)
    
    T3: 0 up to 40 mm (1 cent coin)

<h2>Future changes</h2>

    1) Add different colored LED bar (>=8 LEDs) controlled via a shift register to visualize the slope of the signal. 
    This allows the user to distinguish between primarily magnetic (permeability >> 1) or conductive (conductivity >> 0) objects.
    
    2) Add MCP1640 boost converter to allow operation from single AA or AAA cell. 
    Boost converter could be switched via reed switch.

    3) ...
    
