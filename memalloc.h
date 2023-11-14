#ifndef MEMORY_FUNCTIONS_H
#define MEMORY_FUNCTIONS_H

void setup_brk();
void dismiss_brk();
void* memory_alloc(unsigned long int bytes);
int memory_free(void *pointer); 

extern void *brk_original;	
extern void *brk_current;
#endif // MEMORY_FUNCTIONS_H
