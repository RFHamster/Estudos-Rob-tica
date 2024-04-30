;Code from http://www.rjhcoding.com/avr-asm-led-blink.php
;Modified by Diego Molinos 	
;https://ww1.microchip.com/downloads/en/devicedoc/atmel-0856-avr-instruction-set-manual.pdf
	
	
	.include "m328pdef.inc"

	.def	mask 	= r16		; mask register - .def - Set a symbolic name on a register
	.def	ledR 	= r17		; led register - 
	.def	oLoopR 	= r18		; outer loop register
	.def	iLoopRl = r24		; inner loop register low
	.def	iLoopRh = r25		; inner loop register high

data:
	.equ	oVal 	= 71		; outer loop value - .equ Set a symbol equal to an expression. 
								; The EQU directive assigns a value to a label.
	.equ	iVal 	= 28168		; inner loop value

setup:
	.cseg						; directive defines the start of a Code Segment
	.org	0x00				;The AVR assembler directive .org is used to specify 
								; the starting address of a code segment.
	clr	ledR					; clear led register
	ldi mask, 0b00000001		; load 00000001 into mask register
	out	DDRB, mask				; set PINB0 to output

start:	
	eor	ledR,mask				; toggle PINB0 in led register
	out	PORTB,ledR				; write led register to PORTB

	ldi	oLoopR,oVal				; initialize outer loop count

oLoop:	
	ldi	iLoopRl,LOW(iVal)		; intialize inner loop count in inner
	ldi	iLoopRh,HIGH(iVal)		; loop high and low registers

iLoop:	
	sbiw	iLoopRl, 1			; decrement inner loop registers - sbiw - Subtract constant from register
	brne	iLoop				; branch to iLoop if iLoop registers != 0 								; this instruction will test the Z and branches relative to PC (Program counter)

	dec		oLoopR				; decrement outer loop register
	brne	oLoop				; branch to oLoop if outer loop register != 0

	rjmp	start				; jump back to start