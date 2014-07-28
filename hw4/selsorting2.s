	.file	"selsorting2.c"
	.intel_syntax noprefix
	.text
.globl sort
	.type	sort, @function
sort:
	push	ebp
	mov	ebp, esp
#APP
# 6 "selsorting2.c" 1
	.intel_syntax noprefix
	push	esi
	push	ebx
	sub	esp, 16
	mov dword ptr	[ebp-12], 0
	L2:
	mov	eax, [ebp+12]
	dec	eax
	cmp	eax, [ebp-12]
	jle	L1
	mov	eax, [ebp-12]
	mov	[ebp-20], eax
	mov	eax, [ebp-12]
	inc	eax
	mov	[ebp-16], eax
	L5:
	mov	eax, [ebp-16]
	cmp	eax, [ebp+12]
	jge	L6
	mov	eax, [ebp-16]
	lea	esi, [0+eax*4]
	mov	ebx, [ebp+8]
	mov	eax, [ebp-20]
	lea	ecx, [0+eax*4]
	mov	edx, [ebp+8]
	mov	eax, [esi+ebx]
	cmp	eax, [ecx+edx]
	jge	L7
	mov	eax, [ebp-16]
	mov	[ebp-20], eax
	L7:
	lea	eax, [ebp-16]
	inc	dword ptr [eax]
	jmp	L5
	L6:
	mov	eax, [ebp-12]
	lea	edx, [0+eax*4]
	mov	eax,  [ebp+8]
	mov	eax, [edx+eax]
	mov	[ebp-24], eax
	mov	eax, [ebp-12]
	lea	ecx, [0+eax*4]
	mov	ebx, [ebp+8]
	mov	eax, [ebp-20]
	lea	edx, [0+eax*4]
	mov	eax, [ebp+8]
	mov	eax, [edx+eax]
	mov	[ecx+ebx], eax
	mov	eax, [ebp-20]
	lea	ecx, [0+eax*4]
	mov	edx, [ebp+8]
	mov	eax, [ebp-24]
	mov	[ecx+edx], eax
	lea	eax, [ebp-12]
	inc	dword ptr [eax]
	jmp	L2
	L1:
	add	esp, 16
	pop	ebx
	pop	esi
	pop	ebp
	ret
	
# 0 "" 2
#NO_APP
	pop	ebp
	ret
	.size	sort, .-sort
	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
.globl main
	.type	main, @function
main:
	push	ebp
	mov	ebp, esp
	and	esp, -16
	push	edi
	push	ebx
	sub	esp, 72
	lea	ebx, [esp+20]
	mov	eax, 0
	mov	edx, 10
	mov	edi, ebx
	mov	ecx, edx
	rep stosd
	mov	DWORD PTR [esp+20], 5
	mov	DWORD PTR [esp+24], 8
	mov	DWORD PTR [esp+28], 12
	mov	DWORD PTR [esp+32], 2
	mov	DWORD PTR [esp+36], 1
	mov	DWORD PTR [esp+40], 7
	mov	DWORD PTR [esp+4], 6
	lea	eax, [esp+20]
	mov	DWORD PTR [esp], eax
	call	sort
	mov	DWORD PTR [esp+60], 0
	jmp	.L4
.L5:
	mov	eax, DWORD PTR [esp+60]
	mov	edx, DWORD PTR [esp+20+eax*4]
	mov	eax, OFFSET FLAT:.LC0
	mov	DWORD PTR [esp+4], edx
	mov	DWORD PTR [esp], eax
	call	printf
	add	DWORD PTR [esp+60], 1
.L4:
	cmp	DWORD PTR [esp+60], 5
	jle	.L5
	mov	eax, 0
	add	esp, 72
	pop	ebx
	pop	edi
	mov	esp, ebp
	pop	ebp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.1 20090725 (Red Hat 4.4.1-2)"
	.section	.note.GNU-stack,"",@progbits
