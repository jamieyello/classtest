#include "stdafx.h"
#include "hashtable.h"
#include <iostream>
#include <string>
using namespace std;

//Hashtable template based off
//https://www.youtube.com/watch?v=ExuBpRu8bD4

//Goes through the table and sets the default values.
hashtable::hashtable(){
	for (int i = 0; i < tableSize; i++){
		HashT[i] = new item;
		HashT[i]->name = "empty";
		HashT[i]->drink = "empty";
		HashT[i]->next = NULL;
	}
}

void hashtable::AddItem(string name, string drink){
	int index = Hashtable(name);

	if (HashT[index]->name == "empty"){
		HashT[index]->name = name;
		HashT[index]->drink = drink;
	}
	else{
		item* Ptr = HashT[index];
		item* n = new item;
		n->name = name;
		n->drink = drink;
		n->next = NULL;

		while (Ptr->next != NULL){
			Ptr = Ptr->next;
		}
		Ptr->next = n;
	}
}

int hashtable::NumberOfItemsInIndex(int index){
	int count = 0;

	if (HashT[index]->name == "empty"){
		return count;
	}
	else{
		count++;
		item* Ptr = HashT[index];
		while (Ptr->next != NULL){
			count++;
			Ptr = Ptr->next;
		}
	}
	return count;
}

void hashtable::PrintTable(){
	int number;
	for (int i = 0; i < tableSize; i++){
		number = NumberOfItemsInIndex(i);
		cout << "--------------------\n";
		cout << "index = " << i << endl;
		cout << HashT[i]->name << endl;
		cout << HashT[i]->drink << endl;
		cout << "# of items = " << number << endl;
		cout << "--------------------\n";
	}
}

void hashtable::PrintItemsInIndex(int index){
	item * Ptr = HashT[index];

	if (Ptr->name == "empty"){
		cout << "index = " << index << " is empty";
	}
	else{
		cout << "index " << "contains the following items\n";
		while (Ptr != NULL){
			cout << "----------------\n";
			cout << Ptr->name << endl;
			cout << Ptr->drink << endl;
			cout << "----------------\n";
			Ptr = Ptr->next;
		}
	}
}



int hashtable::Hashtable(string key)
{
	int hash = 0;
	int index;
	for (int i = 0; i < key.length(); i++){
		hash += (int)key[i];

	}

	index = hash % tableSize;

	return index;
}

void hashtable::FindDrink(string name){
	int index = Hashtable(name);
	bool foundName = false;
	string drink;

	item* Ptr = HashT[index];
	while (Ptr != NULL){
		if (Ptr->name == name){
			foundName = true;
			drink = Ptr->drink;
		}
		Ptr = Ptr->next;
	}
	if (foundName == true){
		cout << "Favorite drink = " << drink << endl;
	}
	else{
		cout << name << "'s info was not found in the hash table." << endl;
	}
}

void hashtable::RemoveItem(string name){
	int index = Hashtable(name);

	item* delPtr;
	item* P1;
	item* P2;

	// Case 0 - bucket is empty
	if (HashT[index]->name == "empty" && HashT[index]->drink == "empty"){
		cout << name << " was not found in the has table.\n";
	}
	
	// Case 1 - only 1 item contained in bucket and the item has a matching name
	else if (HashT[index]->name == name && HashT[index]->next == NULL){
		HashT[index]->name = "empty";
		HashT[index]->drink = "empty";
	}

	// Case 2 - match is located in the first item in the bucket but there are more items in the bucket
	else if (HashT[index]->name == name){
		delPtr = HashT[index];
		HashT[index] = HashT[index]->next;
		delete delPtr;

		cout << name << " was removed from the hash table.\n";
	}


	// Case 3 - bucket contains items but first item is not a match
	else {
		P1 = HashT[index]->next;
		P2 = HashT[index];

		while (P1 != NULL && P1->name != name){
			P2 = P1;
			P1 = P1->next;
		}
		// Case 3.1 - looked through all the items in the bucket but there is no match
		if (P1 == NULL){
			cout << name << " was not found in the has table.\n";
		}
		// Case 3.2 - match is found
		else{
			delPtr = P1;
			P1 = P1->next;
			P2->next = P1;

			delete delPtr;
			cout << name << " was removed from the hash table.\n";
		}
	}
}

hashtable::~hashtable()
{
	item* Ptr;
	for (int i = 0; i<tableSize; i++)
	{
		while (HashT[i] != NULL)
		{
			Ptr = HashT[i];
			HashT[i] = HashT[i]->next;
			delete Ptr;
		}
	}
}