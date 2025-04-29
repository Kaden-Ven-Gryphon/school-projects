@ File: Lab4Beaver.s
@ Author: 	Kaden Beaver
@		kb0125@uah.edu
@		CS 309-01 2020 Fall
@Purpose:get a number 1-100 from user and out put the sum of 1 to n
@
@
@ Commands to assemble
@
@ as -o Lab4Beaver.o Lab4Beaver.s
@ gcc -o Lab4Beaver Lab4Beaver.o
@ ./Lab4Beaver
@ gbd --args ./Lab4Beaver
@
@
@
@***********************START***********************


.equ READERROR, 0

.global main

main:

prompt:		@PROMPT**********************

	ldr r0, = strInputPrompt1	@load the adress of the int prompt
	bl  printf			@call printf

		@prompt___________________


get_input1:		@GET_INPUT1*******************
@get the input from the user
@checks for scan errors
@compares the entered number to 1-100


	ldr r0, =numInputPattern	@load the pattern for int
	ldr r1, =intInput		@load the memory location for the data to be scaned to

	bl  scanf			@call scanf
	cmp r0, #READERROR		@check for read error
	beq readErrorInt		@if read error jump to READERRORINT
	ldr r1, =intInput		@load what was scaned to r1
	ldr r1, [r1]			@get the acutal number loaded into r1

	cmp r1, #100			@compare to 100
	bgt myexit			@if greater quit	
	cmp r1, #1			@compare to 1
	blt myexit			@if less quit

	ldr r7, =intInput		@load the user inputed value
	ldr r7, [r7]			@r7 will hold the value of the end of the loop

	ldr r0, =strRespons		@load the response, it needs two ints
	mov r1, r7			@mov the user input for the first int
	mov r2, r7			@mov the user input for the second int

	bl  printf			@print the users input and 1-%d

		@get_input1__________________


header:			@HEADER************************
@print out the header of
@ the table and set
@ up the regs for the loop

	ldr r0, =headerFormat		@load the string with formaters for the header
	ldr r1, =strNumberHeader	@load the string for Number, the first header
	ldr r2, =strSumHeader		@load the string for Sum, the second header
	bl  printf			@print out the formated string with the headers in it

	mov r5, #1			@r5 keeps track of the number, starts at 1
	mov r6, #1			@r6 keeps track of the sum, starts at 1
	
		@header_______________________

loop:			@LOOP*************************
@this will print out the lines
@of the table and update the
@counters r5, and r6
@it will also check if the loop
@needs to repeat

	ldr r0, =tableFormat		@load the string with the two int formaters
	mov r1, r5			@load the curent number for the first formater
	mov r2, r6			@load the curent sum for the second formater

	bl  printf			@print out the table line with values
loopAP:	@break point after printf
	add r5, r5, #1			@add one to the number, this acts as the loops counter
	add r6, r6, r5			@add the new number to the runing sum to update the sum

	cmp r5, r7			@compare the curent number with the user inputed number
	ble loop			@if the curent number is less than the user number loop again

	b  myexit			@otherwise exit

		@loop_________________________


readErrorInt:		@READERRORINT******************
@ got a read error from the scanf call
@ going to clear the buffer and send
@ the data to a junk memory location

	ldr r0, =strInputPattern	@load the filter to clear the buffer
	ldr r1, =strInputError		@load the adress to dump the junk
	bl  scanf			@call the scanf function

	b   myexit			@jump back to PROMPT

		@readerror______________________




myexit:			@MYEXIT**********************
@ end of the code use
@ system call to force
@ exit and return control
@ to the os

	mov r7, #0x01			@load the exit command into r7
	svc 0				@system call
		@myexit_________________________



.data			@DATA***********************

		@Strings for prompts and outputs****
.balign 4
strInputPrompt1: .asciz "Input a number 1-100:> \n"

.balign 4
strNumberHeader: .asciz "Number"

.balign 4
strSumHeader: .asciz "Sum"

.balign 4
strRespons: .asciz "You entered: %d the sum of 1-%d is:\n"


		@Scanf patters***********************
.balign 4
numInputPattern: .asciz "%d"

		@Printf formats**********************
.balign 4
headerFormat: .asciz "%10s %10s\n"

.balign 4
tableFormat: .asciz "%10d %10d\n"

		@input varibles**********************
.balign 4
intInput: .word 0

		@To clear the buffer*****************
.balign 4
strInputPattern: .asciz "[^\n]"

.balign 4
strInputError: .skip 100*4


		@uses printf, and scanf

.global printf

.global scanf
