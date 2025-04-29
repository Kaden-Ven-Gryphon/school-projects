@ Filename:		Beaver.s
@ Author: 		Kaden Beaver
@ Purpose: 		Make a four function binary logic calculator that uses bl functions and passes operands through the the stacl
@ Date:			02.21.2021
@
@
@ Use these commands to assemble, link, run and debug
@	as -o Beaver.o Beaver.s
@	gcc -o Beaver Beaver.o
@	./Beaver
@	gdb -- args ./Beaver
@
@			EDITED TO RUN IN THUMB     COMMENTS IN CAPS ARE EDITES

.equ READERROR, 0

.global main

@____________________________________________MAIN___________________________________________

main:						@start of main

	ldr 	r0, =welcome +1			@SWTICH TO THUMB MODE
	bx	r0
.code 16


welcome:					@welcom label

	ldr	r0, =strWelcome			@load adress of welcome message
	bl	printf				@call printf

	mov 	r4, #1				@counter to keep track of which prompt to loop to

prompt1:
	mov	r4, #1				@NEEDED FOR IF THE PROGRAM LOOPS    THIS FIXED A POSIBLE SEGMITATION FAULT

	ldr	r0, =strPrompt1			@load the prompt for a hex
	bl	printf				@call printf

	ldr	r0, =hexScan			@load the scan modifier for hexadecimal
	ldr	r1, =hexInput			@load the adress of the hex
	bl	scanf				@call scanf

	cmp	r0, #READERROR			@check for a read error
	beq	readError			@go clear the buffer

	ldr 	r1, =hexInput			@load the adress of the user input
	ldr 	r1, [r1]			@load the value of the user input
	push	{r1}				@put the user input on the stack

	mov	r4, #2				@up the counter for the prompt

prompt2:

	ldr	r0, =strPrompt2			@load the prompt for the operation
	bl	printf				@call printf

	ldr	r0, =intScan			@load scan modifier for int
	ldr	r1, =intInput			@load adress of varible
	bl	scanf				@call scanf

	cmp	r0, #READERROR			@check for error
	beq	readError			@go clear the buffer

	ldr	r0, =intInput			@load adress
	ldr	r0, [r0]			@load the value

						@HAD TO REMOVE THE CONDITINAL EXICUTION HERE
	cmp	r0, #1				@is the user input greater than 0
	blt	outOfRange
	cmp	r0, #4				@is the user input less than 5
	bgt	outOfRange
	mov 	r4 ,#3				@add 1 to the counter for prompts
	b	prompt3				@go to prompt 3

outOfRange:

	ldr	r0, =strInputError		@load the error message
	bl	printf				@print the message
	mov	r4, #2
	b	prompt2				@redue prompt 2



prompt3:

	ldr	r0, =strPrompt1			@load the prompt for a hexadecimal
	bl	printf				@call printf

	ldr	r0, =hexScan			@load the scan modifer for hexadcimal
	ldr	r1, =hexInput			@load adress of input varible
	bl	scanf				@call scanf

	cmp	r0, #READERROR			@check for error
	beq	readError			@go clear the buffer

	ldr 	r1, =hexInput			@load the adress of varible
	ldr	r1, [r1]			@load the adress of the input
	push	{r1}				@push the user input onto the stack


runOperation:

	ldr	r4, =intInput			@load the user input for operation
	ldr	r4, [r4]			@load the value

						@ALSO HAD TO REWRITE THIS PART TO REMOVE CONDTINAL EXICUTION
	cmp	r4, #1				@if the input was a 1
	bne	check2
	bl	functionAND			@call the AND function
	b	printResult			@THIS IS NEEDED TO AVOIF CALLING THE REST OF THE FUNCTIONS AFTER FINSHING THE CHOSEN FUNCTION IT TOOK OVER AN HOUR TO FIGURE OUT WHAT WAS WRONG

check2:
	cmp	r4, #2				@if the input was a 2
	bne	check3
	bl	functionORR			@call the ORR function
	b	printResult			@TO AVOID REPEATES

check3:
	cmp	r4, #3				@if the input was a 3
	bne	check4
	bl	functionEOR			@call the EOR function
	b	printResult			@TO AVOID REPEATES

check4:
	cmp	r4, #4				@if the input was a 4
	bne	outOfRange
	bl	functionBIC			@call the BIC function

