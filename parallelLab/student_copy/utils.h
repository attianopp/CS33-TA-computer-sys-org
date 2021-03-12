#ifndef UTILS
#define UTILS
 
extern int DIM;
 
long histogrammy[10];
long *original;
long *new;
long *super;
long *simple;
long *original;
long *new0;
long *super0;
long *simple0;
long *fib;
long *fib0;

static long NUM_STEPS = 10000;
static long NUM_TRIALS = 5000000;
static int WINDOW_SIZE = 10;

long __attribute__ ((noinline)) gimmie_the_func();

long __attribute__ ((noinline)) we_need_the_func();

long __attribute__ ((noinline)) obfuscate_obfuscate_obfuscate(long hair_dont_care);


#endif