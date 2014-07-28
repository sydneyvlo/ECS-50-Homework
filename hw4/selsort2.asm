; Assemble using:  nasm -f elf -l selsort2.lst selsort2.asm
; Compile with: gcc -o selsort2.out selsortMain.c selsort2.o
; Note that in Linux selectionSort, printf and main must NOT begin with underscores.
; In non-Linux _selectionSort _printf and _main must begin with underscores.
segment	.text
global selectionSort
selectionSort:				
	push	ebp                 	;   store base pointer		
	mov	ebp, esp              	;   move stack pointer to base pointer		
	push	esi                 	;   store esi		
	push	ebx                 	;   store ebx		
	sub	esp, 16               	;   add 16 to stack pointer make room for local variables		
	mov	dword [ebp-12], 0		;  i <- 0, -12(%ebp) is i, -16(%ebp) is j,
                            	;  -20(%ebp) = minIndex,  -24(%ebp) = temp		
L2:	 	; 		
	mov	eax, [ebp+12]		;   eax <- n		
	dec	eax                     	;   --eax = n - 1		
	cmp	eax, [ebp-12]		;   i ? n - 1		
	jle	L1                      	;   if(n - 1 <= i) then L1		
	mov	eax, [ebp-12]		;   eax <- ebp[-12] = i		
	mov	[ebp-20], eax		;   ebp[-20] = minIndex <- eax = i		
	mov	eax, [ebp-12]		;   eax <- ebp[-20] = i		
	inc	eax                     	;   ++eax = ++i		
	mov	[ebp-16], eax		;   ebp[-16] = j <- eax = i		
L5:	 	; 		
	mov	eax, [ebp-16]		;   eax <- ebp[-16] = j		
	cmp	eax, [ebp+12]		;   n ? j		
	jge	L6                      	;      if( j >= n) then L6		
	mov	eax, [ebp-16]		;   eax <- j		
	lea	esi, [0+eax*4]          	;   esi <- eax * 4 = j * 4		
	mov	ebx, [ebp+8]		;   ebx <- &ar[0]		
	mov	eax, [ebp-20]		;   eax <- minIndex		
	lea	ecx, [0+eax*4]          	;   ecx <- minIndex * 4		
	mov	edx, [ebp+8]		;   edx <- &ar[0]		
	mov	eax, [esi+ebx]	;   eax <- *(&ar[0] + j * 4) = *(&ar[j]) = ar[j]		
	cmp	eax, [ecx+edx]	;   *(&ar[0] + minIndex * 4) ? ar[j]		
	jge	L7                      	;   if(ar[j] >= ar[minIndex]) then goto L7		
	mov	eax, [ebp-16]		;   eax <- j		
	mov	[ebp-20], eax		;   minIndex <- j		
L7:		; 		
	lea	eax, [ebp-16]           	;   eax <- &j		
	inc	dword [eax]         	;   ++(*eax) = ++j
	jmp	L5	 	; 		
L6:	 	; 		
	mov	eax, [ebp-12]		;   eax <- i		
	lea	edx, [0+eax*4]          	;   edx <- eax * 4 = i * 4		
	mov	eax,  [ebp+8]		;   eax <- &ar[0]		
	mov	eax, [edx+eax]	;   eax <- *(&ar[0] + i * 4) = *(&ar[i]) = ar[i]		
	mov	[ebp-24], eax		;   temp <- ar[i]		
	mov	eax, [ebp-12]		;   eax <- i		
	lea	ecx, [0+eax*4]          	;   ecx <- i * 4		
	mov	ebx, [ebp+8]		;   ebx <- &ar[0]		
	mov	eax, [ebp-20]		;   eax <- minIndex		
	lea	edx, [0+eax*4]          	;   edx <- minIndex * 4		
	mov	eax, [ebp+8]		;   eax <- &ar[0]		
	mov	eax, [edx+eax]		;   eax <- *(&ar[0] + minIndex * 4) = *(&ar[minIndex]) = ar[minIndex]		
	mov	[ecx+ebx], eax		;   *(&ar[0] + i * 4) = *(&ar[i]) = ar[i] <- eax = ar[minIndex]		
	mov	eax, [ebp-20]		;   eax <- minIndex		
	lea	ecx, [0+eax*4]          	;   ecx <- eax * 4 = minIndex * 4		
	mov	edx, [ebp+8]		;   edx <- &ar[0]		
	mov	eax, [ebp-24]		;   eax <- temp		
	mov	[ecx+edx], eax		;   *(&ar[0] + minIndex * 4) = *(&ar[minIndex]) = ar[minIndex] = eax = temp		
	lea	eax, [ebp-12]           	;   eax <- &i		
	inc	dword [eax]         	;   (*i)++
	jmp	L2	 	 		
L1:	 	 		
	add	esp, 16                 	;   remove room for local variables		
	pop	ebx                     	;   restore ebx		
	pop	esi                     	;   restore esi		
	pop	ebp                     	;   restore ebp		
	ret                         	; 		

