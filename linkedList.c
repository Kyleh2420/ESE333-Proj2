/*
Class: ESE 333
Instructor Daniel Benz
Student: Kyle Han
Project 2
Item: Linked list
Description: Delete and insert a linked list with string data
Last Modified: 2023-10-11
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

}

struct node* delete(struct node* list, char* value)
{

}
