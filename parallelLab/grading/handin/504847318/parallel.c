//Potential Solution

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>

#include "utils.h"

double work_it_par(long* old, long* new, long* super, long* simple, long* fibonacci) {
  int i, j, k;
  int u;
  int ton = 0;
  long compute_it, moving_average;
  double pi, pi2, x, y, sum, step = 0.0;
  long dot_product = 0;
  long nCirc = 0;
  long aggregate = 1.0;
  double r = 1.0;
  int was_smart = 16;

  step = 1.0 / NUM_STEPS;

  int u0, u1, u2, u3, u4, u5, u6, u7, u8, u9;
  u0 = 0;
  u1 = 0;
  u2 = 0;
  u3 = 0;
  u4 = 0;
  u5 = 0;
  u6 = 0;
  u7 = 0;
  u8 = 0;
  u9 = 0;
  moving_average = 0;
  for (i = 0; i < DIM - 1; i++)
    {
      super[i] += simple[i];
    }
  #pragma omp parallel for private(i) reduction(+:dot_product)
  for (i = 0; i < DIM - 1; i++)
    {
      dot_product += super[i] * simple[i];
      
    }

  printf("\n A secret is: %d", obfuscate_obfuscate_obfuscate(5));
  fibonacci[0] = 1;
  fibonacci[1] = 1;
  for (i = 2; i < DIM - 1; i++)
    {
      fibonacci[i] = fibonacci[i - 1] + fibonacci[i - 2];
    }
 

#pragma omp parallel private(i,j, k, u,ton, x, y, compute_it) reduction(+:dot_product,sum,aggregate,nCirc,u0,u1,u2,u3,u4,u5,u6,u7,u8,u9,)
  {


    #pragma omp for
    for (i = 0; i < NUM_STEPS; i++)
      {
	x = (i + 0.5) * step;
	sum = sum + 4.0 / (1.0 + x * x);
      }


  
    #pragma omp for
    for (i = 0; i < NUM_TRIALS; i++)
      {
		 x = (random() % 10000000) / 10000000.0;
		 y = (random() % 10000000) / 10000000.0;
	if ((x * x + y * y) <= r * r) {
	  nCirc++;
	}
      }
    const int DSQ = DIM * DIM;

    #pragma omp for
    for (i = 1; i < DIM - 1; i++) {
      for (j = 1; j < DIM - 1; j++) {
	for (k = 1; k < DIM - 1; k++) {
	  compute_it = old[i * DSQ + j * DIM + k] * we_need_the_func();
	  aggregate += compute_it / gimmie_the_func();
	}
      }
    }
      #pragma omp for
    for (i = 1; i < DIM - 1; i++)
      {
	int iDSQ = i * DSQ; 
	int iDM = (i - 1) * DSQ; 
	int iDA = (i + 1) * DSQ; 
	for (j = 1; j < DIM - 1; j++) {
	  for (k = 1; k < DIM - 1; k++) {

	    int temp_unroll = 0;
	    temp_unroll += old[iDM + (j - 1) * DIM + (k - 1)];
	    temp_unroll += old[iDM + (j - 1) * DIM + k];
	    temp_unroll += old[iDM + (j - 1) * DIM + (k + 1)];
	    temp_unroll += old[iDM + j * DIM + (k - 1)];
	    temp_unroll += old[iDM + j * DIM + k];
	    temp_unroll += old[iDM + j * DIM + (k + 1)];
	    temp_unroll += old[iDM + (j + 1) * DIM + (k - 1)];
	    temp_unroll += old[iDM + (j + 1) * DIM + k];
	    temp_unroll += old[iDM + (j + 1) * DIM + (k + 1)];

	    temp_unroll += old[iDSQ + (j - 1) * DIM + (k - 1)];
	    temp_unroll += old[iDSQ + (j - 1) * DIM + k];
	    temp_unroll += old[iDSQ + (j - 1) * DIM + (k + 1)];
	    temp_unroll += old[iDSQ + j * DIM + (k - 1)];
	    temp_unroll += old[iDSQ + j * DIM + k];
	    temp_unroll += old[iDSQ + j * DIM + (k + 1)];
	    temp_unroll += old[iDSQ + (j + 1) * DIM + (k - 1)];
	    temp_unroll += old[iDSQ + (j + 1) * DIM + k];
	    temp_unroll += old[iDSQ + (j + 1) * DIM + (k + 1)];

	    temp_unroll += old[iDA + (j - 1) * DIM + (k - 1)];
	    temp_unroll += old[iDA + (j - 1) * DIM + k];
	    temp_unroll += old[iDA + (j - 1) * DIM + (k + 1)];
	    temp_unroll += old[iDA + j * DIM + (k - 1)];
	    temp_unroll += old[iDA + j * DIM + k];
	    temp_unroll += old[iDA + j * DIM + (k + 1)];
	    temp_unroll += old[iDA + (j + 1) * DIM + (k - 1)];
	    temp_unroll += old[iDA + (j + 1) * DIM + k];
	    temp_unroll += old[iDA + (j + 1) * DIM + (k + 1)];
	    temp_unroll /= 27;
	    new[iDSQ + j * DIM + k] = temp_unroll;
	    u = (temp_unroll / 100); 
	    if (u <= 0) u0++;
	    if (u >= 9) u9++;
	    if (u == 1) u1++;
	    if (u == 2) u2++;
	    if (u == 3) u3++;
	    if (u == 4) u4++;
	    if (u == 5) u5++;
	    if (u == 6) u6++;
	    if (u == 7) u7++;
	    if (u == 8) u8++;

	  }
	}
      }
  }

  histogrammy[0] = u0;
  histogrammy[9] = u9;

  histogrammy[1] = u1;
  histogrammy[2] = u2;
  histogrammy[3] = u3;
  histogrammy[4] = u4;
  histogrammy[5] = u5;
  histogrammy[6] = u6;
  histogrammy[7] = u7;
  histogrammy[8] = u8;
  pi = step * sum;
  printf("\n %d trials, Riemann flavored pi is %f \n", NUM_STEPS, pi);
  pi2 = 4.0 * ((double)nCirc / (double)NUM_TRIALS);
  printf("\n %d trials, Monte-Carlo flavored pi is %f \n", NUM_TRIALS, pi2);
  printf("AGGR:%ld\n", aggregate);
  return (double)(dot_product + moving_average + pi + pi2);


}
