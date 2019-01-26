/*
 *Name: Bryan Kwong
 *Instructor: P. Nico
 *Section: CPE357-05
 *Version: Asgn 2 - fw/fw.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "HashMap.c"

HashMap *addFileToMap(FILE *fileToProcess, HashMap *map);

int main(int argc, char **argv) {

   int numOfWords;
   HashMap *wordMap;
   HashNode **nodeMap;
   int argCounter;
   int i, j, cnt;
   char *strtolPtr;

   /* Initalize a hashmap to store words */
   wordMap = initHashMap(10000);
   /*Initialize the default number of words to print*/
   numOfWords = 10;
   /*Keeping a separate argument counter to prevent corrupting original one*/
   argCounter = 0;
   /*Keep count of how many files were processed*/
   cnt = 0;

   /* Loop through terminal arguments */
   while(argCounter < (argc-1)) {
      argCounter += 1;

      if(strcmp(argv[argCounter], "-n") == 0) {
         /*If -n is encountered, go to the next arg to check for number*/
         if ((argCounter += 1) > (argc-1)) {
            printf("usage: fw [-n num] [ file1 [ file2 [...] ] ]\n");
            exit(EXIT_FAILURE);
         }
         numOfWords = atoi(argv[argCounter]);
         /*If atoi returns zero, check to see if it is an error or a int 0*/
         if (numOfWords == 0 && argv[argCounter][0] != 48) {
            printf("%s: not a well-formed number\n", argv[argCounter]);
            printf("usage: fw [-n num] [ file1 [ file2 [...] ] ]\n");
            exit(EXIT_FAILURE);
         }
      } else {
         /*open file to see if the file exists, increment file count*/
         FILE *fileToProcess;
         fileToProcess = fopen(argv[argCounter], "r");
         cnt += 1;

         if(!fileToProcess) {
            perror(argv[argCounter]);
         } else {
            /*add all the words in the file to the hashmap*/
            wordMap = addFileToMap(fileToProcess, wordMap);
            fclose(fileToProcess);
         }
      }
   }

   /*If no files were given aka file count (cnt) is 0, check stdin*/
   if (cnt == 0) {
      wordMap = addFileToMap(stdin, wordMap);
   }

   /*prepare outputs to stdout*/
   printf("The top %d words (out of %d) are:\n", numOfWords, wordMap->numItems);
   /*turn the hashmap into an array so no time would be wasted looping through
     NULL*/
   nodeMap = hashmapToArray(wordMap);

   /*Loop through the array to find the n most frequent words */
   for (i=0; i<numOfWords; i++) {

      /*establish a hashnode pointer that points to the current max*/
      HashNode *maxNode;
      maxNode = NULL;
      maxNode = nodeMap[0];
      /*loop through array to see if a higher value is found, if it is found 
        replace the maximum node with this new found higher valued node */ 
      for (j=1; j<(wordMap->numItems); j++) {
         if (nodeMap[j]->value > maxNode->value) {
            maxNode = nodeMap[j];
         } else if (nodeMap[j]->value == maxNode->value) {
            if (strcmp(nodeMap[j]->key, maxNode->key) > 0) {
               maxNode = nodeMap[j];
            }
         }
      }
      /* if a new Maxnode is found, set the maxnode value to 0 and print
         its contents. Setting to 0 ensures it will not be displayed again*/
      if (maxNode != NULL && maxNode->value != 0) {
         printf("%9d %s\n", maxNode->value, maxNode->key);
         maxNode->value = 0;
      }

      /*If no more words exists break the loop*/
      if(i > wordMap->numItems)
         break;
   }
   
   /*free the hashmap and the allocated array*/
   freeHashMap(wordMap);
   free(nodeMap);
   return 0;
}

HashMap *addFileToMap(FILE *fileToProcess, HashMap *map) {

   int newChar;

   int wordLocation;
   int wordLength;
   char *currentWord;

   wordLocation = 0;
   wordLength = 1000;
   currentWord = (char *)calloc(wordLength, sizeof(char));

   if(!currentWord) {
      perror("addToHashMap");
      printf("The top 10 words (out of 0) are:\n" );
      exit(EXIT_FAILURE);
   }

   while ((newChar = fgetc(fileToProcess)) != EOF) {
      newChar = tolower(newChar);
      if (wordLocation >= (wordLength - 1)) {
         currentWord = realloc(currentWord, wordLength *= 2);
         if(!currentWord) {
            perror("addToHashMap");
            exit(EXIT_FAILURE);
         }
      }
      if (isalpha(newChar)) {
         currentWord[wordLocation++] = newChar;
      } else if (strlen(currentWord) != 0) {
         map = insert(map, currentWord, 1);
         free(currentWord);

         wordLength = 100;
         wordLocation = 0;
         currentWord = (char *)calloc(wordLength, sizeof(char));
      }
   }

   if(strlen(currentWord) != 0) {
      map = insert(map, currentWord, 1);
   }
   free(currentWord);

   return map;

}
