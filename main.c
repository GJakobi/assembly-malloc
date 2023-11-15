#include <stdio.h>
#include <stdlib.h>

#include "memalloc.h" 							


void double_alloc_after_free(void **pointers_array){
	pointers_array[1] = memory_alloc(50);
	pointers_array[2] = memory_alloc(100);
}

void simple_alloc(void **pointers_array){
	pointers_array[0] = memory_alloc(100);
}

int main() { 

	printf("============================== ROTINAS DE TESTE ==============================\n");
	
	setup_brk();
	void *initial_brk = brk_original;
	void *alloc_pointers[3];
	void *teste;

	printf("brk original: %d\n", brk_original);

	simple_alloc(alloc_pointers);
	printf("==>> ALOCANDO UM ESPAÇO DE 100 BYTES:\n");
	printf("\tLOCAL: %s\n", alloc_pointers[0]-16 == initial_brk ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[0]-16)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[0]-8)) == 100 ? "CORRETO!" : "INCORRETO!");

	printf("==>> DESALOCANDO UM ESPAÇO DE 100 BYTES:\n");
	memory_free(alloc_pointers[0]);
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[0]-16)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[0]-8)) == 100 ? "CORRETO!" : "INCORRETO!");

	int toBeAllocated = 83;

	teste = memory_alloc(toBeAllocated);

	// printf("%d\n", teste);



	printf("endereco do primeiro bloco: %d, em uso: %d, tamanho: %d\n", teste, *((long long*) (teste-16)), *((long long*) (teste-8)));
	printf("endereco do segundo bloco: %d, em uso: %d, tamanho: %d\n", ((long long*) (teste+toBeAllocated+16)), *((long long*) (teste+toBeAllocated)), *((long long*) (teste+toBeAllocated+8)));

	return 0;
}
