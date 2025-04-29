@ Filename	Beaver.s
@ Author: 	Kaden Beaver
@ Purpose:	Act as a vending machine, take in coin, keep track of envetory, return change
@ Date:		03.21.2021
@
@
@
@ Use these command to assemble, link, run and debug
@ 	as -o Beaver.o Beaver.s
@	gcc -o Beaver Beaver.o
@	./Beaver
@	gdb -- args ./Beaver
@
@
@
@
@

.equ READERROR, 0				@scanf readerror

.global main

@_______________________________________________MAIN_______________________________________
main:						@start of main


	mov	r5, #0			@ this will keep track of which prompt it is on
	mov	r6, #2			@ the number of cokes
	mov 	r7, #2			@ the number of sprites
	mov	r8, #2			@ the number of dr peppers
	mov	r9, #2			@ the number of coke zeros
	mov	r10, #0			@ this will keep track of the running total of inputed money

welcome:

	ldr	r0, =strWelcome
	bl	printf


@________________________________________
prompt1:				@
	mov	r5, #0			@ set the prompt coutner to zero
					@
	ldr	r0, =strCoinPrompt	@ print the coin prompt
	bl	printf			@
					@
prompt1a:				@
					@
	ldr	r0, =charScan		@ load the char scan templet
	ldr	r1, =charInput		@ load the adress to save the scan to
	bl	scanf			@ scanf
					@
	cmp	r0, #READERROR		@ check for an error in scaning
	beq	readError		@ go handle it
					@
	ldr	r0, =charInput		@ load the adress of the input
	ldr	r0, [r0]		@ load the value of the input
					@
	bl	charToCents		@ call the funtion to decode the char into a cent value
					@
	cmp	r0, #-3			@ I hate how scanf and \n interact I do not think that the way we clear the buffer with %[^\n] works when trying to scan for chars
	beq	prompt1a		@ if there is a \n that gets pick up try again
					@
	cmp	r0, #-4			@ the user wants to cancel
	beq	cancel			@ go return their change
					@
	cmp	r0, #-2			@ if the funtion returns -2 the char was invald
	beq	invaldInput		@ ask the user for anouth input
					@
	cmp	r0, #-1			@ if the funtion returns -1 the inventory code was called
	beq	inventroy		@ go print the inventroy
					@
					@
	add	r10, r10, r0		@ other wise add the returned value to the running total
					@
	mov	r1, r10			@ move the running total to r1
	ldr	r0, =strTotal		@ load the sting to r0
	bl	printf			@ print the total inputed cents
					@
	cmp	r10, #55		@ comapre the running total to 55 cents
	bge	prompt2			@ go the next prompt
					@
					@
	b	prompt1			@ other wise ask for more change
@________________________________________


@ this will get the users drink selection
@________________________________________
prompt2:				@
	mov	r5, #1			@ set the prompt coutner to 1
					@
	ldr	r0, =strDrinkPrompt	@ load the drink prompt
	bl	printf			@ print the prompt
					@
prompt2a:				@ incase a \n gets picked up
	ldr	r0, =charScan		@ load the scanf filter
	ldr	r1, =charInput		@ load the adress of input var
	bl	scanf			@ scan
					@
	cmp	r0, #READERROR		@ check for read error
	beq	readError		@ go handle it
					@
	ldr	r1, =charInput		@ get the adress of the input
	ldr	r1, [r1]		@ get the value of the input
	mov	r11, r1			@ save the value to r11 for latter use
					@
	cmp	r1, #10			@ was it a \n
	beq	prompt2a		@ try again
					@
	cmp	r1, #67			@ was it C
	ldreq	r4, =strCoke		@ set the string to Coke
	cmpeq	r6, #0			@ check if out of stock
	beq	outOfStock		@ go to out of stock message
	cmp	r11, #67		@
	beq	prompt3			@ goto prompt 3
					@
	cmp	r1, #83			@ was it S
	ldreq	r4, =strSprite		@ set teh string to Sprite
	cmpeq	r7, #0			@ cech if out of stock
	beq	outOfStock		@ out of stock message
	cmp	r11, #83		@
	beq	prompt3			@ go to prompt 3
					@
	cmp	r1, #80			@ was it P
	ldreq	r4, =strPepper		@ set the spring to Dr. Pepper
	cmpeq	r8, #0			@ check if out of stock
	beq	outOfStock		@ print out of stock message
	cmp	r11, #80		@
	beq	prompt3			@ got to parompt 3
					@
	cmp	r1, #90			@ was it Z
	ldreq	r4, =strCokeZero	@ set the string to Coke Zero
	cmpeq	r9, #0			@ check if out of stock
	beq	outOfStock		@ print out of stock message
	cmp	r11, #90		@
	beq	prompt3			@ go to prompt 3
					@
	cmp	r1, #88			@ was it X
	beq	cancel			@ go return change
					@
	b	invaldInput		@ other wise it was an invald input
