# Plug-n-play USB Gamepad adapter for 6 channel RC Transmitter PPM output
 This sketch turns Arduino Leonardo (Pro Micro) or any other ATmega32U4 based *duino clone into a plug-n-play DirectX compatible
 USB Gamepad adapter for 6 channel RC Transmitter PPM output.
 The adapter can be used to play any flight or FPV drone simulator.



# Features
 - 6 channels PPM input
 - Compatible with any RC transmitter with PPM output (only first 6 channels used)
 - Interrupt driven 0.5 uS accuracy
 - Low jitter
 - 4 axis USB Gamepad (X,Y,Z,Z rotation) + 2 sliders
 - 1000 points resolution
 - Plug-n-play
 - DirectX compatible

# Hardware

## Hardware Selection

  This sketch will work with any ATmega32U4 based *duino clone. This includes many boards such as the Leonardo or Pro Micro. **Do not flash non ATmega32U4 MCU's with this sketch. It won't do any harm but will not work.**
  
  A great modern choice for a micro controller is the [SparkFun Qwiic Pro Micro](https://www.sparkfun.com/products/15795). Its features inlcude:
 - USB-C for computer connection
 - Tiny form factor
 - Easy to solder pads


## Connections
 - RC PPM out <--> Digital Pin 4 of Arduino (INT1 pin)
 - RC GND  <--> Arduino GND

 This sketch uses Timer1 ICP1 interrupt so connection to a Digital pin that has INT1 (Normally pin 4) is mandatory.

### PPM Controller Connection
  The PPM pin on the transmitter is different with each brand or even model of transmitters. 

  #### *Trainer port of FlySky FS-i6 (Left) and Spektrum DX6 (Right)*
  ![Alt text](images/fs-i6_50.png?raw=true "Trainer port") ![Alt text](images/SPK-DX6.jpg?raw=true "Trainer port")
  

## Flashing
 - Open the sketch in Arduino IDE (RCTransmitter-USBGamepad.ino), choose Arduino board, upload.

## Driver Setup
  In Windows additional setup might be necessary to calibrate the minimum and maximum for each channel on the controller. To complete this calibration walk through the following steps:

  1. Open Windows Control Pannel Device Options
     - Windows key + R
     - Enter "control"
     - Click "View devices and printers"
  2. Open Game Contoller Settings
     - Locate your arduno in the device list (For the SparkFun Qwiic Pro Micro it would be SparkFun Pro Micro)
     - Right Click and select "Game Controller Settings"



 Tested with FlySky(Turnigy) FS-i6 transmitter, aerofly RC 7 simulator and FreeRider FPV.
 FlySky FS-i6 flashed with 10 channels firmware. On the trainer port it transmits only 6 channels.
 Note: consider turning on *Trainer mode* in the RC configuration menu. It wont stop RC from binding and transmitting, but can prevent your aircraft from flying away accidentally.
 You can remap AUX1 and AUX2 dials to switches in RC configuration.



# Sample screens
 Adding controller in aerofly RC 7
 ![Alt text](images/aerofly1.png?raw=true "Adding controller")

 Controller configuration in aerofly RC 7
 ![Alt text](images/aerofly2.png?raw=true "Controller configuration")

  Controller calibration in FreeRider FPV
 ![Alt text](images/freerider.png?raw=true "Controller calibration")