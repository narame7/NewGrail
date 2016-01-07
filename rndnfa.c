/* generates a random NFA using a bitstream */
/* this method is similar to the one described by Champarnaud et
 * al. in Random Generation Models for NFAs */
/* we are lazy and use libgmp */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gmp.h>
#include <sys/time.h>
#include <time.h>


#define RAND(X) ((unsigned long int) (X * (rand() / (RAND_MAX + 1.0))))


char *fname;
void showBitmap(mpz_t nfa, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
	if (mpz_tstbit(nfa, i))
	    printf("1");
	else
	    printf("0");
    }
    printf("\n");
}

void showFAdoNFA(unsigned long n,unsigned long k,  mpz_t nfa, mpz_t finals)
{
    unsigned long i, size = n*n*k, src, dst, lbl, foo;

	FILE *fp;

	fp = fopen(fname, "w");
    fprintf(fp, "(START) |- 0\n");
    for (i = 0; i < n; i++)
	if (mpz_tstbit(finals, i))
	    fprintf(fp, "%lu -| (FINAL)\n", i);
  /*  putchar('\n'); 
	fprintf(fp, "\n");
*/

    for (i = 0; i < size; i++)
    {
	if (mpz_tstbit(nfa, i))
	{
	    src = i/(n*k);
	    foo = i%(n*k);
	    dst = foo/k;
	    lbl = foo%k;
	    fprintf(fp, "%lu %lu %lu\n", src, lbl, dst);
	}
    }
    fflush(stdout);
	fclose(fp);
}

/* make sure the NFA is inicially connected (accessible) */
/* the initial state (only one...) is 0 */
/* we mantain two sets: one for the already visited states (visited)
 * and another one for the not yet visited states (virgin) */
/* at each step we randomly choose a state s0 from visited, a state s1
 * from virgin and a symbol x from the alphabet; we then create the
 * transition delta(s0, x) = s1 */
void Connect(int n, int k, mpz_t nfa)
{
    int *visited, *virgin, a, b;
    int i, src, dst, lbl, foo, bar;

    visited = (int*)calloc(n, sizeof(int));
    virgin = (int*)calloc(n, sizeof(int));

    a = 1;
    b = n-1;
    visited[0] = 0;
    for (i = 0; i < b; i++)
	virgin[i] = i+1;

    while (a < n)
    {
	foo = RAND(a);
	bar = RAND(b);
	src = visited[foo];
	dst = virgin[bar];
	lbl = RAND(k);
	mpz_setbit(nfa, src*n*k+(dst*k)+lbl);
	visited[a++] = dst;
	b--;
	memmove(virgin+bar, virgin+bar+1, (b-bar)*sizeof(int));
    }
    
    free(visited);
    free(virgin);
}

void addRandomTransitions(mpz_t nfa, unsigned long int size, int t)
{
    unsigned long int i, j, *unused, foo;
    
    if (t <= 0)
	return;

    unused = (unsigned long int*)malloc((size-mpz_popcount(nfa))*sizeof(unsigned long int));

    j = 0;
    for (i = 0; i < size; i++)
	if (! mpz_tstbit(nfa, i))
	    unused[j++] = i;
    
    for (i = 0; i < t; i++)
    {
	foo = RAND(j);
	mpz_setbit(nfa, unused[foo]);
	if (foo < j-1)
	    memmove(unused+foo, unused+foo+1, (j-foo-1)*sizeof(unsigned long int));
	j--;
    }
    
    free(unused);
}

void generate(int n, int k, double d)
{
    int t;
    unsigned long int size = k*n*n;
    mpz_t nfa, finals;
    gmp_randstate_t rstate;

    /* initialize the random structures */
    gmp_randinit_default(rstate);
    gmp_randseed_ui(rstate,(unsigned long)RAND(RAND_MAX));
    /* initialize the NFA and final states bit streams (we know the exact size) */
    mpz_init2(nfa, size);
    mpz_init2(finals, n);
    
    /* random transition density (always < 1) */
    if (d < 0)
	d = (float)rand()/(float) 0x7fffffff;

    /* first, let us make sure that the NFA is initially connected */
    Connect(n, k, nfa);

    /* now we can add random transitions until the desired density is
     * obtained */
    t = (int)(d*n*n*k-(n-1));
    if (t < 0) {
	fprintf(stderr, "error: it is not possible to have an accessible NFA with %d states and a transition density as low as %f\n", n, d);
	return;
    }
    else
	addRandomTransitions(nfa, size, t);

    /* and we need some final states */
    mpz_rrandomb(finals, rstate, n);

    /* print it */
    showFAdoNFA(n, k, nfa, finals);

    /* free used memory */
    mpz_clear(finals);
    mpz_clear(nfa);
    gmp_randclear(rstate);
}

 void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

 void itoa(int n, char s[])
 {
     int i, sign;
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

int main(int argc, char *argv[])
{
	int counter;
	char *cnt;

    struct timeval foo;
    struct timezone bar;

    if (argc != 3 && argc != 4)
    {
	printf("usage: %s #states #symbols [density]\n", argv[0]);
	return 1;
    }

    bar.tz_minuteswest = 0;
    gettimeofday(&foo, &bar);
    srand((unsigned int)foo.tv_sec+foo.tv_usec);


	for(counter = 0; counter < 100 ; counter++){
		fname = calloc(20, sizeof(char));
		cnt = calloc(20, sizeof(char));
		strcat(fname, "NFAs/input_");	
		itoa(counter, cnt);	
		strcat(fname, cnt);	
		strcat(fname, ".txt");	
/*		fp = fopen(counter.txt, "w");
	printf("%s\n", fname);
*/
    if (argc == 4)
	generate(atoi(argv[1]), atoi(argv[2]), atof(argv[3]));
    else
	generate(atoi(argv[1]), atoi(argv[2]), -1.0);

	free(NULL);
	}
    return 0;
}
