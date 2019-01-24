#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashNode HashNode;
typedef struct HashMap HashMap;

struct HashNode {
   char *key;
   int value;
   HashNode *next;
};

struct HashMap {
   int numItems;
   int fullSize;
   HashNode **array;
};

HashMap *initHashMap(int fullSize);
HashMap *rehash(HashMap *dictionary);
int hash(HashMap *dictionary, char *str);
HashMap *insert(HashMap *dictionary, char *key, int val);
void printHashMap(HashMap *dictionary);
void freeHashMap(HashMap *map);
void freeHashArray(HashNode **array, int numOfItems);

HashMap *initHashMap(int fullSize) {
   HashMap *dictionary;
   dictionary = (HashMap *)malloc(sizeof(HashMap));

   if(!dictionary) {
      perror("initHashMap");
      exit(EXIT_FAILURE);
   }

   dictionary->numItems = 0;
   dictionary->fullSize = fullSize;

   dictionary->array = (HashNode**)calloc(fullSize, sizeof(HashNode*));

   if(!dictionary->array) {
      perror("initHashMap");
      exit(EXIT_FAILURE);
   }


   return dictionary;
}

HashMap *rehash(HashMap *dict) {
   HashMap *newMap;
   int i;

   newMap = initHashMap((dict->fullSize) << 2);

   for(i=0; i<(dict->fullSize); i++) {
      if (dict->array[i] != NULL) {
         insert(newMap, dict->array[i]->key, dict->array[i]->value);
      }
   }

   freeHashMap(dict);

   return newMap;
}

int hash(HashMap *dictionary, char *str) {
   int hash = 5381;
   int c;

   while ((c = *str++))
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

   if (hash < 0) {
      return - hash % (dictionary->fullSize);
   }
   return hash % (dictionary->fullSize);
}

HashMap *insert(HashMap *dictionary, char *key, int val){
   char *cpyKey;
   double loadFactor;
   int pos;
   HashNode *currentLocation;
   HashNode *newNode;
   HashNode *list;

   cpyKey = (char *)malloc(sizeof(char)*strlen(key));
   strcpy(cpyKey, key);

   loadFactor = (double)(dictionary->numItems)/(double)(dictionary->fullSize);
   if (loadFactor >= 0.5) {
      dictionary = rehash(dictionary);
   }

   pos = hash(dictionary, cpyKey);
   list = dictionary->array[pos];
   newNode = (HashNode *)malloc(sizeof(HashNode));

   if(!newNode) {
      perror("insert");
      exit(EXIT_FAILURE);
   }

   currentLocation = list;
   while(currentLocation != NULL){
      if(strcmp(currentLocation->key, cpyKey) == 0){
         currentLocation->value = currentLocation->value + 1;
         free(cpyKey);
         free(newNode);
         return dictionary;
      }
      currentLocation = (pos < dictionary->fullSize)?
                         dictionary->array[pos+=1]:
                         dictionary->array[0];
      // temp = temp->next;
   }
   dictionary->numItems += 1;
   newNode->key = cpyKey;
   newNode->value = val;
   newNode->next = list;
   dictionary->array[pos] = newNode;
   return dictionary;
}

void printHashMap(HashMap *dictionary) {
   int i;
   printf("{ ");
   for(i=0; i<(dictionary->fullSize); i++) {
      if (dictionary->array[i] != NULL) {
         printf("[\"%s\", %d], ", dictionary->array[i]->key,
                dictionary->array[i]->value);
      }
   }
   printf("\b\b}\n");

}

HashNode **hashmapToArray(HashMap *dictionary) {

   HashNode **nodeMap = (HashNode**)calloc((dictionary->numItems),
                         sizeof(HashNode*));

   int i;
   int arrayCounter;
   arrayCounter = 0;

   for(i=0; i<(dictionary->fullSize); i++) {
      if (dictionary->array[i] != NULL) {
         nodeMap[arrayCounter] = dictionary->array[i];
         arrayCounter += 1;
      }
   }
   return nodeMap;
}

void freeHashMap(HashMap *dictionary) {

   int i;
   for(i=0; i<(dictionary->fullSize); i++) {
      if (dictionary->array[i] != NULL) {
         free(dictionary->array[i]->key);
         free(dictionary->array[i]);
      }
   }
   free(dictionary->array);
   free(dictionary);
   return;

}

void freeHashArray(HashNode **array, int numOfItems) {

   free(array);
}

