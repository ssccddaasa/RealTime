;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	Project:		Simple calculator-pic 
;	Source File Name:	Master.ASM		
;	Devised by:		1202121-Anas Hawamda
;				1200991-Saleh Khatib
;				1201146-Yasmeen Abukamal
;				1200060-Omar Masalmah		
;	Date:			15/2/2024
;	Status:			Final version
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

CURPOS	EQU	32	; Store the cursor position on the second row (any digit I want to incrimant) 
FTEN	EQU	33	; first number tenth part
FUNIT	EQU	34	; first number unit part
STEN	EQU	35	; second number tenth part
SUNIT	EQU	36	; second number unit part
TIM	EQU	37	; Timer to check if got 2 seconds
TEMP1	EQU	38	; temporary variable 1
TEMP2	EQU	39	; temporary variable 2
THU	EQU	40	; Thousand element
HUN	EQU	41	; Hundred element
TENS	EQU	42	; Tens element
UNIT	EQU	43	; Unit element
TEMP3	EQU	44	; temporary variable 3
TEMP4	EQU	45	; temporary variable 4
; PROGRAM BEGINS ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	ORG	0		; Default start address 
	NOP			; required for ICD mode
	
; Port & display setup.....................................
      BANKSEL	TRISD		; Select bank 1
      CLRF	TRISD		; Display port is output
      BANKSEL	TRISB
      MOVLW   	b'00000001'    ; Port B Direction Code
      MOVWF	TRISB
      BANKSEL	PORTD		; Select bank 0
      CLRF	PORTD		; Clear display outputs
      CALL	inid		; Initialise the display
      GOTO	Start

; MAIN LOOP ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Start
      CALL	WELCOME
Loop1      
      CALL	inid		; Initialise the display
      CALL	Time1
      CALL	Time2
      CALL	CALC
      CALL	RESULT
      goto  Loop1
      

WELCOME
      BCF	Select,RS	; set display command mode
      MOVLW	080		; code to home cursor
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	'W'		; load W code
      CALL	send		; and output
      MOVLW	'e'		; load e code
      CALL	send		; and output
      MOVLW	'l'		; load l code
      CALL	send		; and output
      MOVLW	'c'		; load c code
      CALL	send		; and output
      MOVLW	'o'		; load o code
      CALL	send		; and output
      MOVLW	'm'		; load m code
      CALL	send		; and output
      MOVLW	'e'		; load e code
      CALL	send		; and output
      MOVLW	' '		; load space code
      CALL	send		; and output
      MOVLW	't'		; load t code
      CALL	send		; and output
      MOVLW	'o'		; load o code
      CALL	send		; and output
      BCF	Select,RS	; set display command mode
      MOVLW	0xC0		; code to sceond row
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	'm'		; Load ASCII code for 'm'
      CALL	send		; and send character
      MOVLW	'u'		; Load ASCII code for 'u'
      CALL	send		; and send character
      MOVLW	'l'		; Load ASCII code for 'l'
      CALL	send		; and send character
      MOVLW	't'		; Load ASCII code for 't'
      CALL	send		; and send character
      MOVLW	'i'		; Load ASCII code for 'i'
      CALL	send		; and send character
      MOVLW	'p'		; Load ASCII code for 'p'
      CALL	send		; and send character
      MOVLW	'l'		; Load ASCII code for 'l'
      CALL	send		; and send character
      MOVLW	'i'		; Load ASCII code for 'i'
      CALL	send		; and send character
      MOVLW	'c'		; Load ASCII code for 'c'
      CALL	send		; and send character
      MOVLW	'a'		; Load ASCII code for 'a'
      CALL	send		; and send character
      MOVLW	't'		; Load ASCII code for 't'
      CALL	send		; and send character
      MOVLW	'i'		; Load ASCII code for 'i'
      CALL	send		; and send character
      MOVLW	'o'		; Load ASCII code for 'o'
      CALL	send		; and send character
      MOVLW	'n'		; Load ASCII code for 'n'
      CALL	send		; and send character
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;      
      MOVLW	D'250'		; delay for..
      CALL	xms		; ..250ms debounce
      MOVLW	D'250'		; delay for..
      CALL	xms		; ..250ms debounce
      MOVLW	D'250'		; delay for..
      CALL	xms		; ..250ms debounce
      MOVLW	D'250'		; delay for..
      CALL	xms		; ..250ms debounce
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;     
      BCF	Select,RS	; set display command mode
      MOVLW	0x08		; code to sceond row
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	D'100'		; delay for..
      CALL	xms		; ..100ms debounce
      BCF	Select,RS	; set display command mode
      MOVLW	0x0C		; code to sceond row
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	D'250'		; delay for..
      CALL	xms		; ..250ms debounce
      MOVLW	D'250'		; delay for..
      CALL	xms		; ..250ms debounce
      MOVLW	D'250'		; delay for..
      CALL	xms		; ..250ms debounce
      MOVLW	D'250'		; delay for..
      CALL	xms		; ..250ms debounce
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;      
      BCF	Select,RS	; set display command mode
      MOVLW	0x08		; code to sceond row
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	D'100'		; delay for..
      CALL	xms		; ..250ms debounce
      BCF	Select,RS	; set display command mode
      MOVLW	0x0C		; code to sceond row
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	D'250'		; delay for..
      CALL	xms		; ..250ms debounce
      MOVLW	D'250'		; delay for..
      CALL	xms		; ..250ms debounce
      MOVLW	D'250'		; delay for..
      CALL	xms		; ..250ms debounce
      MOVLW	D'250'		; delay for..
      CALL	xms		; ..250ms debounce
      BCF	Select,RS	; set display command mode
      MOVLW	0x08		; code to sceond row
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	D'100'		; delay for..
      CALL	xms		; ..250ms debounce
      BCF	Select,RS	; set display command mode
      MOVLW	0x0C		; code to sceond row
      CALL	send		; output it to display
      
      RETURN			; and finish
      
      
      
      
      
