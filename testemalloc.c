#include <stdio.h>

void *brk_original = 0, *brk_current = 0;

void setup_brk() {
    brk_original = sbrk(0);
    brk_current = brk_original;
}

void dismiss_brk() {
    brk_current = brk_original;
    brk(brk_original);
}
// in_use   size   data

void *memory_alloc(unsigned long int bytes) {
    if(brk_current == brk_original) {
        // allocate room for new block
        brk_current = brk_current + 16 + bytes;

        // set in_use to 1
        *((long long*) (brk_current - 16)) = 1;

        // set size to bytes
        
        
    }


}

int main() {

}