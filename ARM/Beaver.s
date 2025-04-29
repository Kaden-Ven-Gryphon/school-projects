@ File:		Beaver.s
@ Author:	Kaden Beaver
@ Email:	kb0125@uah.edu
@ Section:	CS413-01 Spring 2021
@ Purpose:	Take a user input and print hellow world the number of times equal to the input
@
@
@ Use these commands to assemble. link. run and debug the program
@
@  as -o Beaver.o Beaver.s
@  gcc -o Beaver Beaver.o
@  ./Beaver ;echo$?
@  gbd --args ./Beaver
@
@
@
@

.equ READERROR, 0			@user to check scanf read error


.global main
main:

	ldr r0, =strWelcome		@load adrees of welcom message
	bl  printf			@call printf

@__________________________________________________PROMPT________________________
prompt:
@ ask the user to enter a number

	ldr r0, =strInputPrompt		@load adress of prompt message
	bl  printf			@call printf

@###################################################prompt#######################

@__________________________________________________GETINPUT______________________
getInput:
@ use scanf to get input and check if it is valid

	ldr r0, =numInputPattern	@load adress of scanf formater
	ldr r1, =intInput		@load adress of varible
	bl  scanf			@call scanf
	cmp r0, #READERROR		@check for read error
	beq readError			@if there is an error jump to error function
	ldr r1, =intInput		@load the adress of varible in to r1
	ldr r4, [r1]			@load the value of varible in to r4 to use as counter

@##################################################getInput######################

@__________________________________________________CHECKINPUT____________________
checkInput:
@ check that the input is between 0 and 10

	cmp r4, #10			@compare the user input to 10
	bgt outOfRange			@if greater jumpt to OutOfRange
	cmp r4, #0			@compare the user input to 0
	blt outOfRange			@if less jump to OutOfRange

@##################################################checkInput####################


@__________________________________________________LOOP__________________________
loop:
@ loop through the number of times the user inputed and print "Hollow World!"

	cmp r4, #0			@compare the counter to 0
	ble myExit			@if lessthan or equal to 0 the loop is over jump to exit

	ldr r0, =strHellow		@load the adress for Hollow world
	bl  printf			@call printf

	sub r4, r4, #1			@remove 1 from the counter

	b   loop			@jump to start of loop

@__________________________________________________OUTOFRANGE____________________
outOfRange:
@ the input was a number but out of the range

	ldr r0, =strOutOfRange		@load the error message
	bl  printf			@print the message
	b   prompt			@jump back to prompt to try again

@__________________________________________________READERROR_____________________
readError:
@ the scanf returned an error so the buffer needs to be cleared and the prompt resent

	ldr r0, =strClear		@load the formater to clear the buffer
	ldr r1, =strInputError		@load the "adress" to dump the junk into
	bl  scanf			@call scanf

	b   prompt			@jump back to the prompt to try again

@###################################################readError####################



@___________________________________________________MYEXIT_______________________
myExit:
@ Force exit and return control to the OS

	mov r7, #0x01		@load the exit comand
	svc 0			@make the system call

@###################################################myExit#######################


@__________________________________________________DATA__________________________
.data

.balign 4
strWelcome: .asciz "Welcome.\n"

.balign 4
strInputPrompt: .asciz "Input a number 0-10 :>\n"

.balign 4
strHellow: .asciz "Hellow World!\n"

.balign 4
strOutOfRange: .asciz "Out of range (0-10)\n"


@##########  Scanf Formaters  #####################

.balign 4
numInputPattern: .asciz "%d" 		@read an int

.balign 4
strClear: .asciz "%[^\n]" 		@to clear the input buffer

.balign 4
strInputError: .skip 100*4		@used to clear buffer

@##########  Varibles  ##############################

.balign 4
intInput: .word 0 			@to store user input

@##########  C Functions  ###########################

.global printf
.global scanf
