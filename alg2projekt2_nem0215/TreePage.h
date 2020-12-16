#pragma once
#include <iostream>
using namespace std;

class TreePage
{
public:
	TreePage(int o, bool l);
	void InsertKey(int value);
	void InsertUnconditionally(int value);
	void SplitChild(int x, TreePage* p);
	~TreePage();
private:
	/// Rad stromu - maximalni pocet klicu v jakekoliv strance je dvojnasobek zvoleneho radu (vychazim z definice z ALG2 materialu)
	int order;
	int minKeys;
	int maxKeys;
	/// Pole klicu pro konkretni stranku - zvolil jsem cela cisla (int)
	int* keys;
	/// Pocet klicu pro konkretni stranku
	int n;
	/// Potomci konkretni stranky - ty stranky, ktere se hierarchicky nachazeji pod tou aktualni
	TreePage** ChildPages;
	/// Hodnota urcujici, jestli je dana stranka 
	bool leaf;
	/// Hodnota urcujici, jestli je dana stranka koren
	bool root;

	void SimpleRemove(int value);
	friend class BTree;
};

