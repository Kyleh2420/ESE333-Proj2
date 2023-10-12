/*
Class: ESE 333
Instructor Daniel Benz
Student: Kyle Han
Project 2
Item: Linked list
Description: Create the modify and delete functions to make this sorted linked list effective
Last Modified: 2023-08-30
Github: https://github.com/Kyleh2420/
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "linkedList.h"

struct node* insert(struct node* current, char* value)
{
	//This function will insert the string value at a given point in the node
	//If there exists a node afterwards, it will relink it.
	//If the inserted node is at the end. the next character will not be set
	

	if (value == NULL) {
		return current;
	}

	//Allocate some memory for the new node
	struct node* newNode = (struct node*)malloc(sizeof(struct node));

	//Allocate some memory for the new node's data
	newNode->data = (char*)malloc(sizeof(value));

	//Set the new node to the previous node's next
	newNode->next = current->next;

	//Add the string data into newNode
	strcpy(newNode->data, value);

	//move the current->next to point to the new node we just created
	current->next = newNode;

	return newNode;

	/*
	//Store the head somewhere
	//We will search the linked list for the proper place to put value, then insert it.
	//When found, note the pointer. This pointer becomes the inserted values next. 
	//Will return the head of the entire list
	

	//Create the variables previous, current, and toInsert
	struct node* previous;
	previous = malloc(sizeof(struct node));
	previous = list;

	struct node* current;
	current = malloc(sizeof(struct node));
	current = list;
	
	struct node* toInsert;
	toInsert = malloc(sizeof(struct node));
	toInsert->data = value;

	//While there still exists a list, follow until the first instance where the number is larger than the given value
	while (current->next != NULL) {
	//	printf("Current value: %d \n", current-> data);
		if (current->data > value) {
			//If these two are the same, we are at the very beginning of the list. Change the pointers accordingly
			if (current == previous) {
				list = toInsert;
				toInsert->next = current;
				previous = toInsert;
			} else {
				//Otherwise, we can operate as standard
				previous->next = toInsert;
				toInsert->next = current;
			}
			return list;
		}
		//Move along the linked list
		previous = current;
		current = current->next;

	}

	//If at this point, we have not inserted the value, then it must go at the very end
	current->next = toInsert;
	toInsert->next = NULL;
	return list;
	*/
	
};

struct node* delete(struct node* list, char* value)
{
	//Put your code here
	//list should have the head of the linked list
	//value is the value to be deleted: loop through the entire list looking for value
	//Once found, free the memory and change around the pointers
	

	struct node* previous;
	previous = malloc(sizeof(struct node));
	previous = list;

	struct node* current;
	current = malloc(sizeof(struct node));
	current = list;

	while (current->next != NULL) {
		if (current -> data == value) {
		//If the item to delete is the first in the list
			if (current == previous) {
				list = current->next;
				free (current);
				return list;
			} else {
				previous->next = current -> next;
				free (current);
				return list;
			}
			//Move along the linked list
		}
		previous = current;
		current = current->next;

	}
	//At this point we still have not checked the last value. Do that now
	if (current -> data == value) {
		previous->next = current -> next;
		free (current);
		return list;
	}
	
	return list;

};
