/*--------------------------------------------------------------------*/
/* replace.c                                                          */
/* Author: JunJie                                                     */
/*--------------------------------------------------------------------*/

#include "str.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/*--------------------------------------------------------------------*/

/* If pcFrom is the empty string, then write string pcLine to stdout
   and return 0.  Otherwise write string pcLine to stdout with each
   distinct occurrence of string pcFrom replaced with string pcTo,
   and return a count of how many replacements were made.  Make no
   assumptions about the maximum number of replacements or the
   maximum number of characters in strings pcLine, pcFrom, or pcTo. */

static size_t replaceAndWrite(const char *pcLine,
                              const char *pcFrom, const char *pcTo)
{
   assert(pcLine != NULL && pcFrom != NULL && pcTo != NULL);
   if(*pcFrom == '\0') {
      fprintf(stdout, "%s", pcLine);
      return 0;
   }
   size_t lengthFrom = Str_getLength(pcFrom);
   size_t lengthTo = Str_getLength(pcTo);

   char *original = pcLine;
   char *start;
   int count = 0;
   while((start = Str_search(original, pcFrom)) != NULL) {
      count++;
      // output to the start of first pcFrom
      char tmp = *start;
      *start = '\0';
      fprintf(stdout, "%s", original);
      *start = tmp; 

      // output pcTo and jump the length of pcFrom 
      fprintf(stdout, "%s", pcTo);
      original += lengthFrom;
   }
   // for the newline character
   fprintf(stdout, "%s", original);
   return count;
}

/*--------------------------------------------------------------------*/

/* If argc is unequal to 3, then write an error message to stderr and
   return EXIT_FAILURE.  Otherwise...
   If argv[1] is the empty string, then write each line of stdin to
   stdout, write a message to stderr indicating that 0 replacements
   were made, and return 0.  Otherwise...
   Write each line of stdin to stdout with each distinct occurrence of
   argv[1] replaced with argv[2], write a message to stderr indicating
   how many replacements were made, and return 0.
   Assume that no line of stdin consists of more than MAX_LINE_SIZE-1
   characters. */

int main(int argc, char *argv[])
{
   enum {MAX_LINE_SIZE = 4096};
   enum {PROPER_ARG_COUNT = 3};

   char acLine[MAX_LINE_SIZE];
   char *pcFrom;
   char *pcTo;
   size_t uReplaceCount = 0;

   if (argc != PROPER_ARG_COUNT)
   {
      fprintf(stderr, "usage: %s fromstring tostring\n", argv[0]);
      return EXIT_FAILURE;
   }

   pcFrom = argv[1];
   pcTo = argv[2];

   while (fgets(acLine, MAX_LINE_SIZE, stdin) != NULL)
      /* Insert your code here. */
      uReplaceCount += replaceAndWrite(acLine, pcFrom, pcTo);

   fprintf(stderr, "%lu replacements\n", (unsigned long)uReplaceCount);
   return 0;
}