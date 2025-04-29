@ Filename:	Lab1Beaver.s
@ Author:	Kaden Beaver
@ Purpose:	Take user input to fill array using Auto indexing.  add arrays and print result using a non leaf subrutine
@ Date:		02.07.2021
@
@
@
@ Use these commands to assemble, link, run and debug
@	as -o Lab1Beaver.o Lab1Beaver.s
@	gcc -o Lab1Beaver Lab1Beaver.o
@	./Lab1Beaver
@	gdb -- args ./Lab1Beaver
@
@
@

.equ READERROR, 0 			@scanf readerror

.global main

@____________________________________________________________________MAIN___________________________________________________________________
main:					@start of main

welcome:				@welcome Label

	ldr	r0, =strWelcome		@load adress of welcome message
	bl	printf			@call printf

	mov 	r4, #10			@set the counter for the loop to 10
	ldr	r5, =b			@load the adress of the start of array b


@________________________________________________
	inputLoop:				@ this will let the user input values for the first 10 indexs of array b
						@
		ldr	r0, =intScan		@ load the int scan formater
		ldr	r1, =intBuffer		@ load the address of buffer variable
		bl	scanf			@ call scanf
						@
		ldr	r1, =intBuffer		@ load the address of the buffer variable after the scanf
		ldr	r1, [r1]		@ load the value of the buffer
		str	r1, [r5], #4		@ store the buffer value into the array b and update the index
						@
		subs	r4, #1			@ tick the counter down 1
		beq	inputLoopEND		@ if the counter is 0 exit the loop
		b	inputLoop		@ loop
						@
	inputLoopEND:				@
@________________________________________________

startAdd:				@start of the adding

	ldr	r3, =a			@load the adress of array a
	ldr 	r4, =b			@load teh adress of array b
	ldr	r5, =c			@load the adress of array c
	mov	r6, #20			@set the loop counter to 20


@_______________________________________________
	addLoop:				@ this will add the elements of array a and b into c
						@
		ldr	r0, [r3], #4		@ load the value of array a into r0, and update the index
		ldr	r1, [r4], #4		@ load the value of array b into r1, and update the index
		add	r2, r0, r1		@ add a and b and into r2
		str	r2, [r5], #4		@ store the sum into array c and update teh index
						@
		subs	r6, #1			@ tick the counter
		beq	addLoopEND		@ if the counter reaches 0 exit loop
		b	addLoop			@ loop
						@
	addLoopEND:				@
@________________________________________________


printTheArrays:				@start of the array printing

	ldr	r0, =strArrayA		@load the adress of the header for array a
	bl	printf			@print the header

	ldr	r0, =a			@load the adress of array a into r0 for the array print function
	mov	r1, #20			@load the length of the array into r1
	bl	voidPrintArray		@call the array print function

	ldr	r0, =strArrayB		@load the adress of the header for array b
	bl	printf			@print the header

	ldr 	r0, =b			@load the adress of array b into r0 for the arrat print function
	mov 	r1, #20			@load the length of the array into r0
	bl	voidPrintArray		@call the array print function

	ldr 	r0, =strArrayC		@load the header for array c
	bl	printf			@print the header

	ldr 	r0, =c			@load the adress of array c
	mov	r1, #20			@load the length of the array
	bl	voidPrintArray		@call the array print function

					@END OF PROGRAM CONTINUE TO EXIT


@________________________________________
myExit:					@Exit of the program
 					@
					@
	mov r7, #0x01			@SVC call to exit
	svc 0				@
					@
@________________________________________

@_______________________________________FUNCTIONS________________________________________________________________________________________________________

@r0: adress of start of array
@r1: length of array
@nothing to return
@_______________________________________________________________________________________________
voidPrintArray:											@
												@
	push	{r4, lr}		@save the curent regs to the stack			@
												@
	mov r4, r0			@move the adress of array to r4 so its not lost to print@
	mov r5, r1			@move the length of array to r5 for counter		@
@________________________________________________						@
	loopPrintArray:				@ this will print values of			@
						@ the array on a single line			@
		ldr	r0, =strInt		@ load the string formater			@
		ldr 	r1, [r4], #4		@ load the value of the array and update index	@
		bl 	printf			@ print the vaule				@
						@						@
		subs r5, r5, #1			@ tick the counter				@
		beq	loopPrintArrayEND	@ if the counter is at 0 exit loop		@
		b	loopPrintArray		@ loop						@
						@						@
	loopPrintArrayEND:			@						@
@________________________________________________						@
												@
	ldr 	r0, =strCR		@load the adress of newline				@
	bl	printf			@print the new line					@
												@
	pop	{r4, lr}		@restore the regs from the stack			@
												@
	bx	lr			@exit the funtion through the lr			@
@________________________________________________________________________________________________



@__________________________________________________________________DATA________________________________________________________________
.data



@_______________STRINGS________________
.balign 4
strWelcome: .asciz "Welcome please enter 10 ints to fill out array b.\n"

.balign 4
strArrayA: .asciz "\t Array A:> "

.balign 4
strArrayB: .asciz "\t+Array B:> "

.balign 4
strArrayC: .asciz "\t=Array C:> "

.balign 4
strInt: .asciz "%4d"

.balign 4
strCR: .asciz "\n"


@_______________SCANF_PATTERNS_________
.balign 4
intScan: .asciz "%d"

.balign 4
clearScan: .asciz "%[^\n]"

.balign 4
strInputError: .skip 100*4

@_______________INTS___________________
.balign 4
intBuffer: .word 0


@_______________ARRAYS_________________
.balign 4	@array a all hard coded
a:	.word 5		@ a[0]
	.word 10	@ a[1]
	.word 4		@ a[2]
	.word 0		@ a[3]
	.word -5	@ a[4]
	.word 6		@ a[5]
	.word -7	@ a[6]
	.word 0		@ a[7]
	.word 42	@ a[8]
	.word -71	@ a[9]
	.word 2		@ a[10]
	.word 12	@ a[11]
	.word 33	@ a[12]
	.word -34	@ a[13]
	.word -5	@ a[14]
	.word -9	@ a[15]
	.word 64	@ a[16]
	.word -8	@ a[17]
	.word -23	@ a[18]
	.word 31	@ a[19]

.balign 4	@array b first half is user entered
b: 	.word 0		@ b[0]	0-9 are to be user entered
	.word 0 	@ b[1]
	.word 0 	@ b[2]
	.word 0		@ b[3]
	.word 0 	@ b[4]
	.word 0		@ b[5]
	.word 0		@ b[6]
	.word 0 	@ b[7]
	.word 0 	@ b[8]
	.word 0		@ b[9]
	.word 63	@ b[10]
	.word 31	@ b[11]
	.word 5		@ b[12]
	.word -55	@ b[13]
	.word 2		@ b[14]
	.word -52	@ b[15]
	.word -12	@ b[16]
	.word -8	@ b[17]
	.word 42	@ b[18]
	.word 63	@ b[19]


.balign 4	@array c to be the sum of the two arrays
c: .skip (4*20)



.global printf		@using printf
.global scanf		@using scanf