printResult:

	pop	{r1}				@pop the resutls from the stack, was pushed from r2
	ldr	r0, =strResult			@load the result string
	bl	printf				@print the result

	ldr 	r0, =strContinue		@load the continue question
	bl	printf				@print the quesiton

	ldr	r0, =intScan			@load the int scan modifier
	ldr	r1, =intInput			@load the adress
	bl 	scanf				@call scanf

	cmp	r0, #READERROR			@if there is an error
	beq	myExit				@then just exit

	ldr	r0, = intInput			@load the adress of the user input
	ldr	r0, [r0]			@load the value

	cmp	r0, #1				@compare to 1
	beq	prompt1				@if it is 1 jump back to prommpt 1 and start again


	b myExit				@else just exit the program





readError:

	ldr	r0, =scanClear			@scan mod to clear the buffer
	ldr	r1, =scanClearBuf		@place to dump the buffer
	bl	scanf				@call the scanf

	ldr	r0, =strInputError		@load error message
	bl	printf				@print message

	cmp	r4, #1				@was the error in prompt one
	beq	prompt1				@go back to prompt 1

	cmp	r4, #2				@was the error in prompt two
	beq	prompt2				@go back to prompt 2

	cmp	r4, #3				@was the error in prompt three
	beq	prompt3				@go back to prompt 3

@________________________________________________________________________________________________
@gets to inputs from the stack									@
@returns one result on the stack								@
functionAND:											@
												@
	pop	{r2}				@get the first operand, was pushed from r1	@
												@
	pop	{r3}				@ger the second operand, was pushed from r1	@
												@
	and	r2, r2, r3			@preform the and operation			@
												@
	push	{r2}				@push the result onto the stack			@
												@
	bx 	lr				@return						@
												@
@________________________________________________________________________________________________

@________________________________________________________________________________________________
@gets two  inputs from the stack								@
@returns one result on the stack								@
functionORR:											@
												@
	pop	{r2}				@get the first operand, was pushed from r1	@
												@
	pop	{r3}				@get the second operand, was pushed form r1	@
												@
	orr	r2, r2, r3			@preform the orr operation			@
												@
	push	{r2}				@push the resutls onto the stack		@
												@
	bx	lr				@return						@
												@
@________________________________________________________________________________________________

@________________________________________________________________________________________________
@gets two inputs form the stack									@
@returns one result on the stack								@
functionEOR:											@
												@
	pop 	{r2}				@geth te first operadn, was pushed form r1	@
												@
	pop	{r3}				@get the second operand, was pushed from r1	@
												@
	eor	r2, r2, r3			@proform the eor operation			@
												@
	push	{r2}				@push the reuslt on to the stack		@
												@
	bx	lr				@return						@
												@
@________________________________________________________________________________________________

@________________________________________________________________________________________________
@gets two inputs from the stack									@
@returns one result on the stack								@
functionBIC:											@
												@
	pop	{r3}				@get the first operand form the stack		@
												@
	pop	{r2}				@get the second operand from the stack		@
												@
	bic	r2, r2, r3			@preform the bic operation ORDER BACKWARDS?	@
												@
	push 	{r2}				@push the result onto the stack			@
												@
	bx	lr				@return						@
												@
@________________________________________________________________________________________________






@________________________________________________
myExit:						@Exit of the program
						@
	mov r7, #0x01				@load the SVC call to exit
	svc 0					@SCV call
						@
@________________________________________________


.data

@________________________STRINGS_____________________________
.balign 4
strWelcome:	.asciz "Welcome to the four function binary logic calculator.\n\n"

.balign 4
strPrompt1:	.asciz "\nEnter a 32-bit hexadecimal digit:> "

.balign 4
strPrompt2:	.asciz "\n\tAND(1)\n\tORR(2)\n\tEOR(3)\n\tBIC(4)\n\nChose an operation:> "

.balign 4
strInputError:	.asciz "\nInvalid Input\n"

.balign 4
strContinue:	.asciz "Would you like to continue? (press (1) to continue, anything else to close) :> "

.balign 4
strResult:	.asciz "\nResult = %x\n"


@________________________VARIBLES____________________________
.balign 4
hexInput:	.word 0

.balign 4
intInput:	.word 0



@________________________SCAN________________________________
.balign 4
hexScan:	.asciz "%x"		@hexadecimal scan modifier

.balign 4
intScan:	.asciz "%d"		@int scan modifier

.balign 4
scanClear:	.asciz "%[^\n]"		@Used to clear the input buffer for invalid input

.balign 4
scanClearBuf:	.skip 100*4






.global printf
.global scanf
