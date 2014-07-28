; Assemble using:  nasm -f elf -l selsort.lst selsort.asm
; Compile with: gcc -o selsort.out selsort.o
; Note that in Linux selectionSort, printf, and main must NOT begin with underscores.
; In non-Linux, _selectionSort, _printf, and _main must begin with underscores.
extern printf	
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

section .data;
LC0:                          	; 		
	db "%d" , 10, 0              	;
section .text                       	;
  global main                  	;
main:                        	; 		
	push	ebp                   	;   store base pointer		
	mov	ebp, esp                	;   move stack pointer to base pointer		
	push	edi                   	;   store edi		
	sub	esp, 100                	;   make room for local variables -72(%ebp) is ar[0], -12(%ebp) is i		
	and	esp, -16                	;   clear last 4 bits of stack pointer so it is on a quad word boundary		
	mov	eax, 0                  	;   eax <- 0		
	add	eax, 15                 	;   eax += 15		
	add	eax, 15                 	;   eax += 15 = 30		
	shr	eax, 4                    ;   eax >>= 4, 30 >>=4 = 1
	sal	eax, 4                  	;   eax <<= 4, 1 <<= 4 = 16 this effectively clears the and lower 4 bits of eax		
                                ;   so it is on a quad word boundary
	mov	[ebp-76], eax		;   ebp[-76] <- 16		
	mov	eax, [ebp-76]		;   eax <- 16  	;   allocates 16 bytes on the stack frame, and returns pointer to its start
;	call	__alloca
;	call	___main               	;
	lea	edi, [ebp-72]           	;   edi <- &(ebp -72) = &ar[0]		
	cld                         	;   direction flag <- 0, so cx will be decreased in stosl		
	mov	edx, 0                  	;   edx <- 0		
	mov	eax, 10                 	;   eax <- 10		
	mov	ecx, eax                	;   ecx <- eax = 10		
	mov	eax, edx                	;   eax <- edx = 0		
	rep stosd                   	;   while(ecx > 0) stosl		
                                ;   (es:edi) = ar[di] <- eax = 0, ecx--
 	mov	dword [ebp-72], 5    	;   ebp[-72] = ar[0] <- 5
	mov	dword [ebp-68], 8   	;   ebp[-68] = ar[1] <- 8
	mov	dword [ebp-64], 12		;   ebp[-64] = ar[2] <- 12
	mov	dword [ebp-60], 2   	;   ebp[-60] = ar[3] <- 2
	mov	dword [ebp-56], 1   	;   ebp[-56] = ar[4] <- 1
	mov	dword [ebp-52], 7     ;   ebp[-52] = ar[5] <- 7
	mov	dword [esp+4], 6      ;   esp[4] <- 6,  push 6
	lea	eax, [ebp-72]             ;   eax <- &(ebp[-72]) = &(ar[0])
	mov [esp], eax      ;   push eax, push (&ar[0])
	call	selectionSort		  		
	mov	dword [ebp-12],  0     ;   ebp[-12] = i <- 0
L10:		  		
	cmp	dword [ebp-12], 5     ;   5 ? ebp[-12] = i
	jg	L11                       ;   if (ebp[-12] > 5) then goto L11
	mov	eax, [ebp-12]       ;   eax <- ebp[-12] = i
	mov	eax, [ebp-72+eax*4]		;   eax <- *(&ebp[-72] + 4 * eax) = *(ar[0] + 4 * eax) = ar[i]		
	mov	[esp+4], eax		;   esp[4] <- eax = ar[i], push ar[i]		
	mov	dword [esp], LC0      ;   esp[0] <- LC0 = &(%d\12\0), push address of formatting string
	call	printf		  		
	lea	eax, [ebp-12]             ;   eax <- &(ebp[-12]) = &i
	inc	dword [eax]           ;   ++(*eax) = ++(* (&i)) = ++i
	jmp	L10		  		
L11:		  		
	mov	eax, 0                    ;   eax <- 0, return 0
	mov	edi, [ebp-4]		;   edi <- ebp[-4] = previous thing on stack???		
	leave		;	.scl	3		.endef
	ret				
