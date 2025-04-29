@ File: Lab5Beaver.s
@ Author: 	Kaden Beaver
@		kb0125@uah.edu
@		CS 309-01 2020 Fall
@Purpose: act as an atm for witdrawling cash
@
@ Commands to assemble
@
@ as -o Lab5Beaver.o Lab5Beaver.s
@ gcc -o Lab5Beaver Lab5Beaver.o
@ ./Lab5Beaver
@ gbd --args ./Lab5Beaver
@
@
@
@***********************START***********************

.equ READERROR, 0

.global main

main:

prompt:			@PROMPT*********************

	ldr r0, =strWelcome		@load the adress of the welcome string
	bl  printf			@call printf

		@prompt______________________________

get_input:		@GET_INPUT*******************
@get the input from the user
@checks if the number is less than 200

	ldr r0, =intInputPattern	@load the scanf pattern for int
	ldr r1, =intInput		@load adress for input var

	bl  scanf			@call scanf
	
	ldr r1, =intInput		@load the input var adress
	ldr r1, [r1]			@load the val of input into r1

	cmp r1, #200			@compare user input to 200
	bgt too_great			@if it is greater then 200 print error
	cmp r1, #0			@compare user intput to 0 
	ble negative			@go check for secret code

		@get_input__________________________

mod10:			@MOD10**********************
@check if the input is a 
@multiple of 10

	
	subs r1, r1, #10		@subtrac 10 from user input
	beq  funds_check		@if r1 is now 0 then it is a multiple of 10
	blt  invalid_mult		@if r1 is now less than 10 (and not 0) it is not a multiple of 10
	b    mod10			@loop till less 10
	


		@mod10______________________________

invalid_mult:		@INVALID_MULT***************
@user entered a non 10
@multiple number

	ldr r0, =strInvalidMulti	@load error message
	bl  printf			@print message
	b   prompt			@jump back to the prompt


		@invalid_mult_______________________

too_great:		@TOO_GREAT******************
@the input was to great
	
	ldr r0, =strToGreat		@load message
	bl  printf			@print message
	b   prompt			@jump to prompt

		@too_great__________________________

negative:		@NEGATIVE***************************
@the input was negative
@check for -9

	cmp r1, #-9			@compare user input to -9
	beq admin			@if it is -9 then jump to admin

	ldr r0, =strNeg			@otherwise load error message
	bl  printf			@printmessage
	b   prompt			@jump to prompt

		@negative__________________________

funds_check:		@funds_check***************
@user input is a valid number
@now check if their is suficiant
@funds for their withsrawl

	ldr r1, =intInput		@load user input adress
	ldr r1, [r1]			@load val
	ldr r2, =intBalance		@load the balance adress
	ldr r2, [r2]			@load the val
	cmp r2, r1			@compare the balance and the user input
	blt insuf_funds			@if the user input is greater than the balance print error
	b   dist			@jump to distribute funds

		@funds_check_______________________


insuf_funds:		@INSUF_FUNDS***************
@there was insuficiant funds

	ldr r0, =strInsufFunds		@load error message	
	ldr r1, =intBalance		@load adress of balence
	ldr r1, [r1]			@load val of balance
	bl  printf			@print error
	b   prompt			@loop back to prompt

		@insuf_funds_______________________

dist:			@DIST**********************
@the user input has passed
@all checks now need to 
@distibute bills

	ldr r1, =intInput		@load adress of user input
	ldr r2, [r1]			@load val

	ldr r3, =int20s			@load adress of remaining 20s
	ldr r4, [r3]			@load val

	ldr r5, =int10s			@load adress of remaining 10s
	ldr r6, [r5]			@load val

	ldr r7, =intBalance		@load adress of balance
	ldr r8, [r7]			@load val

	ldr r9, =intTrans		@load adress of runing total of transactions
	ldr r10, [r9]			@load val

	add r10, r10, #1		@add one to the trasactions
	str r10, [r9]			@store the number of trasactions back into memory

	sub r8, r8, r2			@subtract the user input from the balance
	str r8, [r7]			@save the new balance back into memory

	mov r1, #0			@set r1 to 0 for counting 20s
	mov r11, #0			@set r11 to 0 for countings 10s

	loop20s:		@LOOP20S****
	@loop for distributing 20s

		cmp r2, #20		@comapre user input to 20
		blt loop10s		@if it is less than 20 jump to 10s loop
		cmp r4, #0		@compare the number of 20s to 0
		beq loop10s		@if there is 0 20s jump to 10s loop

		cmp r2, #20		@compare the user input to 20
		beq l20end		@if equil then jump to distribueting the final 20

		sub r2, r2, #20		@subtrac 20 from the user input
		sub r4, r4, #1		@subtrac 1 20 from the number of 20s
		add r1, r1, #1		@add 1 to the number of 20s given
	
		b   loop20s		@loop

			@loop20s____________

	loop10s:		@LOOP10s****
	@loop for distributing 10s

		cmp r6, #0		@compare the number of 10s to 0
		beq end			@if there is 0 then jump to end
		
		cmp r2, #10		@compare the user input to 10
		beq l10end		@if it is equal to 10 jump to distribut last 10

		sub r2, r2, #10		@subtract 10 from user input
		sub r6, r6, #1		@subtract 1 10 from number of 10s
		add r11, r11, #1	@add 1 to the number of 10s given

		b   loop10s		@loop

			@loop10s____________

	l20end:			@20END******
	@end of 20 loop
	@distribute last 20

		sub r4, r4, #1		@subract 1 20 from number of 20s
		add r1, r1, #1		@add 1 to the number of 20s given
		b   dist_end		@jump to saving vals

			@20end______________

	l10end:			@10end******
	@end of 10 loop
	@distribute last 10

		sub r6, r6, #1		@subtract 1 10 from the number of 10s
		add r11, r11, #1	@add 1 to the number of 10s given

		b   dist_end		@jumpt to saving vals

			@10end______________

	dist_end:		@DIST_END***
	@distrubution is finshed
	@save the number of bills remaiing

		str r4, [r3]		@store the number of 20s
		str r6, [r5]		@store the number of 10s

			@dist_end___________

	ldr r0, =strWitdrawBills	@load message for the number of bills given
	mov r2, r11			@move the number of 10s to r2, the number of 20s is already in r1
	bl  printf			@call printf

	cmp r10, #10			@compare the number of transaction to 10
	beq end				@if 10 jump to end
	
	cmp r6, #0			@comapre the number of 10s to zero
	beq end				@if 0 jump to end


	b   prompt			@other wise jump back up to the prompt again
	


		@dist______________________________

