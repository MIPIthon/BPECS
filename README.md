Project Title: BPECS (Battery Powered Eddy Current Sensor)

The purpose of this project is to design a low-cost battery powered eddy current sensor intended for metal detection of objects made out of magnetic and conductive materials.

This project uses microcontroller by Microchip (8-Bit) PIC12/PIC16 to integrate most of the functionality in a single device.

HW revisions and description

**BPECS_A** - 2L HASL PCB / PIC12F1572 / 16 MHz external oscillator (+/- 50 ppm) / output via single LED or UART / With eddy current probe Ls=346 uH, Rs = 29 Ohm at 1 kHz

**BPECS_B** - 4L ENIG PCB / PIC16F1764 / 16 MHz external oscillator (+/- 50 ppm) / output four LEDs and UART / Without eddy current probe 

Principle of operation

LC oscillator based on MCU internal push pull output comparator with integrated hysteresis. Frequency measurement via a reciprocal counter realized with timer0 overflow as gate signal for timer1 (timer1 in signel shot toggle mode). 

Detection algorithm is based on signal change rather than absolute value detection, hence the risk of false positive detection due to changing environmental conditions is reduced. C0G capacitors for the tank circuit are still highly recommended. 



