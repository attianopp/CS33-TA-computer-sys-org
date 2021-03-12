//LAST UPDATED: Spring 2020 
//BY TA: Attiano Purpura-Pontoniere
//EMAIL: attiano@ucla.edu
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

int DIM = 500;
double work_it_par(long* old, long* new, long* super, long* simple, long* fibonacci);
double work_it_seq(long *old, long *new, long* super, long* simple, long *fibonacci);

long __attribute__ ((noinline)) gimmie_the_func() {
  return 333.333;
}

long __attribute__ ((noinline)) we_need_the_func() {
  return 222.222;
}

long __attribute__ ((noinline)) obfuscate_obfuscate_obfuscate(long hair_dont_care) {
  return hair_dont_care * 111111;
}

long old_histogrammy[10];
long new_histogrammy[10];

//helper functions to compare printf behavior

long getFirstNum(char* p){
	while (*p) { // While there are more characters to process...
		if ( isdigit(*p) || ( (*p=='-'||*p=='+') && isdigit(*(p+1)) )) {
			// Found a number
			long val = strtol(p, &p, 10); // Read number in base 10
			return val; 
		} else {
			// Otherwise, move on to the next character.
			p++;
		}
		
    }
	return -1; //error
}



//used to compare par.txt and seq.txt line by line
// checks fp1 vs fp2 for the specific output for the current version of parallel.c, returns -1 if failed, 0 if it worked.
int compareFiles(FILE *fp1, FILE *fp2) 
{ 
	char l1[1024];
	char l2[1024];
	char w1[1024];
	char w2[1024];
	char *c1;
	char *c2;
	
	//parse files
	while ((c1 = fgets(l1, 1024, fp1)),(c2 = fgets(l2, 1024, fp2)), c1 != NULL && c2 != NULL)
	{
		int n1;
		int n2;

		char *line1 = l1;
		char *line2 = l2;

		int r1;
		int r2;
		//parse lines word by word breaking on whitespace
		while ((r1 = sscanf(line1, "%1023s%n", w1, &n1)), (r2 = sscanf(line2, "%1023s%n", w2, &n2)), r1 == 1 && r2 == 1)
		{
			if (strcmp(w1, w2) != 0){
				//printf("w1: %s, w2: %s \n",w1,w2);
				//lines dont match lets check why
				//if there is a float in both lines, which can be checked if there is a period '.' in both, its fine
				//floats are currently checked through the return value check
				//otherwise the printf output doesnt match!
				int periodCheck1 = strchr(w1, '.') != NULL; //returns 1 if period is in w1
				int periodCheck2 = strchr(w2, '.') != NULL;
				//printf("period1: %d, period2: %d\n",periodCheck1,periodCheck2);
				if( periodCheck1 != 1 && periodCheck2 !=1 ) {
					return -1;
				}

				
			}
			line1 += n1;
			line2 += n2;
		}
		//one line had no contents while the other had content
		if(r1 != r2){
			return -1;
		}
	}
	//one file ended before the other
	if(c1 != c2){
		return -1;
	}
	//everything matches!
	return 0;
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
  
  long *original0 = (long *)malloc(sizeof(long)*DIM*DIM*DIM);
  long *new0 = (long *)malloc(sizeof(long)*DIM*DIM*DIM);
  long *super0 = (long *)malloc(sizeof(long)*DIM);
  long *simple0 = (long *)malloc(sizeof(long)*DIM);
  long *fib0 = (long *)malloc(sizeof(long)*DIM);
  
  double result = 0.0;
  double result0 = 0.0;
  
  for (i=0; i<10; i++)
    histogrammy[i]=0;

  for (i=0; i<DIM; i++)
    for (j=0; j<DIM; j++)
      for (k=0; k<DIM; k++)
        {
          original[i*DIM*DIM+j*DIM+k]=(long)(random()%1000);
		  original0[i*DIM*DIM+j*DIM+k]=original[i*DIM*DIM+j*DIM+k];
        }

  for (i=0; i<DIM; i++){
    super[i]=(long)(random()%1000);
    simple[i]=(long)(random()%1000);
	super0[i]=super[i];
	simple0[i]=simple[i];
	fib[i]=0;
	fib0[i]=0;
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

 /*  for (i=0; i<10; i++)
    printf("HISTO[%d]:%d\n",i,histogrammy[i]);
 */
  for (i=0; i<10; i++)
    histogrammy[i]=0;

// TO DO: SWITCH FREOPEN TO dup()/dup2()/open() based on:
//https://stackoverflow.com/questions/4832603/how-could-i-temporary-redirect-stdout-to-a-file-in-a-c-program/4832902#4832902

  clock_gettime(CLOCK_MONOTONIC, &start);
  // get printf behavior and store it in seq.txt
  freopen("seq.txt","w",stdout);
  result = work_it_seq(original, new, super, simple, fib);
  clock_gettime(CLOCK_MONOTONIC, &finish);
  // repoint stdout to stdout for linux
  freopen ("/dev/tty", "a", stdout);

  seqDelay = (finish.tv_sec - start.tv_sec);
  seqDelay += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  for (i=0; i<10; i++){
/*     printf("NEWHISTO[%d]:%d\n",i,histogrammy[i]);
 */	//save into old histo
	old_histogrammy[i] = histogrammy[i];
	
  }

/*   printf("Sequential version took %lf time units\n",seqDelay);
 */
  for (i=0; i<10; i++){
    histogrammy[i]=0;
  }

  clock_gettime(CLOCK_MONOTONIC, &start);
  //get printf behavior and store it in par.txt
  freopen("par.txt","w",stdout);
  result0 = work_it_par(original0, new0, super0, simple0, fib0);
  clock_gettime(CLOCK_MONOTONIC, &finish);
  freopen ("/dev/tty", "a", stdout);


  parDelay = (finish.tv_sec - start.tv_sec);
  parDelay += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  for (i=0; i<10; i++){
/*     printf("NEWPARHISTO[%d]:%d\n",i,histogrammy[i]);
 */	//compare this to old histo
	new_histogrammy[i] = histogrammy[i];
	
  }


 /*  printf("Parallel version took %lf time units\n", parDelay);

  printf("This resulted in a %fx speed-up\n", (float)(seqDelay / parDelay));
  printf("Ending the parallelization test\n");
 */
  
	/* TESTING STRUCTURE*/
	printf("\n\n##############################TESTING##############################\n");

  /* Spring 2019 TA:
    Make sure you include <unistd.h>
   */
  // printf("\nRunning some extra tests to make sure everything is right...\n");
  // printf("If this is taking way too long, you probably have a lot of errors so just quit\nby pressing CTRL-C a lot of times!");
  // printf("\nNote: Please delete the fail.txt directory before testing if you think its reporting some nonsense\n");
  // char cwd[150];
  // char filePath[60];
  // if (getcwd(cwd, 150) == NULL) {
  //   perror("getcwd() error");
  //   return 1;
  // }
  // sprintf(filePath, "%s/fail.txt",cwd);
  // remove(filePath);
  // FILE* fp = fopen (filePath,"w");
  
  double speed_up = 0.0;
  double score = 0.0;
  double extraCredit = 0.0;
  long numWrong=0;
  long totalWrong=0;
  int iScale, jScale;
  
    // ADD CHECKS FOR PRINTF BEHAVIOR HERE 
    FILE *seqText = fopen("seq.txt", "r+");
	if (seqText == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	FILE *parText = fopen("par.txt", "r+");
	if (parText == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	
    if(compareFiles(seqText,parText)!=0){
		printf("Printf behavior didn't match!\n");
		totalWrong++;
		
	}
	else{
		printf("Printf behavior matches!\n");
	}
    
	fclose(seqText);
	fclose(parText);
	
	//ADD CHECKS FOR RETURN VALUES, HISTO OUTPUT, AND ARRAY/DATA MANIPULATION HERE
  
   /* if( (result <= result0 && result0 <=(result+.005)) || ((result-.005) <= result0 && result0 <=result) )
	{printf("Return values match!\n");} 
  else
	{
	 printf("Return values dont match! - error in dot_product/moving_average/pi\n");
	 printf("Sequential result:%d, Parallel result:%d\n",result,result0);
	 totalWrong++;
	 } */

   for (i=0; i<10; i++){
	if(new_histogrammy[i] != old_histogrammy[i]){
		numWrong++;
	}
	
  }
   printf("ATTENTION: There were %d errors in the histo check\n", numWrong);
   totalWrong+= numWrong;
   
   numWrong = 0;
   for (i=0; i<DIM; i++)
   {
    for (j=0; j<DIM; j++)
	{
      for (k=0; k<DIM; k++)
        {
          if(original0[i*DIM*DIM+j*DIM+k]!=original[i*DIM*DIM+j*DIM+k]){
			numWrong++;
		  }     
		}
	}
   }
   printf("ATTENTION: There were %d errors in the original check\n", numWrong);
   totalWrong+= numWrong;
   
   numWrong = 0;
   for (i=0; i<DIM; i++)
   {
    for (j=0; j<DIM; j++)
	{
      for (k=0; k<DIM; k++)
        {
          if(new0[i*DIM*DIM+j*DIM+k]!=new[i*DIM*DIM+j*DIM+k]){
			numWrong++;
		  }        
		}
	}
   }
  printf("ATTENTION: There were %d errors in the new check\n", numWrong);
  totalWrong+= numWrong;
  
  numWrong = 0;
  for (i=0; i<DIM; i++){
	if(super[i]!=super0[i]){
		numWrong++;
	}
  }
  printf("ATTENTION: There were %d errors in the super check\n", numWrong);
  totalWrong+= numWrong;
  numWrong = 0;
  
  for (i=0; i<DIM; i++){
	if(simple[i]!=simple0[i]){
		numWrong++;
	}
  }
  printf("ATTENTION: There were %d errors in the simple check\n", numWrong);
  totalWrong+= numWrong;
  numWrong = 0;
  
/*   for (i=0; i<DIM; i++){
	if(fib[i]!=fib0[i]){
		numWrong++;
		printf("\n fib[%d]:%d, fib0[%d]:%d",i,fib[i],i,fib0[i]);
	}
  }
  printf("ATTENTION: There were %d errors in the fib check\n", numWrong); */
  
  //this doesn't affect the score because the TA who made this had errors in his code from spring 2019
  //numWrong = 0;
  /* for (i=1; i<DIM-1; i++) {
    iScale = i *DIM*DIM;
    for (j=1; j<DIM-1; j++) {
      jScale = j*DIM;
      for (k=1; k<DIM-1; k++) {
        if(original[iScale+jScale+k] != new0[iScale+jScale+k]){
          ++numWrong;
        }
      }
    }
  }
  printf("\n\nATTENTION: There were %d errors in K.S. check\n", numWrong); */
  
  if(totalWrong != 0){
	//TA ATTN: could give correctness score based on number of errors here or based on each testing section above
	
    // printf("%lf\n", 0.0 / parDelay);
    // fprintf(stderr, "\n%lf", seqDelay / parDelay);
    // double pt = (numWrong / (DIM*DIM*DIM)) * 100;
    printf("\n\nATTENTION: There were %d errors in the output of parallel.c \n", totalWrong);
	//printf("Score: %d\n",score);
    // printf("About %d of your matrix was wrong\nCheck the newly created fail.txt file to see which elements were wrong",numWrong);
  }else{
	//best of 5 runs
    for (i=0; i<5; i++){
      clock_gettime(CLOCK_MONOTONIC, &start);
      result0 = work_it_par(original0, new0, super0, simple0, fib0);
      clock_gettime(CLOCK_MONOTONIC, &finish);

      repDelay = (finish.tv_sec - start.tv_sec);
      repDelay += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
      
/*       printf("Rep version took %lf time units\n", repDelay);
 */
      if(repDelay < parDelay)
        parDelay = repDelay;
    }
	
	speed_up = (seqDelay / parDelay);
	printf("Speedup:%lf\n", speed_up);
	if(speed_up >= 4.5){
		extraCredit = 15.0*(1.0-2.0*exp(-floor(speed_up-3.5)/2.0));
		extraCredit = abs(extraCredit);
		score = 100.0;
	}
	else if (3.5<=speed_up && speed_up<4.5){
		score = 100.0;
	}
	else if (3.0<=speed_up && speed_up<3.5){
		score = 87.5;
	}
	else if (2.5<=speed_up&&speed_up<3.0){
		score = 71.5;
	}
	else if (2.0<=speed_up&&speed_up<2.5){
		score = 57.25;
	}
	else if (1.5<=speed_up&&speed_up<2.0){
		score = 43.0;
	}
	else if (1.0<=speed_up&&speed_up<1.5){

		score = 28.75;

	}
	
	
    printf("\nEverything looks good. Sequential and Parallel matrices match!\n");
   }
   //write out results to file score.txt for perl script to read into overall scores.txt file
  /* END TESTING STRUCTURE*/
    FILE *f = fopen("score.txt", "w+");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	fprintf(f, "%f %f\n", score, extraCredit);
	fclose(f);
	
	printf("\n\n##############################TESTING OVER##############################\n");
	printf("Score: %4.2f Extra Credit: %4.2f\n",score, extraCredit);


  
 
  
  free(original);
  free(new);
  free(super);
  free(simple);
  free(fib);
  free(original0);
  free(new0);
  free(super0);
  free(simple0);
  free(fib0);

  return 0;

}