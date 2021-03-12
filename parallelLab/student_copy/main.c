#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>
#include "utils.h"

int DIM = 500;
double work_it_par(long* old, long* new, long* super, long* simple, long* fibonacci);
double work_it_seq(long *old, long *new, long* super, long* simple, long *fibonacci);


long __attribute__ ((noinline)) gimmie_the_func() {
  return 58113.3;
}

long __attribute__ ((noinline)) we_need_the_func() {
  return 823421.3;
}

long __attribute__ ((noinline)) obfuscate_obfuscate_obfuscate(long hair_dont_care) {
  return hair_dont_care * 1337;
}

int main( int argc, const char* argv[] )
{
  int i,j,k;
  int u;
  struct timespec start, finish;
  double seqDelay, parDelay, repDelay;

  long *original = (long *)malloc(sizeof(long)*DIM*DIM*DIM);
  long *new = (long *)malloc(sizeof(long)*DIM*DIM*DIM);
  long *super = (long *)malloc(sizeof(long)*DIM);
  long *simple = (long *)malloc(sizeof(long)*DIM);
  long *fib = (long *)malloc(sizeof(long)*DIM);
  
  long *super0 = (long *)malloc(sizeof(long)*DIM);
  long *simple0 = (long *)malloc(sizeof(long)*DIM);
  
  double result = 0.0;
  double result0 = 0.0;
  
  for (i=0; i<10; i++)
    histogrammy[i]=0;

  for (i=0; i<DIM; i++)
    for (j=0; j<DIM; j++)
      for (k=0; k<DIM; k++)
        {
          original[i*DIM*DIM+j*DIM+k]=(long)(random()%1000);
        }

  for (i=0; i<DIM; i++){
    super[i]=(long)(random()%1000);
    simple[i]=(long)(random()%1000);
	fib[i]=0;
	super0[i]=super[i];
    simple0[i]=simple[i];

  }

  for (i=1; i<DIM-1; i++) {
    for (j=1; j<DIM-1; j++) {
      for (k=1; k<DIM-1; k++) {
        u=(original[i*DIM*DIM+j*DIM+k]/100);
        if (u>=9) u=9;
        if (u<=0) u=0;
        histogrammy[u]++;
      }
    }
  }

  for (i=0; i<10; i++)
    printf("HISTO[%d]:%d\n",i,histogrammy[i]);

  for (i=0; i<10; i++)
    histogrammy[i]=0;

  clock_gettime(CLOCK_MONOTONIC, &start);
  result = work_it_seq(original, new, super, simple, fib);
  clock_gettime(CLOCK_MONOTONIC, &finish);

  seqDelay = (finish.tv_sec - start.tv_sec);
  seqDelay += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  for (i=0; i<10; i++){
    printf("NEWHISTO[%d]:%d\n",i,histogrammy[i]);
  }

  printf("Sequential version took %lf time units\n",seqDelay);

  for (i=0; i<10; i++){
    histogrammy[i]=0;
  }

  clock_gettime(CLOCK_MONOTONIC, &start);
  result0 = work_it_par(original, new, super0, simple0, fib);
  clock_gettime(CLOCK_MONOTONIC, &finish);

  parDelay = (finish.tv_sec - start.tv_sec);
  parDelay += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  for (i=0; i<10; i++)
    printf("NEWPARHISTO[%d]:%d\n",i,histogrammy[i]);

  if( (result <= result0 && result0 <=(result+.005)) || ((result-.005) <= result0 && result0 <=result) )
	{
	 printf("\nReturn values match!\n");
	} 
  else
	{
	 printf("\nReturn values dont match! - error in dot_product/moving_average/pi\n");
	 printf("Sequential result:%d, Parallel result:%d\n",result,result0);
	 }


  printf("Parallel version took %lf time units\n", parDelay);

  printf("This resulted in a %fx speed-up\n", (float)(seqDelay / parDelay));
  printf("Ending the parallelization test\n");

  

  free(original);
  free(new);
  free(super);
  free(simple);
  free(fib);
  
  free(super0);
  free(simple0);
 

  return 0;

}