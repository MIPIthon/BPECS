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


