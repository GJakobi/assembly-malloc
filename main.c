#include <stdio.h>
#include <stdlib.h>

#include "memalloc.h" 							


int main() { 

	printf("============================== ROTINAS DE TESTE ==============================\n");
	
	setup_brk();
	void *initial_brk = brk_original;
	void *alloc_pointers;

	printf("original brk: %d \n", brk_original);
	

	printf("alocando 10 bytes: \n");
	alloc_pointers = memory_alloc(10);

	printf("alloc_pointers: %d \n", alloc_pointers);
	printf("brk_current: %d \n", brk_current);
	printf("diferenca: %d \n", alloc_pointers - initial_brk);

	printf("in use: %d , %d size \n", *((long long*) (alloc_pointers - 26)), *((long long*) (alloc_pointers - 18)));
	

	
	return 0;
}
