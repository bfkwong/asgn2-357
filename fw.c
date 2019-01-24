/*
 *Name: Bryan Kwong
 *Instructor: P. Nico
 *Section: CPE357-05
 *Version: Asgn 2 - fw
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

   wordMap = initHashMap(5000);
   numOfWords = 10;
   argCounter = 0;
   cnt = 0;

   while(argCounter < (argc-1)) {
      argCounter += 1;
      if(strcmp(argv[argCounter], "-n") == 0) {
         if ((argCounter += 1) > (argc-1)) {
            printf("usage: fw [-n num] [ file1 [ file2 [...] ] ]\n");
            exit(EXIT_FAILURE);
         }
         numOfWords = atoi(argv[argCounter]);
         if (numOfWords == 0) {
            printf("%s: not a well-formed number\n", argv[argCounter]);
            printf("usage: fw [-n num] [ file1 [ file2 [...] ] ]\n");
            exit(EXIT_FAILURE);
         }
      } else {

         FILE *fileToProcess;
         fileToProcess = fopen(argv[argCounter], "r");
         cnt += 1;

         if(!fileToProcess) {
            perror(argv[argCounter]);
            // exit(EXIT_FAILURE);
         } else {
            wordMap = addFileToMap(fileToProcess, wordMap);
            fclose(fileToProcess);
         }
      }
   }

   if (cnt == 0) {
      wordMap = addFileToMap(stdin, wordMap);
   }

   printf("The top %d words (out of %d) are:\n", numOfWords, wordMap->numItems);
   nodeMap = hashmapToArray(wordMap);
   for (i=0; i<numOfWords; i++) {
      HashNode *maxNode;
      maxNode = NULL;
      maxNode = nodeMap[0];
      for (j=1; j<(wordMap->numItems); j++) {
         if (nodeMap[j]->value > maxNode->value) {
            maxNode = nodeMap[j];
         } else if (nodeMap[j]->value == maxNode->value) {
            if (strcmp(nodeMap[j]->key, maxNode->key) > 0) {
               maxNode = nodeMap[j];
            }
         }
      }
      if (maxNode != NULL && maxNode->value != 0) {
         printf("%9d %s\n", maxNode->value, maxNode->key);
         maxNode->value = 0;
      }

      if(i > wordMap->numItems)
         break;
   }
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
      free(currentWord);

      wordLength = 100;
      wordLocation = 0;
      currentWord = (char *)calloc(wordLength, sizeof(char));
   }

   return map;

}