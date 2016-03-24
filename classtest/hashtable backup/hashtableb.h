#pragma once
#include <iostream>
#include <string>
using namespace std;


class hashtable
{
private:
	static const int tableSize = 4;
	
	struct item{
		string name;
		string drink;
		item* next;
	};

	item* HashT[tableSize];

public:
	hashtable();
	~hashtable();
	int Hashtable(string key);
	void AddItem(string name, string drink);
	int NumberOfItemsInIndex(int index);
	void PrintTable();
	void PrintItemsInIndex(int index);
	void FindDrink(string name);
	void RemoveItem(string name);
};