Number1
      CLRF	FUNIT		;set to zero
      CLRF	FTEN		;set to zero
      CLRF	SUNIT		;set to zero
      CLRF	STEN		;set to zero
      CLRF	THU		;set to zero
      CLRF	HUN		;set to zero
      CLRF	TENS		;set to zero
      CLRF	UNIT		;set to zero
      CLRF	CURPOS		;set to zero
      BCF	Select,RS	; set display command mode
      MOVLW	080		; code to home cursor
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	'N'		; load N code
      CALL	send		; and output
      MOVLW	'u'		; load u code
      CALL	send		; and output
      MOVLW	'm'		; load m code
      CALL	send		; and output
      MOVLW	'b'		; load b code
      CALL	send		; and output
      MOVLW	'e'		; load e code
      CALL	send		; and output
      MOVLW	'r'		; load r code
      CALL	send		; and output
      MOVLW	' '		; load SPACE code
      CALL	send		; and output
      MOVLW	'1'		; load 1 code
      CALL	send		; and output
      BCF	Select,RS	; set display command mode
      MOVLW	0xC0		; code to sceond row
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	'0'		; load 1 code
      CALL	send		; and output
      MOVLW	0X01		; cursor position
      MOVWF	CURPOS		; save the cursor position	
      RETURN

      
Number2
      BCF	Select,RS	; set display command mode
      MOVLW	080		; code to home cursor
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	'N'		; load N code
      CALL	send		; and output
      MOVLW	'u'		; load u code
      CALL	send		; and output
      MOVLW	'm'		; load m code
      CALL	send		; and output
      MOVLW	'b'		; load b code
      CALL	send		; and output
      MOVLW	'e'		; load e code
      CALL	send		; and output
      MOVLW	'r'		; load r code
      CALL	send		; and output
      MOVLW	' '		; load SPACE code
      CALL	send		; and output
      MOVLW	'2'		; load 2 code
      CALL	send		; and output
      BCF	Select,RS	; set display command mode
      MOVLW	0xC2		; code to sceond row third pos
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	'x'		; load x code
      CALL	send		; and output
      MOVLW	'0'		; load 0 code
      CALL	send		; and output
      MOVLW	D'3'		; cursor position
      MOVWF	CURPOS		; save the cursor position	
      RETURN
      
      

Clicked
      CLRF	TIM		;set timer to zero
      MOVF	CURPOS, W	;Load the cursor position
      ADDWF	PCL		;jump into table
      NOP
      GOTO	FirstTen	;set the the tenth part of the first number
      GOTO	FirstUnit	;set the the unit part of the first number
      GOTO	SceondTen	;set the the tenth part of the sceond number
      GOTO	SceondUnit	;set the the unit part of the sceond number
      GOTO	Loop		;Calculate the result
      
      
      

FirstTen
      INCF	FTEN		;Incremant the tenth part
      MOVF	FTEN, W		;Load the tenth part
      XORLW	D'10'		;Check if it reach 10
      BTFSC	STATUS, Z
      CLRF	FTEN		;put the tenth part to 0
      BCF	Select,RS	; set display command mode
      MOVLW	0xC0		; code to sceond row
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	0x30		; add 0x30 to send the char value
      ADDWF	FTEN, W		; put it into w reg and send it
      CALL	send		; and output
