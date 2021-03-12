//OpenMP version.  Edit and submit only this file.
/* Enter your details below
 * Name :
 * UCLA ID : 
 * Email :
 */

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#include "utils.h"

double work_it_par(long *old, long *new, long *super, long *simple, long *fibonacci) {
  int i, j, k, kk, jj, ii;
  int u, v, w, u1;
  int ton = 0;
  long compute_it, i1, moving_average;
  double pi, pi2, x, y, sum, step = 0.0;
  long dot_product=0;
  long nCirc=0;
  long aggregate=1.0;
  double r=1.0;
  int was_smart = 16;
  long temp = 0;
  int iDim, jDim, iuDim, jDimPlus, jDimPlusPlus, simplified, uDim;
  long wntf = we_need_the_func();
  long gtf = gimmie_the_func();
  int dimSquare = DIM*DIM;
  int b = 10;

 #pragma omp parallel for private(i)
  for(i=0; i<DIM-1;i++)
  {
    super[i] += simple[i];
  }

 #pragma omp parallel for private(i) reduction(+:dot_product,moving_average)
  for(i=0; i<DIM-1;i++)
  {
    dot_product += super[i]*simple[i];
    
    moving_average = 0;
    for(ton=i;ton<DIM-1-WINDOW_SIZE;ton++)
    {
      moving_average += simple[ton];
    }
  }

  int a_secret = 5;
  fibonacci[0] = 1;
  fibonacci[1] = 1;
  for(i=2; i<DIM-1;i++)
  {
    fibonacci[i]=fibonacci[i-1]+fibonacci[i-2];
	
    if(i==3)
    {
      printf("\n A secret is: %d",obfuscate_obfuscate_obfuscate(a_secret));
    }
	
  }
	

  step = 1.0 / NUM_STEPS;
  #pragma omp parallel for private(x) reduction(+:sum)
  for (i=0;i<NUM_STEPS; i++)
  {
    x = (i+0.5)*step;
    sum = sum + 4.0/(1.0+x*x);

  }
  pi = step * sum;
  printf("\n %d trials, Riemann flavored pi is %f \n",NUM_STEPS, pi); 

  #pragma omp parallel for private(x, y) reduction(+:nCirc)
  for(i=0;i<NUM_TRIALS; i++)
  {
    x = (random()%10000000)/10000000.0; 
    y = (random()%10000000)/10000000.0;
    if (( x*x + y*y) <= r*r)
    {
      nCirc++;
    }
  } 
  pi2 = 4.0 * ((double)nCirc/(double)NUM_TRIALS);
  printf("\n %d trials, Monte-Carlo flavored pi is %f \n",NUM_TRIALS, pi2); 
  
	#pragma omp parallel for num_threads(16) private (j, k, compute_it, iDim, jDim, iuDim, u, v, w, simplified, uDim, kk, jj, ii) reduction(+:aggregate, temp)
	  for (i=1; i<DIM-1-b; i+=b) {
		for (j=1; j<DIM-1-b; j+=b) {
			 for (k=1; k<DIM-1-b; k+=b) {
				  for (ii = i; ii < i+b; ii++){
					   iDim = ii*dimSquare;
					   for (jj = j; jj < j+b; jj++){
							jDim = jj*DIM;
							for (kk = k; kk < k+b; kk++){
								 simplified = iDim + jDim + kk;
								 compute_it = old[simplified] * wntf;
								 aggregate += compute_it / gtf;
								 temp = 0;
								 for (u=-1; u<=1; u++) {
									  uDim = u*dimSquare;
									  temp += old[simplified-DIM-1+uDim];
									  temp += old[simplified-DIM+uDim];
									  temp += old[simplified-DIM+1+uDim];
									  temp += old[simplified-1+uDim];
									  temp += old[simplified+uDim];
									  temp += old[simplified+1+uDim];
									  temp += old[simplified+DIM-1+uDim];
									  temp += old[simplified+DIM+uDim];
									  temp += old[simplified+DIM+1+uDim];
								  } 
								temp/=27;
								new[simplified] = temp;
							}
					   }
				  }
			 }
		}
	  }

	 printf("AGGR:%ld\n",aggregate);

	 for (i=1; i<DIM-1; i++) {
		iDim = i*dimSquare;
		for (j=1; j<DIM-1; j+=3) {
		  jDim = j*DIM;
		  jDimPlus = (j+1)*DIM;
		  jDimPlusPlus = (j+2)*DIM;
		  for (k=1; k<DIM-1; k+=3) {
			u= (new[iDim+jDim+k] * 41) >> 12;
			if (u<=0) u=0;
			if (u>=9) u=9;
			histogrammy[u]++;
			u= (new[iDim+jDim+k+1] * 41) >> 12;
			if (u<=0) u=0;
			if (u>=9) u=9;
			histogrammy[u]++;
			u= (new[iDim+jDim+k+2] * 41) >> 12;
			if (u<=0) u=0;
			if (u>=9) u=9;
			histogrammy[u]++;
			u= (new[iDim+jDimPlus+k]* 41) >> 12;
			if (u<=0) u=0;
			if (u>=9) u=9;
			histogrammy[u]++;
			u= (new[iDim+jDimPlus+k+1]* 41) >> 12;
			if (u<=0) u=0;
			if (u>=9) u=9;
			histogrammy[u]++;
			u= (new[iDim+jDimPlus+k+2]* 41) >> 12;
			if (u<=0) u=0;
			if (u>=9) u=9;
			histogrammy[u]++;
			u= (new[iDim+jDimPlusPlus+k]* 41) >> 12;
			if (u<=0) u=0;
			if (u>=9) u=9;
			histogrammy[u]++;
			u= (new[iDim+jDimPlusPlus+k+1]* 41) >> 12;
			if (u<=0) u=0;
			if (u>=9) u=9;
			histogrammy[u]++;
			u= (new[iDim+jDimPlusPlus+k+2]* 41) >> 12;
			if (u<=0) u=0;
			if (u>=9) u=9;
			histogrammy[u]++;
		  }
		}
	  }

  /* for (i=1; i<DIM-1; i++) {
    for (j=1; j<DIM-1; j++) {
      for (k=1; k<DIM-1; k++) {
        new[i*DIM*DIM+j*DIM+k]=0;
        for (u=-1; u<=1; u++) {
          for (v=-1; v<=1; v++) {
            for (w=-1; w<=1; w++) {
               new[i*DIM*DIM+j*DIM+k]+=old[(i+u)*DIM*DIM+(j+v)*DIM+(k+w)];
            }
          }
        }
        new[i*DIM*DIM+j*DIM+k]/=27;
      }
    }
  }


  for (i=1; i<DIM-1; i++) {
    for (j=1; j<DIM-1; j++) {
      for (k=1; k<DIM-1; k++) {
        u=(new[i*DIM*DIM+j*DIM+k]/100);
        if (u<=0) u=0;
        if (u>=9) u=9;
        histogrammy[u]++;
      }
    }
  } */
  
  return (double) (dot_product+moving_average+pi+pi2);


}
