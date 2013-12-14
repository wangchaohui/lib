#include <stdio.h>

/* the number and value of the package sizes */
int nsize;
int sizes[10];

/* cando specifies whether a given number is possible or not */
/* since max size = 256, we'll never need to mark more than 256
   in the future, so we use a sliding window */
int cando[256];

int gcd(int a, int b)
 { /* uses standard gcd algorithm to computer greatest common divisor */
  int t;

  while (b != 0)
   {
    t = a % b;
    a = b;
    b = t;
   }
  return a;
 }

int main(int argc, char **argv)
 {
  FILE *fout, *fin;
  int lv, lv2; /* loop variable */
  int pos;     /* count position */
  int last;    /* last impossible count */

  if ((fin = fopen("nuggets.in", "r")) == NULL)
   {
    perror ("fopen fin");
    exit(1);
   }
  if ((fout = fopen("nuggets.out", "w")) == NULL)
   {
    perror ("fopen fout");
    exit(1);
   }

  /* read in data */
  fscanf (fin, "%d", &nsize);
  for (lv = 0; lv < nsize; lv++) fscanf (fin, "%d", &sizes[lv]);

  /* ensure gcd = 1 */
  lv2 = sizes[0];
  for (lv = 1; lv < nsize; lv++)
    lv2 = gcd(sizes[lv], lv2);
  if (lv2 != 1)
   { /* gcd != 1, no bound on size that cannot be purchased */
    fprintf (fout, "0\n");
    return 0;
   }

  /* we can do 0 */
  cando[0] = 1;

  lv = pos = 0;
  last = 0;
  while (pos < 2000000000)
   { /* bound as stated */

    /* if last 256 were all possible, we are done */
    if (pos - last > 256) break; 

    if (!cando[lv]) 
      last = pos; /* this isn't possible, update last impossible */
    else 
     { /* this is possible */
      cando[lv] = 0; /* mark pos+256 as impossible */

      /* mark pos + size as possible for each package size */
      for (lv2 = 0; lv2 < nsize; lv2++)
        cando[(lv+sizes[lv2])%256] = 1;
     }

    /* update lv & pos */
    lv = (++pos) % 256; 
   }
  if (pos >= 2000000000) last = 0; /* shouldn't occur */

  fprintf (fout, "%i\n", last);
  return 0;
 }