wait1 BTFSS	PORTB, 0	; check if the button still pressed.
      GOTO	wait1		; wait.
      GOTO	Floop		; return to loop in first number


FirstUnit
      INCF	FUNIT		;Incremant the tenth part
      MOVF	FUNIT, W		;Load the tenth part
      XORLW	D'10'		;Check if it reach 10
      BTFSC	STATUS, Z
      CLRF	FUNIT		;put the tenth part to 0
      BCF	Select,RS	; set display command mode
      MOVLW	0xC1		; code to sceond row
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	0x30		; add 0x30 to send the char value
      ADDWF	FUNIT, W	; put it into w reg and send it
      CALL	send		; and output
wait2 BTFSS	PORTB, 0	; check if the button still pressed.
      GOTO	wait2		; wait.
      GOTO	Floop		; return to loop in first number



SceondTen
      INCF	STEN		;Incremant the tenth part
      MOVF	STEN, W		;Load the tenth part
      XORLW	D'10'		;Check if it reach 10
      BTFSC	STATUS, Z
      CLRF	STEN		;put the tenth part to 0
      BCF	Select,RS	; set display command mode
      MOVLW	0xC3		; code to sceond row
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	0x30		; add 0x30 to send the char value
      ADDWF	STEN, W		; put it into w reg and send it
      CALL	send		; and output
wait3 BTFSS	PORTB, 0	; check if the button still pressed.
      GOTO	wait3		; wait.
      GOTO	Sloop		; return to loop in first number



SceondUnit
      INCF	SUNIT		;Incremant the tenth part
      MOVF	SUNIT, W		;Load the tenth part
      XORLW	D'10'		;Check if it reach 10
      BTFSC	STATUS, Z
      CLRF	SUNIT		;put the tenth part to 0
      BCF	Select,RS	; set display command mode
      MOVLW	0xC4		; code to sceond row
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	0x30		; add 0x30 to send the char value
      ADDWF	SUNIT, W		; put it into w reg and send it
      CALL	send		; and output
wait4 BTFSS	PORTB, 0	; check if the button still pressed.
      GOTO	wait4		; wait.
      GOTO	Sloop		; return to loop in first number      

Cler1
      INCF	CURPOS		;incremant the cursor position
      CLRF	TIM		;set timer to zero
      BCF	Select,RS	; set display command mode
      MOVLW	0xC1		; code to sceond row
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	0x30		; add 0x30 to send the char value
      ADDWF	FUNIT, W	; put it into w reg and send it
      CALL	send		; and output
      RETURN


Cler2
      INCF	CURPOS		;incremant the cursor position
      CLRF	TIM		;set timer to zero
      BCF	Select,RS	; set display command mode
      MOVLW	0xC4		; code to sceond row
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	0x30		; add 0x30 to send the char value
      ADDWF	SUNIT, W	; put it into w reg and send it
      CALL	send		; and output
      RETURN      
      
Time1
      CLRF	TIM		;set timer to zero
      CALL	Number1		; print the number1 on lcd
Floop
      INCF	TIM		; incremant the time with 10ms
      BTFSS	PORTB, 0	; check if the button pressed.
      GOTO	Clicked		; change the numbers.
      MOVF	TIM, W		;Load the time
      XORLW	D'200'		;Check if it reach 2S
      BTFSC	STATUS, Z
      CALL	Cler1		; clear time and incremant the cursor position
      MOVF	CURPOS, W	;Load the cursor position
      XORLW	D'3'		;Check if it reach 3
      BTFSC	STATUS, Z
      RETURN
      MOVLW	D'10'		; delay for..
      CALL	xms		; ..10ms debounce
      GOTO	Floop

      
      
      
      
      
      
Time2
      CLRF	TIM		;set timer to zero
      CALL	Number2		; print the number1 on lcd
Sloop
      INCF	TIM		; incremant the time with 10ms
      BTFSS	PORTB, 0	; check if the button pressed.
      GOTO	Clicked		; change the numbers.
      MOVF	TIM, W		;Load the time
      XORLW	D'200'		;Check if it reach 2S
      BTFSC	STATUS, Z
      CALL	Cler2		; clear time and incremant the cursor position
      MOVF	CURPOS, W	;Load the cursor position
      XORLW	D'5'		;Check if it reach 3
      BTFSC	STATUS, Z
      RETURN			;go to time 2.
      MOVLW	D'10'		; delay for..
      CALL	xms		; ..10ms debounce
      GOTO	Sloop      
      
      
      
