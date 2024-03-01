;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	Project:			Simple calculator-pic 
;	Source File Name:	Master.ASM		
;	Devised by:			1202121-Anas Hawamda
;						1200991-Saleh Khatib
;						1201146-Yasmeen Abukamal
;						1200060-Omar Masalmah		
;	Date:				15/2/2024
;	Status:				Final version
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; 	We would like to build a simple calculator that is able to perform only multiplication
;	mathematical operations on integer numbers. We’ll assume each integer number will not
;	exceed 2 digits (e.g. maximum number is 99).
;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	PROCESSOR 16F877A
;	Clock = XT 4MHz, standard fuse settings
	__CONFIG 0x3731

;	LABEL EQUATES	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	#INCLUDE "P16F877A.INC" 	; standard labels 	

; GPR 70 - 75 allocated to included LCD display routine

count	EQU		32	; Counter for ADC setup delay
FTEN	EQU		33	; first number tenth part
FUNIT	EQU		34	; first number unit part
STEN	EQU		35	; second number tenth part
SUNIT	EQU		36	; second number unit part
TIM		EQU		37	; Timer to check if got 2 seconds
TEMP1	EQU		38	; temporary variable 1
TEMP2	EQU		39	; temporary variable 2
THU		EQU		40	; Thousand element
HUN		EQU		41	; Hundred element
TENS	EQU		42	; Tens element
UNIT	EQU		43	; Unit element
TEMP3	EQU		44	; temporary variable 3
TEMP4	EQU		45	; temporary variable 4
; PROGRAM BEGINS ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	ORG	0		; Default start address 
	NOP			; required for ICD mode
	
; Port & display setup.....................................
		BANKSEL		TRISC		; Select bank 1
      	MOVLW		0xFF
	  	MOVWF		TRISC
      	BANKSEL		PORTC		; Select bank 0
      	CALL		inid		; Initialise the display
      	GOTO		Loop

; MAIN LOOP ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Loop
      	CALL		WELCOME
      	goto  		Loop
      

WELCOME

	  	CALL		ACK2		; call ACK2 to start receive after the begin of calculation 

      
      	CALL		REVPC		; call the REVPC to receive the value from portc and  
      	MOVWF		TEMP1		; move the value to TEMP1
      
      	MOVLW		0xFF		; move 0xFF value to W
      	MOVWF		TEMP4		; move W to TEMP4 to be send
      	CALL		SENDPC		; call send by portc
      
      	CALL		REVPC		; call the REVPC to receive the value from portc and  
      	MOVWF		SUNIT		; move the value to TEMP1
      	MOVLW		0xFF		; move 0xFF value to W
      	MOVWF		TEMP4		; move W to TEMP4 to be send
      	CALL		SENDPC		; call send by portc

	  	MOVF		TEMP1, W	; move temp1 to w
      	ANDLW		0xF0		; get the high four bits
      	MOVWF		FTEN		; move w to ften
      	SWAPF		FTEN, W		; swap the first four bits with last four bits
      	MOVWF		FTEN
      
      	MOVF		TEMP1, W	; move temp1 to w
      	ANDLW		0x0F		; get the low four bits
      	MOVWF		FUNIT		; move w to funit

		CALL		MULT		; call the multiplication.


	  	MOVLW		0xEF		; move 0xEF value to W
      	MOVWF		TEMP4		; move W to TEMP4 to be send
      	CALL		SENDPC		; call send by portc


	  	SWAPF		HUN, W		; swap the first four bits with last four bits
      	ADDWF		TENS, W		; add the unit part of the first number to ten part, so low bits is the unit part, and high is ten part
	  	MOVWF		TEMP4		; move W to TEMP4 to be send
      	CALL		SENDPC		; call send by portc
      	CALL		ACK			; call ACK to ensure that our value has been sent
      
      	MOVF		UNIT, W		; put unit digit of the second number on w
      	MOVWF		TEMP4		; move W to TEMP4 to be send
      	CALL		SENDPC		; call send by portc
      	CALL		ACK			; call ACK to ensure that our value has been sent

		
		GOTO 		WELCOME


SENDPC
      	BANKSEL		TRISC		; Select bank 1
      	MOVLW		0x00		; make portc as output
      	MOVWF		TRISC
      	BANKSEL		PORTC		; Select bank 0
      	MOVF		TEMP4, W	; move from TEMP4 to W
      	MOVWF		PORTC		; move from W to portc
      	MOVLW		D'10'		; delay for..
      	CALL		xms			; ..10ms debounce
      	RETURN
      