@________________________________________



@ get a y or n from user to comfirm selection
@________________________________________
prompt3:				@
	mov	r5, #2			@ set the prompt coutner to 2
					@
	mov	r1, r4			@ load the drink into the string
	ldr	r0, =strConfirm		@ print the compermation message
	bl	printf			@
					@
prompt3a:				@ in case a \n gets picked up
	ldr	r0, =charScan		@ load the char scan filter
	ldr	r1, =charInput		@ load the adress of var
	bl	scanf			@ scanf
					@
	cmp	r0, #READERROR		@ check for read error
	beq	readError		@ go handle it
					@
	ldr	r1, =charInput		@ load the adress of input
	ldr	r1, [r1]		@ load the value of input
					@
	cmp	r1, #10			@ if it was \n try again
	beq	prompt3a		@
					@
	cmp	r1, #78			@ if it was N
	beq	prompt2			@ go back to drink selection
					@
	cmp	r1, #89			@ if it was Y
	beq	giveDrink		@ go give the drink and change
					@
	b	invaldInput		@ other wise try again invald char
@________________________________________


@ give the user the drink and change, update inventory
@ check if out of stock for all
@________________________________________
giveDrink:				@
					@
	ldr	r0, =strChange		@ load the message
	mov	r1, r4			@ load the trpy of drink
	sub	r10, r10, #55		@ subtract cost to get change
	mov	r2, r10			@ move change to meassage
	bl	printf			@ print message
					@
	mov	r10, #0			@ reset runing total
					@
	cmp	r11, #67		@ if the drink was coke
	subeq	r6, r6, #1		@ subtract one from coke inventroy
					@
	cmp	r11, #83		@ if the drink was sprite
	subeq	r7, r7, #1		@ subtract one from sprite inventroy
					@
	cmp	r11, #80		@ if the drink of Pepper
	subeq	r8, r8, #1		@ subtract one from pepper inentroy
					@
	cmp	r11, #90		@ if the dirnk was coke zero
	subeq	r9, r9, #1		@ subtract one fomr coke zero inentory
					@
	cmp	r6, #0			@ ceck if coke is out
	cmpeq	r7, #0			@ check if sprite is out
	cmpeq	r8, #0			@ check if pepper is out
	cmpeq	r9, #0			@ check if coke zero is out
	beq 	myExit			@ then the machine is out of all stock EXIT PROGRAM
					@
	b	welcome			@ else start again
@________________________________________

@________________________________________
outOfStock:				@
					@
	ldr	r0, =strOutOfStock	@ print out of stock message
	bl	printf			@
					@ go back to prompt 2
	b	prompt2			@
@________________________________________

@________________________________________
cancel:					@
					@
	ldr	r0, =strReturnChange	@ return the inputed change
	mov 	r1, r10			@
	mov	r10, #0			@ set the running total to zero
	bl	printf			@
					@
	b	welcome			@ go back to the begining
@________________________________________






@ this will print the inventory then go back to prompt 1
@________________________________________
inventroy:				@
					@
	ldr 	r0, =strInventory	@ print the coke inentory
	ldr	r1, =strCoke		@
	mov	r2, r6			@
	bl	printf			@
					@
	ldr	r0, =strInventory	@ print the sprite inventory
	ldr	r1, =strSprite		@
	mov	r2, r7			@
	bl	printf			@
					@
	ldr	r0, =strInventory	@ print the dr pepper inentory
	ldr 	r1, =strPepper		@
	mov	r2, r8			@
	bl	printf			@
					@
	ldr	r0, =strInventory	@ print the coke zero inventory
	ldr	r1, =strCokeZero	@
	mov	r2, r9			@
	bl	printf			@
					@
	b	prompt1			@ go back to prompt 1
