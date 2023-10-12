/*
Class: ESE 333
Instructor Daniel Benz
Student: Kyle Han
Project 1
Item: Linked list
Description: Create the modify and delete functions to make this sorted linked list effective
Last Modified: 2023-08-30
Github: https://github.com/Kyleh2420/
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct node
{
	struct node* next;
	char* data;
};


struct node* insert(struct node* current, char* value);

struct node* delete(struct node* list, char* value);
