# Dynamic Memory Allocation

This project uses the first-fit approach to allocate memory. It implements this API:

```c
void setup_brk();
void dismiss_brk();
void* memory_alloc(unsigned long int bytes);
int memory_free(void *pointer);
```