@________________________________________


@ anytime an input has a unwatned value this will be called
@ will use r5 to return to the right prompt
@________________________________________
invaldInput:				@
					@
	ldr	r0, =strInputInvald	@ print error mesage
	bl	printf			@
					@
	cmp	r5, #0			@ go back to the curent prompt
	beq	prompt1			@
	cmp	r5, #1			@
	beq	prompt2			@
	cmp	r5, #2			@
	beq	prompt3			@
					@
@________________________________________



@ any time there is a read error with scanf this will be called
@________________________________________
readError:				@
					@
	ldr	r0, =clearScan		@ clear the buffer, this does not seem to work with chars
	ldr	r1, =strInputError	@
	bl	scanf			@
					@
	b	invaldInput		@ go print error meeage
					@
@________________________________________



@________________________________
myExit:				@ exit program return control to os
				@
	mov	r7, #0x01	@ SVC call to exit
	svc	0		@
				@
@_______________________________@



@ simple function takes in a char and returns the corespnding cent value
@ r0 is input
@ r0 is return value
@ return -1 is the inventroy code
@ return -2 means the char was not reconized
@ return -3 means that a pesky \n was picked up
@ return -4 means that the user wants to cancel
@________________________________
charToCents:			@
				@
	cmp	r0, #78		@ was the input N
	moveq	r0, #5		@ return 5 cents
	bxeq	lr		@
				@
	cmp	r0, #68		@ was the input D
	moveq	r0, #10		@ return 10 cents
	bxeq	lr		@
				@
	cmp	r0, #81		@ was the input Q
	moveq	r0, #25		@ return 25 cents
	bxeq	lr		@
				@
	cmp	r0, #66		@ was the input B
	moveq	r0, #100	@ return 100 cents
	bxeq	lr		@
				@
	cmp	r0, #80		@ was the input P
	moveq	r0, #-1		@ return -1 code
	bxeq	lr		@
				@
	cmp	r0, #10		@ was the input a \n
	moveq	r0, #-3		@ return -3 code
	bxeq	lr		@
				@
	cmp	r0, #88		@ the input was X
	moveq	r0, #-4		@ return -4
	bxeq	lr		@
				@
	mov	r0, #-2		@ else the input was not recongnized
	bx	lr		@ return -2 code
				@
@________________________________



.data


@________________________________________________STRINGS____________________________________
.balign 4
strWelcome: .asciz "Welcome to the vending machine.\nCost of Coke, Sprite, Dr.Pepper, and Coke Zero is 55 cents.\nInsert a penny (P) to check inventory.\n\n"

.balign 4
strCoinPrompt: .asciz "\nEnter money: nickel (N), dime (D), quarter (Q), and one dollar bill (B), or (X) to cancel.\n"

.balign 4
strTotal: .asciz "Total is %d cents.\n\n"

.balign 4
strDrinkPrompt: .asciz "Make selection:\nCoke (C), Sprite (S), Dr. Pepper (P), or Coke Zero (Z), (X) to cancel and return money\n"

.balign 4
strConfirm: .asciz "Selection is %s. Is this OK? (Y or N).\n"

.balign 4
strChange: .asciz "\nA %s has been dispensed with %d cents change.\n"

.balign 4
strReturnChange: .asciz "%d cents have been returned\n"

.balign 4
strOutOfStock: .asciz "That drink is out of stock please select anouther.\n"

.balign 4
strInputInvald: .asciz "Invald input, please try again.\n"

.balign 4
strInventory: .asciz "%10s: %3d\n"

.balign 4
strCoke: .asciz "Coke"

.balign 4
strSprite: .asciz "Sprite"

.balign 4
strPepper: .asciz "Dr. Pepper"

.balign 4
strCokeZero: .asciz "Coke Zero"

.balign 4
strNothing: .asciz "Nothing"


@________________________________________________SCANF PATTERNS______________________________
.balign 4
charScan: .asciz "%c"

.balign 4
clearScan: .asciz "%[^\n]"

.balign 4
strInputError: .skip 100*4

@________________________________________________VARIBLES______________________________________
.balign 4
charInput: .word 0


.global printf
.global scanf
