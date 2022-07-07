# Practices in BeagleBone Black 🤖👨‍💻
## Source code made for the "TPSE I" course, a basic Embedded System course.

<p align="center">
 <a href="#about">About</a> •
 <a href="#practices">Practices</a> •
 <a href="#technologies">Technologies</a> •
 <a href="#team">Team</a>
</p>

### About

This is the repository for the <b>TPSE</b> course, TÉCNICAS DE PROGRAMAÇÃO PARA SISTEMAS EMBARCADOS, or PROGRAMMING TECHNIQUES FOR EMBEDDED SYSTEMS, where the material produced during the discipline is located. This course is divided in two parts:

1) <b>Bare-metal Embedded Systems</b>: Basics in firmware programming, such as GPIO, IRQ and Timers (1 to 4).
2) <b>Embedded Linux</b>: Basics in the use of Linux in the BeagleBone, such as kernel, bootloader, firmware, etc...

### Practices

All the practices and projects made in the course will be available in the repository.

1) Basic Configuration of General Purpose I/O (GPIO), with LEDs (output) and buttons (input).
2) Configuration and Handling of External Interruptions (IRQ).
- Selection of the flashing sequence of the internal LEDs, by means of a button, configured to send a IRQ do CPU.
- <a href="https://www.youtube.com/watch?v=AKgDbaAUTqY&t=25s">IRQ Practice Video on YouTube (in Portuguese) </a>.
3) Counting Time with Timers (DMTIMER) configured for IRQ generation on overflow.
- Menu (on the serial monitor) for selecting a flashing time of one 8 LEDs, by the use of two buttons (with IRQ).
- <a href="https://www.youtube.com/watch?v=jdHQ_jgo2IM">Timers Practice Video on YouTube (in Portuguese) </a>.
4) Configuration of GPIO for Buttons and LEDs using modularized functions.
4) ~~Configuration and Handling External Interruptions using modularized functions~~.
6) ~~Counting Time with Timers using modularized functions~~.
7) ~~Getting data from a Matrix Keyboard using External Interrupts~~.
8) ~~Using a Liquid Crystal Display (LCD) with GPIO and modularized functions~~.
4) ~~Digital calculator system, with LCD display and matrix keyboard.~~

### Technologies

- C99 language.
- BeagleBone Black with Texas AM335x.

![BeagleBone Black](https://i0.wp.com/makezine.com/wp-content/uploads/2013/04/beagleboneblack01.png?fit=1349%2C900&ssl=1)

### Team

- Developer: Pedro M. Botelho
- Advisor: Prof. Dr.  Francisco Helder Candido
