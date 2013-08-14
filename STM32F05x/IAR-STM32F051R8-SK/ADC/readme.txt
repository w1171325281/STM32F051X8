########################################################################
#
#                           ADC.eww
#
# $Revision: 60080 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the IAR-STM32F051R8-SK evaluation board. It shows
  basic use of a LCD driver and an UART.
   The example return received char back to terminal program (local echo) and
  show it on the LCD display.
 
   Terminal configuration:
     Word Length = 8 Bits
     One Stop Bit
     No parity
     BaudRate = 115200
     flow control: None

COMPATIBILITY
=============
   The ADC example project is compatible with IAR-STM32F051R8-SK
  evaluation board. By default, the project is configured to use the 
  J-Link JTAG/SWD interface.
        
GETTING STARTED
===============
  1) Build and download the program.
  2) Run the program.
