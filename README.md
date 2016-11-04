# Plug-n-play USB Gamepad adapter for 6 channel RC Transmitter PPM output
 This sketch turns Arduino Leonardo (Pro Micro) or any other ATmega32U4 based *duino clone into a plug-n-play DirectX compatible
 USB Gamepad adapter for 6 channel RC Transmitter PPM output.
 The adapter can be used to play FreeRider FPV (http://fpv-freerider.itch.io/fpv-freerider),
 aerofly RC 7 (http://www.aeroflyrc.com/)
 or other flight simulators.

#Warning
 Do not flash non ATmega32U4 MCU's with this sketch. It wont do any harm but will not work.

#Features
 - 6 channels PPM input
 - Compatible with any RC transmitter with PPM output (only first 6 channels used)
 - Interrupt driven 0.5 uS accuracy
 - Low jitter
 - 4 axis USB Gamepad (X,Y,Z,Z rotation) + 2 sliders
 - 1000 points resolution
 - Plug-n-play
 - DirectX compatible

#Connections
 - RC PPM out <--> Digital Pin 4 of Arduino Leonardo (Pro Micro)
 - RC GND  <--> Arduino GND

 This sketch uses Timer1 ICP1 interrupt so connection to Digital pin 4 is mandatory.

#Flashing
 - Open the sketch in Arduino IDE, choose Leonardo board, upload.

#Usage
 In Windows: open *Devices and Printers* find *Arduino Leonardo*. Then right click on it and choose *Game controller settings*. Calibrate joystick using your RC Transmitter connected.

 Tested with FlySky(Turnigy) FS-i6 transmitter, aerofly RC 7 simulator and FreeRider FPV.
 FlySky FS-i6 flashed with 10 channels firmware. On the trainer port it transmits only 6 channels.
 Note: consider turning on *Trainer mode* in the RC configuration menu. It wont stop RC from binding and transmitting, but can prevent your aircraft from flying away accidentally.
 You can remap AUX1 and AUX2 dials to switches in RC configuration.

#Trainer port of FlySky FS-i6
 ![Alt text](images/fs-i6.png?raw=true "Trainer port")

#Sample screens
 Adding controller in aerofly RC 7
 ![Alt text](images/aerofly1.png?raw=true "Adding controller")

 Controller configuration in aerofly RC 7
 ![Alt text](images/aerofly2.png?raw=true "Controller configuration")

  Controller calibration in FreeRider FPV
 ![Alt text](images/freerider.png?raw=true "Controller calibration")