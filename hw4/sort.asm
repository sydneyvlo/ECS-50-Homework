section	.text
; Sydney Lo
; Integer sorting function using insertion sort. Register neutral.
; void sort (int array, int arraySize);
; Let's use di as holding the current position and si to hold the array.

	global	my_sort
my_sort:
	push ebp	; Save the old stack pointer
	mov	ebp, esp	; Move the stack pointer onto the base pointer(frame pointer in CUSP)
	push esi	; Save the old esi register
	push edi	; Save the old edi register
	sub esp, 4	; Making room for one local variable
		
	
	
	
