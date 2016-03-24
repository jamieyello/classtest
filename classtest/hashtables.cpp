#include "stdafx.h"
#include "hashtables.h"
#include <iostream>
#include <string>
using namespace std;

//Hashtable template based off
//https://www.youtube.com/watch?v=ExuBpRu8bD4

//Goes through the table and sets the default values.
hashtables::hashtables(){
	for (int i = 0; i < tableSize; i++){
		HashT[i] = new item;
		HashT[i]->name = "empty";
		HashT[i]->text = "";
		HashT[i]->next = NULL;
	}
}

void hashtables::AddItem(string name, string text){
	int index = Hashtable(name);

	if (HashT[index]->name == "empty"){
		HashT[index]->name = name;
		HashT[index]->text = text;
	}
	else{
		item* Ptr = HashT[index];
		item* n = new item;
		n->name = name;
		n->text = text;
		n->next = NULL;

		while (Ptr->next != NULL){
			Ptr = Ptr->next;
		}
		Ptr->next = n;
	}
}

int hashtables::NumberOfItemsInIndex(int index){
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

void hashtables::PrintTable(){
	int number;
	for (int i = 0; i < tableSize; i++){
		number = NumberOfItemsInIndex(i);
		cout << "--------------------\n";
		cout << "index = " << i << endl;
		cout << HashT[i]->name << endl;
		cout << HashT[i]->text << endl;
		cout << "# of items = " << number << endl;
		cout << "--------------------\n";
	}
}

void hashtables::PrintItemsInIndex(int index){
	item * Ptr = HashT[index];

	if (Ptr->name == "empty"){
		cout << "index = " << index << " is empty";
	}
	else{
		cout << "index " << "contains the following items\n";
		while (Ptr != NULL){
			cout << "----------------\n";
			cout << Ptr->name << endl;
			cout << Ptr->text << endl;
			cout << "----------------\n";
			Ptr = Ptr->next;
		}
	}
}



int hashtables::Hashtable(string key)
{
	int hash = 0;
	int index;
	for (int i = 0; i < key.length(); i++){
		hash += (int)key[i];

	}

	index = hash % tableSize;

	return index;
}

string hashtables::FindVal(string name){
	int index = Hashtable(name);
	bool foundName = false;
	string text = "";

	item* Ptr = HashT[index];
	while (Ptr != NULL){
		if (Ptr->name == name){
			foundName = true;
			text = Ptr->text;
		}
		Ptr = Ptr->next;
	}
	if (foundName == true){
		//cout << "Value = " << text << endl;
	}
	else{
		//cout << name << "'s info was not found in the hash table." << endl;
	}
	return text;
}

void hashtables::RemoveItem(string name){
	int index = Hashtable(name);

	item* delPtr;
	item* P1;
	item* P2;

	// Case 0 - bucket is empty
	if (HashT[index]->name == "empty" && HashT[index]->text == ""){
		//cout << name << " was not found in the has table.\n";
	}
	
	// Case 1 - only 1 item contained in bucket and the item has a matching name
	else if (HashT[index]->name == name && HashT[index]->next == NULL){
		HashT[index]->name = "empty";
		HashT[index]->text = "";
	}

	// Case 2 - match is located in the first item in the bucket but there are more items in the bucket
	else if (HashT[index]->name == name){
		delPtr = HashT[index];
		HashT[index] = HashT[index]->next;
		delete delPtr;

		//cout << name << " was removed from the hash table.\n";
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
			//cout << name << " was not found in the has table.\n";
		}
		// Case 3.2 - match is found
		else{
			delPtr = P1;
			P1 = P1->next;
			P2->next = P1;

			delete delPtr;
			//cout << name << " was removed from the hash table.\n";
		}
	}
}

hashtables::~hashtables()
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