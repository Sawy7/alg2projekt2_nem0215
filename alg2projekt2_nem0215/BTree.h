#pragma once
#include "TreePage.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class BTree
{
public:
	BTree(int o);
	void Insert(int value);
	void PrintTree();
	int CountKeys();
	void FindKey(int value);
	void Remove(int value, bool onlyRebalance);
	void PTIMP();
	int GetTreeHeight();
	void InsertInteractive();
	~BTree();
private:
	/// Rad stromu - maximalni pocet klicu v jakekoliv strance je dvojnasobek zvoleneho radu (vychazim z definice z ALG2 materialu)
	int order;
	int minKeys;
	int maxKeys;
	/// Koren stromu (stranka)
	TreePage* root;
	/// Celkovy pocet klicu stromu - k aktualizaci NEDOCHAZI automaticky, ale pouze pri spusteni metody CountKeys()
	int keyCount;

	void PrintTree(TreePage* tp, string prefix);
	int CountKeys(TreePage* tp);
	TreePage* FindKey(int value, TreePage* tp);
	TreePage* FindParentPage(int value, TreePage* p, TreePage* r);
	int FindFirstGreater(int value, TreePage* tp);
	int GetTreeHeight(TreePage* tp, int height);
	bool PTIMP(int level, TreePage* tp);
	int CharsOnLevel(int level, TreePage* tp);
};

