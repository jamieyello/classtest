#pragma once
#include <iostream>
#include <string>
using namespace std;


class hashtables
{
private:
	static const int tableSize = 4;
	
	struct item{
		string name;
		std::string text;
		item* next;
	};

	item* HashT[tableSize];

public:
	hashtables();
	~hashtables();
	int Hashtable(string key);
	void AddItem(string name, string text);
	int NumberOfItemsInIndex(int index);
	void PrintTable();
	void PrintItemsInIndex(int index);
	string FindVal(string name);
	void RemoveItem(string name);
};
