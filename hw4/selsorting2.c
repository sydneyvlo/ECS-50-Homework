#include <stdio.h>

void sort(int ar[], int n)
{
  // students must place their insertion sort code inline here.
  asm(
  ".intel_syntax noprefix\n\t"
  //"push	ebp\n\t"
	// "mov	ebp, esp\n\t"
	"push	esi\n\t"
	"push	ebx\n\t"
	"sub	esp, 16\n\t"
	"mov dword ptr	[ebp-12], 0\n\t"
"L2:\n\t"
	"mov	eax, [ebp+12]\n\t"
	"dec	eax\n\t"
	"cmp	eax, [ebp-12]\n\t"
	"jle	L1\n\t"
	"mov	eax, [ebp-12]\n\t"
	"mov	[ebp-20], eax\n\t"
	"mov	eax, [ebp-12]\n\t"
	"inc	eax\n\t"
	"mov	[ebp-16], eax\n\t"
"L5:\n\t"
	"mov	eax, [ebp-16]\n\t"
	"cmp	eax, [ebp+12]\n\t"
	"jge	L6\n\t"
	"mov	eax, [ebp-16]\n\t"
	"lea	esi, [0+eax*4]\n\t"
	"mov	ebx, [ebp+8]\n\t"
	"mov	eax, [ebp-20]\n\t"
	"lea	ecx, [0+eax*4]\n\t"
	"mov	edx, [ebp+8]\n\t"
	"mov	eax, [esi+ebx]\n\t"
	"cmp	eax, [ecx+edx]\n\t"
	"jge	L7\n\t"
	"mov	eax, [ebp-16]\n\t"
	"mov	[ebp-20], eax\n\t"
"L7:\n\t"
	"lea	eax, [ebp-16]\n\t"
	"inc	dword ptr [eax]\n\t"
	"jmp	L5\n\t"
"L6:\n\t"
	"mov	eax, [ebp-12]\n\t"
	"lea	edx, [0+eax*4]\n\t"
	"mov	eax,  [ebp+8]\n\t"
	"mov	eax, [edx+eax]\n\t"
	"mov	[ebp-24], eax\n\t"
	"mov	eax, [ebp-12]\n\t"
	"lea	ecx, [0+eax*4]\n\t"
	"mov	ebx, [ebp+8]\n\t"
	"mov	eax, [ebp-20]\n\t"
	"lea	edx, [0+eax*4]\n\t"
	"mov	eax, [ebp+8]\n\t"
	"mov	eax, [edx+eax]\n\t"
	"mov	[ecx+ebx], eax\n\t"
	"mov	eax, [ebp-20]\n\t"
	"lea	ecx, [0+eax*4]\n\t"
	"mov	edx, [ebp+8]\n\t"
	"mov	eax, [ebp-24]\n\t"
	"mov	[ecx+edx], eax\n\t"
	"lea	eax, [ebp-12]\n\t"
	"inc	dword ptr [eax]\n\t"
	"jmp	L2\n\t"
"L1:\n\t"
	"add	esp, 16\n\t"
	"pop	ebx\n\t"
	"pop	esi\n\t"
	"pop	ebp\n\t"
	"ret\n\t"
); // asm
} // sort()

int main()
{
  int i, ar[10] = {5, 8, 12, 2, 1, 7};
  sort(ar, 6);
  for(i = 0; i < 6; i++)
  	printf("%d\n", ar[i]);
  	
  return 0;
} // main


