/*
 *Name: Bryan Kwong
 *Instructor: P. Nico
 *Section: CPE357-05
 *Version: Asgn 2 - fw/HashMap.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashNode HashNode;
typedef struct HashMap HashMap;

struct HashNode {
   char *key;
   int value;
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
/* Function to create a hash map
 * PARAMS: int - The initial size of the hashmap array
 * RETURN: HashMap* - A Hashmap
 */

   HashMap *dictionary;
   dictionary = (HashMap *)malloc(sizeof(HashMap));

   if(!dictionary) {
      perror("initHashMap");
      exit(EXIT_FAILURE);
   }

   dictionary->numItems = 0;
   dictionary->fullSize = fullSize;

   /* create the hash array of all 0*/
   dictionary->array = (HashNode**)calloc(fullSize, sizeof(HashNode*));

   if(!dictionary->array) {
      perror("initHashMap");
      exit(EXIT_FAILURE);
   }


   return dictionary;
}

HashMap *rehash(HashMap *dict) {
/* When the load factor of the hash table reaches 0.50, rehash is 
 * called to increase the size of the hashmap
 * PARAMS: HashMap* - A *Hashmap that will be rehashed
 * RETURNS: HashMap* - A *Hashmap that has been rehashed by a power of 2
 */

   HashMap *newMap;
   int i;

   /* create a new hashmap that will be have a sized increased by a 
      power of 2*/
   newMap = initHashMap((dict->fullSize) << 2);

   /* Loop through the old hashmap and reinsert them into the new one*/
   for(i=0; i<(dict->fullSize); i++) {
      if (dict->array[i] != NULL) {
         insert(newMap, dict->array[i]->key, dict->array[i]->value);
      }
   }

   /*free the old hashmap*/
   freeHashMap(dict);

   return newMap;
}

int hash(HashMap *dictionary, char *str) {
/* The hash function that turns a string into a location on a hashmap
 * PARAMS: char* - the string to be hashed 
 *         HashMap* - the *Hashmap that the string will be hashed into
 * RETURN: int - the hash value aka location on the array in the hashmap
 */

   int hash = 5381;
   int c;

   while ((c = *str++))
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

   if (hash < 0) {
      return - hash % (dictionary->fullSize);
   }
   return hash % (dictionary->fullSize);
}

int Althash(HashMap *dictionary, char *str) {
/* Alternative Hashing scheme created during development 
 * PARAMS: char* - the string to be hashed 
 *         HashMap* - the *Hashmap that the string will be hashed into
 * RETURN: int - the hash value aka location on the array in the hashmp
 */

   int hash = 0;
   int c;

   while ((c = *str++))
      hash = c + (hash << 6) + (hash << 16) - hash;

   if (hash < 0) {
      return - hash % (dictionary->fullSize);
   }
   return hash % (dictionary->fullSize);
}

HashMap *insert(HashMap *dictionary, char *key, int val){
/* Inserts a Hashnode consisting of the key value pair into the Hashmap.
 * If the key already exist inside the hashmap, the value of that hashnode
 * will be incremented by 1.
 * PARAMS: HashMap* - The *Hashmap to be inserted into 
 *         char* - The string that will act as the key 
 *         int - the number of occurances that a word has existed
 * RETURN: HashMap* - The hashmap with the key value pair inserted
 */

   char *cpyKey;
   double loadFactor;
   int pos;

   HashNode *currentLocation;
   HashNode *newNode;
   HashNode *list;

   cpyKey = (char *)malloc(sizeof(char)*(strlen(key)+1));
   strcpy(cpyKey, key);

   /* If load factor is greater than 0.5, rehash and increase size */
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

   /* If the hashed location is already filled, do linear probing until 
      a spot is open */
   currentLocation = list;
   while(currentLocation != 0){
      /* Compare to see if current node has the same key, it is has already
         occured, just increment 1 to the value */
      if(strcmp(currentLocation->key, cpyKey) == 0){
         currentLocation->value = currentLocation->value + 1;
         free(cpyKey);
         free(newNode);
         return dictionary;
      }
      /*Linear probing to go to the next, if end of array is reached, go 
        back to the front */
      currentLocation = (pos < (dictionary->fullSize-1))?
                         dictionary->array[pos+=1]:
                         dictionary->array[(pos = 0)];

   }

   /* If the string in question has never occured before, add the new node to
      to the hashed location*/
   dictionary->numItems += 1;
   newNode->key = cpyKey;
   newNode->value = val;
   dictionary->array[pos] = newNode;
   return dictionary;

}

void printHashMap(HashMap *dictionary) {
/* Loop through the Hashmap and print all of the nodes as 
 * key value pairs 
 * PARAMS: HashMap* - hashmap to be printed
 * RETURN: void
 */

   int i;
   printf("{ ");
   for(i=0; i<(dictionary->fullSize); i++) {
      if (dictionary->array[i] != 0) {
         printf("[\"%s\", %d], ", dictionary->array[i]->key,
                dictionary->array[i]->value);
      }
   }
   printf("\b\b}\n");

}

HashNode **hashmapToArray(HashMap *dictionary) {
/* Loop through the hashmap to find all the nodes, and then put all these 
 * nodes into an array where it is to be referred to later
 * PARAMS: HashaMap * - The HashMap to be turned into an array 
 * RETURNS: HashNode ** - An array of HashNodes with no NULL
 */
 
   HashNode **nodeMap = (HashNode**)calloc((dictionary->numItems),
                         sizeof(HashNode*));

   int i;
   int arrayCounter;
   arrayCounter = 0;

   for(i=0; i<(dictionary->fullSize)+1; i++) {
      if (dictionary->array[i] != 0) {
         nodeMap[arrayCounter] = dictionary->array[i];
         arrayCounter += 1;
      }
   }
   return nodeMap;
}

void freeHashMap(HashMap *dictionary) {
/* Free the allocated Hashmap
 * PARAMS: HashMap* - The *Hashmap to be freed
 * RETURN: void
 */

   int i;
   for(i=0; i<(dictionary->fullSize); i++) {
      if (dictionary->array[i] != 0) {
         free(dictionary->array[i]->key);
         free(dictionary->array[i]);
      }
   }
   free(dictionary->array);
   free(dictionary);
   return;

}
