#include "TreePage.h"

/// Konstruktor tridy TreePage - vytvari prazdny list dle zadaneho radu
TreePage::TreePage(int o, bool l)
{
	this->order = o;
	this->minKeys = o;
	this->maxKeys = o * 2;
	this->leaf = l;
	this->n = 0;
	this->keys = new int[this->maxKeys];
	this->ChildPages = new TreePage * [this->maxKeys + 1];
}

/// Metoda, ktera vlozi klic do konkretni stranky; podobne jako zbytek "insert chainu" inspirace odsud: https://www.geeksforgeeks.org/insert-operation-in-b-tree/
void TreePage::InsertKey(int value)
{
	if (leaf)
	{
		InsertUnconditionally(value);
	}
	else
	{
		int placeHere = this->n;
		for (int i = 0; i < this->n; i++)
		{
			if (value < this->keys[i])
			{
				placeHere = i;
				break;
			}
		}
		if (this->ChildPages[placeHere]->n == maxKeys)
		{
			this->SplitChild(placeHere, this->ChildPages[placeHere]);
			if (value <= this->keys[placeHere])
			{
				this->ChildPages[placeHere]->InsertKey(value);
				this->keys[placeHere] = this->ChildPages[placeHere]->keys[this->ChildPages[placeHere]->n - 1];
				this->ChildPages[placeHere + 1]->ChildPages[0] = this->ChildPages[placeHere]->ChildPages[this->ChildPages[placeHere]->n]; //bruuuuuuuuh
				this->ChildPages[placeHere]->n--;
			}
			else
			{
				this->ChildPages[placeHere + 1]->InsertKey(value);
				this->keys[placeHere] = this->ChildPages[placeHere + 1]->keys[0];
				for (int i = 0; i < this->ChildPages[placeHere + 1]->n - 1; i++)
				{
					this->ChildPages[placeHere + 1]->keys[i] = this->ChildPages[placeHere + 1]->keys[i + 1];
				}
				for (int i = 0; i < this->ChildPages[placeHere + 1]->n; i++)
				{
					this->ChildPages[placeHere + 1]->ChildPages[i] = this->ChildPages[placeHere + 1]->ChildPages[i + 1];
				}
				this->ChildPages[placeHere + 1]->n--;
			}
		}
		else
		{
			this->ChildPages[placeHere]->InsertKey(value);
		}
	}
}

/// Metoda, ktera vlozi klic do stranky bezpodminecne - bez jakehokoliv rozhodovani ho tam proste "placne" (je nutno uzivat s rozumem, soucast jinych metod)
void TreePage::InsertUnconditionally(int value)
{
	bool wasGreatest = true;
	int placeHere = -1;
	for (int i = 0; i < this->n; i++)
	{
		if (value < this->keys[i])
		{
			placeHere = i;
			wasGreatest = false;
			break;
		}
		else if (value == this->keys[i])
		{
			return;
		}
	}
	if (wasGreatest)
	{
		this->keys[this->n] = value;
	}
	else
	{
		for (int i = this->n; i > placeHere; i--)
		{
			this->keys[i] = this->keys[i - 1];
		}
		this->keys[placeHere] = value;
	}
	this->n++;
}


/// Metoda, ktera rozdeli potomka konkretni stranky; reseni cerpa z: https://www.geeksforgeeks.org/insert-operation-in-b-tree/
void TreePage::SplitChild(int x, TreePage* p)
{
	/// Vytvoreni nove stranky
	TreePage* z = new TreePage(p->minKeys, p->leaf);
	z->n = this->minKeys;

	/// Kopirovani klicu do nove stranky
	for (int i = 0; i < this->minKeys; i++)
	{
		z->keys[i] = p->keys[i + this->minKeys];
	}

	/// Kopirovani potomku do nove stranky
	for (int i = 0; i < this->minKeys + 1; i++)
	{
		z->ChildPages[i] = p->ChildPages[i + this->minKeys];
	}

	/// Zmena poctu klicu v puvodnim potomkovi
	p->n = this->minKeys;

	/// Posunuti potomku v rodici (odkud spoustime tuto metodu)
	for (int i = this->n; i >= x + 1; i--)
	{
		this->ChildPages[i + 1] = ChildPages[i];
	}

	/// Zarazeni nove stranky jako potomka rodice (odkud spoustime tuto metodu)
	this->ChildPages[x + 1] = z;

	/// Posunuti klicu v rodici (odkud spoustime tuto metodu)
	for (int i = this->n - 1; i >= x; i--)
	{
		this->keys[i + 1] = keys[i];
	}

	/// Pridani noveho klice do rodice (odkud spoustime tuto metodu) - toto se bude menit pozdeji v pridavnem kodu v jinych metodach (preusporadani)
	this->keys[x] = p->keys[p->n]; // latest hotfix 16-12-2020
	this->n++;
}

/// Destruktor - uplne jednoduchy
TreePage::~TreePage()
{
	delete[] this->keys;
	this->keys = nullptr;

	if (!leaf)
	{
		for (int i = 0; i < (this->n + 1); i++)
		{
			delete this->ChildPages[i];
		}
	}

	delete[] this->ChildPages;
	this->ChildPages = nullptr;
}

/// Metoda, ktera proste odebere klic z konkretni stranky - pouziva se jako soucast jinych metod, protoze je casto nutne provest preusporadani stromu
void TreePage::SimpleRemove(int value)
{
	int idxToRemove = 0;
	for (int i = 0; i < this->n; i++)
	{
		if (this->keys[i] == value)
		{
			idxToRemove = i;
			break;
		}
	}
	for (int i = idxToRemove + 1; i < this->n; i++)
	{
		this->keys[i - 1] = this->keys[i];
	}
	for (int i = idxToRemove + 1; i < this->n + 1; i++)
	{
		this->ChildPages[i - 1] = this->ChildPages[i];
	}
	this->n--;
}