ACK
      	BANKSEL		TRISC		; Select bank 1
      	MOVLW		0xFF		; make portc as input
      	MOVWF		TRISC
      	BANKSEL		PORTC		; Select bank 0
REV
      	MOVF		PORTC, W	; move the content from portc to w
      	XORLW		0xFF		; check if the content equal is 0xFF
      	BTFSS		STATUS, Z
      	GOTO		REV			; no, goto rev
      	RETURN
      
      

ACK2
      	BANKSEL		TRISC		; Select bank 1
      	MOVLW		0xFF		; make portc as input
      	MOVWF		TRISC
      	BANKSEL		PORTC		; Select bank 0
REV2
      	MOVF		PORTC, W	; move the content from portc to w
      	XORLW		0xEF		; check if the content is 0xEF
      	BTFSS		STATUS, Z
      	GOTO		REV2		; no, goto rev2
      	RETURN
      
      
      
      

REVPC
      	BANKSEL		TRISC		; Select bank 1
      	MOVLW		0xFF		; make portc as input
      	MOVWF		TRISC
      	BANKSEL		PORTC		; Select bank 0
REV3
      	MOVF		PORTC, W	; move the content from portc to w
      	XORLW		0xFF		; check if the content is 0xFF
      	BTFSC		STATUS, Z
      	GOTO		REV3		; Yes go to REV3
      
REV4
      	MOVF		PORTC, W	; move the content from portc to w
      	XORLW		0xEF		; check if the content is 0xEF
      	BTFSC		STATUS, Z
      	GOTO		REV4		; Yes go to REV3
      
      	MOVF		PORTC, W	; move the content from portc to w
      	RETURN  






        

MULT
      	MOVF		SUNIT, W	; load the ten part of the sceond number
      	CLRF		TEMP1		; clear temp1
ADD1
      	ADDWF		TEMP1		; add sten to total
      	DECFSZ		FUNIT		; funit times
      	GOTO		ADD1		; if not zero add.
      
      
      
      	MOVF		TEMP1, W	; move the result to w
      	MOVWF		UNIT		; put it in unit
      	CLRF		TENS		; clear the ten digit
      	BSF			STATUS, C	; set C flag
      	MOVLW		D'10'		; load 10
AGAIN1
      	SUBWF		UNIT		; sub 10 from result
      	INCF		TENS		; inc ten digit
      	BTFSC		STATUS, C	; check if negative
      	GOTO		AGAIN1		; no, keep going
      	ADDWF		UNIT		; yes, add 10 back 
      	DECF		TENS		; ; inc ten digit
     



      	MOVF		SUNIT, W	; load the ten part of the sceond number
      	CLRF		TEMP1		; clear temp1
ADD2
      	ADDWF		TEMP1		; add sten to total
      	DECFSZ 		FTEN		; ften times
      	GOTO		ADD2		; if not zero add.
      
      
      
      	MOVF		TEMP1, W	; move the result to w
      	MOVWF		TEMP2		; put it in ten2
      	CLRF		HUN			; clear the hundred digit
      	BSF			STATUS, C	; set C flag
      	MOVLW		D'10'		; load 10
AGAIN2
      	SUBWF		TEMP2		; sub 10 from result
      	INCF		HUN			; inc hundred digit
      	BTFSC		STATUS, C	; check if negative
      	GOTO		AGAIN2		; no, keep going
      	ADDWF		TEMP2		; yes, add 10 back 
      	DECF		HUN			; inc hundred digit     
      
      
      
      
      	MOVF		TEMP2, W	; move temp2 to W
      	ADDWF		TENS		; add temp2 to tens
      	BSF			STATUS, C	; set C flag
      	MOVLW		D'10'		; load 10
AGAIN3
      	SUBWF		TENS		; sub 10 from result
      	INCF		HUN			; inc hundred digit
      	BTFSC		STATUS, C	; check if negative
      	GOTO		AGAIN3		; no, keep going
      	ADDWF		TENS		; yes, add 10 back 
      	DECF		HUN			; inc hundred digit

      	RETURN

      

;====================================================================

; INCLUDED ROUTINES ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Include LCD driver routines
;
	#INCLUDE "LCDIS.INC"
;	Contains routines:
;	inid:	Initialises display
;	onems:	1 ms delay
;	xms:	X ms delay
;		Receives X in W
;	send:	Sends a character to display
;		Receives: Control code in W (Select,RS=0)
;		  	  ASCII character code in W (RS=1)	

	END	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
