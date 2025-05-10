# DemoUSART

## Overview

**DemoUSART** is a simple demonstration project showcasing UART (Universal Asynchronous Receiver/Transmitter) communication on a microcontroller. This example is designed for embedded systems students and professionals to illustrate how serial communication can be implemented using C and MPLAB X with XC8.

## Features

- Basic USART initialization and configuration  
- Continuous data transmission via UART  
- Designed for PIC microcontrollers (e.g., PIC16F or PIC18F series)  
- Easily extensible for full-duplex or interrupt-driven communication  

## Project Structure

DemoUSART-main/  
├── main.c                   # Core source code for USART functionality  
├── Makefile                # Build script  
├── build/                  # Compiler-generated object files  
├── debug/                  # Debug trace files  
├── dist/                   # Distribution build outputs  

## Requirements

- **Microcontroller**: Compatible with the PIC series  
- **Toolchain**: MPLAB X IDE with XC8 compiler  
- **Serial Monitor**: (e.g., PuTTY, Tera Term) for viewing UART output  

## How to Build

1. Open the project in MPLAB X IDE.  
2. Ensure the correct device and tool are selected.  
3. Build the project using the provided Makefile or IDE's build system.  
4. Flash the binary to your target PIC microcontroller.  
5. Connect a USB-to-Serial adapter and monitor the output at the specified baud rate.  

## Use in CV/Portfolio

This project demonstrates:
- Familiarity with embedded C programming  
- Ability to configure microcontroller peripherals  
- Practical use of UART communication in embedded systems  
- Experience with MPLAB X and XC8 toolchain  

## License

This example is provided for educational and demonstration purposes.