end:			@END***********************
@the transation are over
@now print out the data

	ldr r0, =strEnd			@load end message
	bl  printf			@print message

	ldr r0, =strTrans		@load trans message
	ldr r1, =intTrans		@load adress of trans
	ldr r1, [r1]			@load val
	bl  printf			@print trans

	ldr r0, =str10billsU		@load 10 bills meassage
	ldr r1, =int10s			@load adress of 10 bill remaining
	ldr r1, [r1]			@load val
	mov r2, #50			@load 50 into r2 for sub call
	sub r1, r2, r1			@sub the number of remaining bill from 50
	bl  printf			@print to number of bills given

	ldr r0, =str20billsU		@load 20 bills measaage
	ldr r1, =int20s			@load adress of 20 bills remaining
	ldr r1, [r1]			@load val
	mov r2, #50			@load 50 into r2 for sub
	sub r1, r2, r1			@sub stract the remaining 20s from 50
	bl  printf			@print the number of bills given

	ldr r0, =strDist		@load the distubution message
	ldr r1, =intBalance		@load adress of balance
	ldr r1, [r1]			@load val
	mov r2, #1500			@load 1500 in to r2 for sub
	sub r1, r2, r1			@subract the remaining balance from 1500 to get the total distributed
	bl  printf			@print the total distribution
	
	ldr r0, =strRemainingBal	@load remaingin balence message
	ldr r1, =intBalance		@load adress of balance
	ldr r1, [r1]			@load val
	bl  printf			@print the remaing balance

	b   myexit			@exit program

	
		@end_______________________________

admin:			@ADMIN*********************
@-9 has been entered
@print out data

	ldr r0, =str10bills		@load 10 bills string
	ldr r1, =int10s			@load adress of 10s
	ldr r1, [r1]			@load val
	bl  printf			@print message

	ldr r0, =str20bills		@load 20 bills string
	ldr r1, =int20s			@load adress of 20s
	ldr r1, [r1]			@load val
	bl  printf			@print numver of 20s remaing
	
	ldr r0, =strRemainingBal	@load balance string
	ldr r1, =intBalance		@load adress of balance
	ldr r1, [r1]			@load val
	bl  printf			@print the remaining adress

	ldr r0, =strTrans		@load trans message
	ldr r1, =intTrans		@load adress of trans
	ldr r1, [r1]			@load val
	bl  printf			@print the number of trasactions

	ldr r0, =strDist		@load dist message
	ldr r1, =intBalance		@load adress of balance
	ldr r1, [r1]			@load val
	mov r2, #1500			@move 1500 into r2
	sub r1, r2, r1			@subtract balance from 1500 to find the total distributed
	bl  printf			@print the total distibuted

	b  prompt			@jump back to the prompt

		@admin_____________________________



myexit:			@MYEXIT**********************
@ end of the code use
@ system call to force
@ exit and return control
@ to the os

	mov r7, #0x01			@load the exit command into r7
	svc 0				@system call
		@myexit_________________________



.data			@DATA*******************************

.balign 4
strWelcome: .asciz "Welcome.  Enter amount to withdraw (<$200 and a multiple of $10) :>\n"

.balign 4
strToGreat: .asciz "Amount entered is too high.  <$200 only.\n"

.balign 4
strNeg: .asciz "Amount must be greater than zero.\n"

.balign 4
strInvalidMulti: .asciz "Invalid amount.  Must be multiple of 10.\n"

.balign 4
strInsufFunds: .asciz "Insuficiant funds.  Pleas enter amount less than $%d\n"

.balign 4
str10bills: .asciz "Number of $10 bills: %d\n"

.balign 4
str10billsU: .asciz "Number of $10 bills distributed: %d\n"

.balign 4
str20bills: .asciz "Number of $20 bills: %d\n"

.balign 4
str20billsU: .asciz "Number of $20 bills distributed: %d\n"

.balign 4
strRemainingBal: .asciz "Remaining Balance: %d\n"

.balign 4
strTrans: .asciz "Current number of transactions: %d\n"

.balign 4
strDist: .asciz "Total distributions made: %d\n"

.balign 4
strWitdrawBills: .asciz "You get %d $20 bills and %d $10 bills.\n"

.balign 4
strEnd: .asciz "Transactions finshed.\n"


			@Scanf patters**********************

.balign 4
intInputPattern : .asciz "%d"


			@Varible adresses

.balign 4
intInput: .word 0

.balign 4
int10s: .word 50

.balign 4
int20s: .word 50

.balign 4
intBalance: .word 1500

.balign 4
intTrans: .word 0

			


		@uses printf, and scanf

.global printf

.global scanf