CALC
      MOVLW	0xEF		; move 0xEF value to W
      MOVWF	TEMP4		; move W to TEMP4 to be send
      CALL	SENDPC		; call send by portc
      
      SWAPF	FTEN, W		; swap the first four bits with last four bits
      ADDWF	FUNIT, W	; add the unit part of the first number to ten part, so low bits is the unit part, and high is ten part
      
      MOVWF	TEMP4		; move W to TEMP4 to be send
      CALL	SENDPC		; call send by portc
      CALL	ACK		; call ACK to ensure that our value has been sent
      
      MOVF	SUNIT, W	; put unit digit of the second number on w
      MOVWF	TEMP4		; move W to TEMP4 to be send
      CALL	SENDPC		; call send by portc
      CALL	ACK		; call ACK to ensure that our value has been sent
      
      CALL	MULT		; call the multiplication.
      
      CALL	ACK2		; call ACK2 to start receive after the end of calculation  
      

      
      CALL	REVPC		; call the REVPC to receive the value from portc and   
      MOVWF	TEMP1		; move the value to TEMP1
      
      MOVLW	0xFF		; move 0xFF value to W
      MOVWF	TEMP4		; move W to TEMP4 to be send
      CALL	SENDPC		; call send by portc
      
      CALL	REVPC		; call the REVPC to receive the value from portc and   
      MOVWF	UNIT		; move the value to UNIT
      MOVLW	0xFF		; move 0xFF value to W
      MOVWF	TEMP4		; move W to TEMP4 to be send
      CALL	SENDPC		; call send by portc
      
      
      RETURN
      
      



SENDPC
      BANKSEL	TRISC		; Select bank 1
      MOVLW	0x00		; make portc as output
      MOVWF	TRISC
      BANKSEL	PORTC		; Select bank 0
      MOVF	TEMP4, W	; move from TEMP4 to W
      MOVWF	PORTC		; move from W to portc
      MOVLW	D'10'		; delay for..
      CALL	xms		; ..10ms debounce
      RETURN
      

ACK
      BANKSEL	TRISC		; Select bank 1
      MOVLW	0xFF		; make portc as input
      MOVWF	TRISC
      BANKSEL	PORTC		; Select bank 0
REV
      MOVF	PORTC, W	; move the content from portc to w
      XORLW	0xFF		; check if the content equal is 0xFF
      BTFSS	STATUS, Z
      GOTO	REV		; no, goto rev
      RETURN
      
      

ACK2
      BANKSEL	TRISC		; Select bank 1
      MOVLW	0xFF		; make portc as input
      MOVWF	TRISC
      BANKSEL	PORTC		; Select bank 0
REV2
      MOVF	PORTC, W	; move the content from portc to w
      XORLW	0xEF		; check if the content is 0xEF
      BTFSS	STATUS, Z
      GOTO	REV2		; no, goto rev2
      RETURN
      
      
      
      

REVPC
      BANKSEL	TRISC		; Select bank 1
      MOVLW	0xFF		; make portc as input
      MOVWF	TRISC
      BANKSEL	PORTC		; Select bank 0
REV3
      MOVF	PORTC, W	; move the content from portc to w
      XORLW	0xFF		; check if the content is 0xFF
      BTFSC	STATUS, Z
      GOTO	REV3		; Yes go to REV3
      
REV4
      MOVF	PORTC, W	; move the content from portc to w
      XORLW	0xEF		; check if the content is 0xEF
      BTFSC	STATUS, Z
      GOTO	REV4		; Yes go to REV3
      
      MOVF	PORTC, W	; move the content from portc to w
      RETURN      
      
      
MULT
      MOVF	STEN, W		; load the ten part of the sceond number
      CLRF	TEMP1		; clear temp1
ADD1
      ADDWF	TEMP1		; add sten to total
      DECFSZ	FUNIT		; funit times
      GOTO	ADD1		; if not zero add.
      
      
      
      MOVF	TEMP1, W	; move the result to w
      MOVWF	TENS		; put it in tens
      CLRF	HUN		; clear the hundred digit
      BSF	STATUS, C	; set C flag
      MOVLW	D'10'		; load 10
AGAIN1
      SUBWF	TENS		; sub 10 from result
      INCF	HUN		; inc hundred digit
      BTFSC	STATUS, C	; check if negative
      GOTO	AGAIN1		; no, keep going
      ADDWF	TENS		; yes, add 10 back 
      DECF	HUN		; ; inc hundred digit
     



      MOVF	STEN, W		; load the ten part of the sceond number
      CLRF	TEMP1		; clear temp1
