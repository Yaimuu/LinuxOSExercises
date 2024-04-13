#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
   int i, n;
   time_t t;

   n = 10;

   int haystack [n];

   /* Intializes random number generator */
   srand((unsigned) time(&t));

   /* Print 5 random numbers from 0 to 50 */
   for( i = 0 ; i < n ; i++ ) 
   {
      printf("%d\n", rand() % 50);
      haystack[i] = rand();
      printf("%d\n", haystack[i]);

   }

   return(0);
}