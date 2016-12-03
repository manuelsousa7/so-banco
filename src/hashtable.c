/******************************************************************************************
* File Name:    hashtable.c
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     #include <hashtable.h> - Prototipos e estruruas da funcoes da hashtable
*
* DESCRIPTION:  Funcoes da hashtable utilizadas no i-banco
* DIAGNOSTICS:  OK
*****************************************************************************************/

#include "hashtable.h"


int hashCode(int key) {
	return key % SIZE;
}

struct DataItem *search(int key) {
	/* get the hash  */
	int hashIndex = hashCode(key);

	/* move in array until an empty */
	while (hashArray[hashIndex] != NULL) {

		if (hashArray[hashIndex]->key == key)
			return hashArray[hashIndex];

		/* go to next cell */
		++hashIndex;

		/* wrap around the table */
		hashIndex %= SIZE;
	}

	return NULL;
}

void insert(int key, int data) {
	/* obtem a hash */
	int hashIndex = hashCode(key);
	struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
	item->data = data;
	item->key = key;

	/* move in array until an empty or deleted cell */
	while (hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
		/* go to next cell */
		++hashIndex;

		/* wrap around the table */
		hashIndex %= SIZE;
	}

	hashArray[hashIndex] = item;
}

void display() {
	int i = 0;

	for (i = 0; i < SIZE; i++) {

		if (hashArray[i] != NULL)
			printf(" (%d,%d)", hashArray[i]->key, hashArray[i]->data);
		else
			printf(" ~~ ");
	}

	printf("\n");
}


void freeHash() {
	int i = 0;

	for (i = 0; i < SIZE; i++) {
		if (hashArray[i] != NULL)
			free(hashArray[i]);
	}
}


struct DataItem* delete(struct DataItem* item) {
	int key = item->key;

	/* get the hash */
	int hashIndex = hashCode(key);

	/* move in array until an empty */
	while (hashArray[hashIndex] != NULL) {

		if (hashArray[hashIndex]->key == key) {
			struct DataItem* temp = hashArray[hashIndex];

			/* assign a dummy item at deleted position */
			hashArray[hashIndex] = dummyItem;
			return temp;
		}

		/* go to next cell */
		++hashIndex;

		/* wrap around the table */
		hashIndex %= SIZE;
	}

	return NULL;
}