ADD2
      ADDWF	TEMP1		; add sten to total
      DECFSZ	FTEN		; ften times
      GOTO	ADD2		; if not zero add.
      
      
      
      MOVF	TEMP1, W	; move the result to w
      MOVWF	TEMP2		; put it in HUN2
      CLRF	THU		; clear the hundred digit
      BSF	STATUS, C	; set C flag
      MOVLW	D'10'		; load 10
AGAIN2
      SUBWF	TEMP2		; sub 10 from result
      INCF	THU		; inc Thousand digit
      BTFSC	STATUS, C	; check if negative
      GOTO	AGAIN2		; no, keep going
      ADDWF	TEMP2		; yes, add 10 back 
      DECF	THU		; inc Thousand digit     
      
      
      
      
      MOVF	TEMP2, W	; move temp2 to W
      ADDWF	HUN		; add temp2 to hun
      BSF	STATUS, C	; set C flag
      MOVLW	D'10'		; load 10
AGAIN3
      SUBWF	HUN		; sub 10 from result
      INCF	THU		; inc Thousand digit
      BTFSC	STATUS, C	; check if negative
      GOTO	AGAIN3		; no, keep going
      ADDWF	HUN		; yes, add 10 back 
      DECF	THU		; inc Thousand digit

      RETURN





RESULT

      MOVF	TEMP1, W	; move temp1 to w
      ANDLW	0xF0		; get the high four bits
      MOVWF	TEMP2		; move w to temp2
      SWAPF	TEMP2, W	; swap the first four bits with last four bits
      MOVWF	TEMP2
      
      MOVF	TEMP1, W	; move temp1 to w
      ANDLW	0x0F		; get the low four bits
      MOVWF	TEMP3		; move w to temp3
      
      MOVF	TEMP3, W	; move temp2 to W
      ADDWF	TENS		; add temp2 to hun
      BSF	STATUS, C	; set C flag
      MOVLW	D'10'		; load 10
AGAIN4
      SUBWF	TENS		; sub 10 from result
      INCF	HUN		; inc Thousand digit
      BTFSC	STATUS, C	; check if negative
      GOTO	AGAIN4		; no, keep going
      ADDWF	TENS		; yes, add 10 back 
      DECF	HUN		; inc Thousand digit
      
      MOVF	TEMP2, W	; move temp2 to W
      ADDWF	HUN		; add temp2 to hun
      BSF	STATUS, C	; set C flag
      MOVLW	D'10'		; load 10
AGAIN5
      SUBWF	HUN		; sub 10 from result
      INCF	THU		; inc Thousand digit
      BTFSC	STATUS, C	; check if negative
      GOTO	AGAIN5		; no, keep going
      ADDWF	HUN		; yes, add 10 back 
      DECF	THU		; inc Thousand digit
      
      
      
      
      BCF	Select,RS	; set display command mode
      MOVLW	080		; code to home cursor
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	'R'		; load R code
      CALL	send		; and output
      MOVLW	'e'		; load e code
      CALL	send		; and output
      MOVLW	's'		; load s code
      CALL	send		; and output
      MOVLW	'u'		; load u code
      CALL	send		; and output
      MOVLW	'l'		; load l code
      CALL	send		; and output
      MOVLW	't'		; load t code
      CALL	send		; and output
      MOVLW	':'		; load : code
      CALL	send		; and output
      MOVLW	' '		; load space code
      CALL	send		; and output
      BCF	Select,RS	; set display command mode
      MOVLW	0xC5		; code to sceond row fifth pos
      CALL	send		; output it to display
      BSF	Select,RS	; and restore data mode
      MOVLW	'='		; load x code
      CALL	send		; and output
      
      MOVLW	0x30		; add 0x30 to send the char value
      ADDWF	THU, W		; put it into w reg and send it
      CALL	send		; and output
      
      MOVLW	0x30		; add 0x30 to send the char value
      ADDWF	HUN, W		; put it into w reg and send it
      CALL	send		; and output
      
      MOVLW	0x30		; add 0x30 to send the char value
      ADDWF	TENS, W		; put it into w reg and send it
      CALL	send		; and output
      
      MOVLW	0x30		; add 0x30 to send the char value
      ADDWF	UNIT, W		; put it into w reg and send it
      CALL	send		; and output

Loop2      
      BTFSS	PORTB, 0	; check if the button pressed.
      RETURN			; yes return
      GOTO	Loop2		; no go to loop2
      
      
      






      
      
Loop
      GOTO	Loop
      
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
