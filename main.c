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
	void *alloc_pointers[3], *test1, *test2, *test3;

	simple_alloc(alloc_pointers);
	printf("==>> ALOCANDO UM ESPAÇO DE 100 BYTES:\n");
	printf("\tLOCAL: %s\n", alloc_pointers[0]-16 == initial_brk ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[0]-16)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[0]-8)) == 100 ? "CORRETO!" : "INCORRETO!");

	printf("==>> DESALOCANDO UM ESPAÇO DE 100 BYTES:\n");
	memory_free(alloc_pointers[0]);
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[0]-16)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[0]-8)) == 100 ? "CORRETO!" : "INCORRETO!");

	double_alloc_after_free(alloc_pointers);
	printf("==>> ALOCANDO UM ESPAÇO DE 50 BYTES:\n");
	printf("\tLOCAL: %s\n", alloc_pointers[1]-16 == initial_brk ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[1]-16)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[1]-8)) == 50 ? "CORRETO!" : "INCORRETO!");
	printf("==>> ALOCANDO UM ESPAÇO DE 100 BYTES:\n");
	printf("\tLOCAL: %s\n", alloc_pointers[1]+100 == alloc_pointers[2]-16 ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[2]-16)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[2]-8)) == 100 ? "CORRETO!" : "INCORRETO!");
	printf("==> VERIFICANDO A FRAGMENTAÇÃO DE MEMÓRIA:\n");
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[1]+50)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[1]+58)) == 34 ? "CORRETO!" : "INCORRETO!");

	test1 = memory_alloc(34);
	printf("==>> ALOCANDO O BLOCO LIVRE NO MEIO: \n");
	printf("\tLOCAL: %s\n", test1-16 == alloc_pointers[1]+50 ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((long long*) (test1-16)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (test1-8)) == 34 ? "CORRETO!" : "INCORRETO!");


	printf("==>> DESALOCANDO TUDO:\n");
	memory_free(alloc_pointers[1]);
	memory_free(alloc_pointers[2]);
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[1]-16)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[1]-8)) == 50 ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[2]-16)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[2]-8)) == 100 ? "CORRETO!" : "INCORRETO!");


	test2 = memory_alloc(25);
	test3 = memory_alloc(9);
	printf("==>> DIVIDINDO PRIMEIRO BLOCO DE 50 BYTES EM 25 E 9:\n");
	printf("\tLOCAL PRIMEIRO: %s\n", test2 == alloc_pointers[1] ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO PRIMEIRO: %s\n", *((long long*) (test2-16)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO PRIMEIRO: %s\n", *((long long*) (test2-8)) == 25 ? "CORRETO!" : "INCORRETO!");
	printf("\tLOCAL SEGUNDO: %s\n", test3-16 == test2+25 ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO SEGUNDO: %s\n", *((long long*) (test3-16)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO SEGUNDO: %s\n", *((long long*) (test3-8)) == 9 ? "CORRETO!" : "INCORRETO!");


	printf("==>> DESALOCANDO A PILHA (ILEGAL):\n");
	unsigned int alloc_return = memory_free((void*) alloc_pointers);
	if (!alloc_return) printf("\tO RETORNO DA LIBERAÇÃO FOI NULL!\n");

	return 0;
}
