#include <stdio.h>

// rdpmc_instructions uses a "fixed-function" performance counter to return the count of retired instructions on
//       the current core in the low-order 48 bits of an unsigned 64-bit integer.
unsigned long rdpmc_instructions()
{
   unsigned a, d, c;

   c = (1<<30);
   __asm__ volatile("rdpmc" : "=a" (a), "=d" (d) : "c" (c));

   return ((unsigned long)a) | (((unsigned long)d) << 32);;
}

// rdpmc_actual_cycles uses a "fixed-function" performance counter to return the count of actual CPU core cycles
//       executed by the current core.  Core cycles are not accumulated while the processor is in the "HALT" state,
//       which is used when the operating system has no task(s) to run on a processor core.
unsigned long rdpmc_actual_cycles()        
{   
   unsigned a, d, c;

   c = (1<<30)+1;
   __asm__ volatile("rdpmc" : "=a" (a), "=d" (d) : "c" (c));

   return ((unsigned long)a) | (((unsigned long)d) << 32);;
}

// rdpmc_reference_cycles uses a "fixed-function" performance counter to return the count of "reference" (or "nominal")
//       CPU core cycles executed by the current core.  This counts at the same rate as the TSC, but does not count
//       when the core is in the "HALT" state.  If a timed section of code shows a larger change in TSC than in
//       rdpmc_reference_cycles, the processor probably spent some time in a HALT state.
unsigned long rdpmc_reference_cycles()
{
   unsigned a, d, c;

   c = (1<<30)+2;
   __asm__ volatile("rdpmc" : "=a" (a), "=d" (d) : "c" (c));

   return ((unsigned long)a) | (((unsigned long)d) << 32);
}

unsigned long l1_cache_misses() {
   unsigned a, d, c;

    c = 0;
   __asm__ volatile("rdpmc" : "=a" (a), "=d" (d) : "c" (c));

   return ((unsigned long)a) | (((unsigned long)d) << 32);
}

int main() {
    unsigned long x = l1_cache_misses();
    printf("%lld\n", x);
    int a = 0;
    for(int i = 0; i < 10000000; i++) {
        a++;
    }
    printf("%d\n", a);
    unsigned long y = l1_cache_misses();
    printf("%lld\n", y - x);
}