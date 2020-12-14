#include "BTree.h"

/// Konstruktor tridy BTree - vytvari prazdny strom dle zadaneho radu
BTree::BTree(int o)
{
	this->order = o;
	this->minKeys = o;
	this->maxKeys = o * 2;
	this->root = nullptr;
	this->keyCount = 0;
}

/// Metoda, kterou volame, kdyz chceme pridat klic do stromu; podstatna cast reseni cerpa z: https://www.geeksforgeeks.org/insert-operation-in-b-tree/
void BTree::Insert(int value)
{
	if (this->root == nullptr) /// Zjistuje, jestli existuje korenova stranka
	{
		this->root = new TreePage(this->order, true);
		this->root->InsertKey(value);
	}
	else
	{
		if (this->FindKey(value, this->root) != nullptr)
		{
			cout << "Klic " << value << " uz tady je." << endl;
			return;
		}
		if (this->root->n == maxKeys) /// Kdyz je korenova stranka plna
		{
			TreePage* newRoot = new TreePage(this->order, false);

			newRoot->ChildPages[0] = this->root;

			newRoot->SplitChild(0, this->root); /// Rozdeleni puvodniho korenu

			if (value <= newRoot->keys[0]) /// Pokud je novy klic mensi nez klic v novem koreni, pak bude pridan do leveho potomka
			{
				newRoot->ChildPages[0]->InsertKey(value);
				newRoot->keys[0] = newRoot->ChildPages[0]->keys[newRoot->ChildPages[0]->n - 1]; /// Nejvetsi klic z leveho potomka vyplave do korene
				newRoot->ChildPages[0]->n--;
			}
			else
			{
				newRoot->ChildPages[1]->InsertKey(value);
				newRoot->keys[0] = newRoot->ChildPages[1]->keys[0]; /// Nejmensi klic z praveho potomka vyplave do korene a ostatni klice a potomci se shiftnou doleva
				for (int i = 0; i < newRoot->ChildPages[1]->n - 1; i++)
				{
					newRoot->ChildPages[1]->keys[i] = newRoot->ChildPages[1]->keys[i + 1];
				}
				newRoot->ChildPages[1]->n--;
				for (int i = 0; i < newRoot->ChildPages[1]->n + 1; i++)
				{
					newRoot->ChildPages[1]->ChildPages[i] = newRoot->ChildPages[1]->ChildPages[i + 1];
					newRoot->ChildPages[newRoot->ChildPages[1]->n + 1] = nullptr; //lastfix
				}
			}
			this->root = newRoot; /// Konecne nahrazujeme puvodni koren novym
		}
		else
		{
			this->root->InsertKey(value); /// Pokud neni korenova stranka plna, proste vlozime klic
			//this->PrintTree();
		}
	}
	/// Upozorneni a vizualizace
	cout << "Vkladani klice " << value << ":" << endl;
	this->PTIMP();
	cout << endl;
}

/// Metoda pro volani z mainu - spousti jednoduchou vizualizaci stromu
void BTree::PrintTree()
{
	this->PrintTree(this->root, "|");
}

/// Metoda pro jednoduchou vizualizaci stromu - funguje pekne, ale da chvili zabrat, nez se v tom clovek zorientuje
void BTree::PrintTree(TreePage* tp, string prefix)
{
	cout << prefix;
	for (int i = 0; i < tp->n; i++)
	{
		cout << tp->keys[i] << " ";
	}
	cout << endl;
	if (tp->leaf)
	{
		return;
	}
	else
	{
		prefix = prefix + "-";
		for (int i = 0; i < tp->n + 1; i++)
		{
			this->PrintTree(tp->ChildPages[i], prefix);
		}
	}
}

/// Metoda pro volani z mainu - spousti funkci, ktera vrati vysku celeho stromu
int BTree::GetTreeHeight()
{
	return this->GetTreeHeight(this->root, 0);
}

/// Metoda, ktera umoznuje zadavat klice k pridani primo z command line
void BTree::InsertInteractive()
{
	while (true)
	{
		char mode;
		int toAddOrRemove;
		cout << "Pridavame, nebo mazeme? Napiste '+[cislo]', '-[cislo]', nebo 'k' pro ukonceni: ";
		cin >> mode >> toAddOrRemove;
		if (mode == '+')
		{
			this->Insert(toAddOrRemove);
		}
		else if (mode == '-')
		{
			this->Remove(toAddOrRemove,false);
		}
		else if (mode == 'k')
		{
			cout << "Nazdar!" << endl;
			return;
		}
		else
		{
			cout << "Zkus to znovu" << endl;
		}
	}
}

/// Metoda, ktera vraci vysku celeho podstromu od zvole stranky (jako koren)
int BTree::GetTreeHeight(TreePage* tp, int height)
{
	height++;
	if (tp->leaf)
	{
		return height;
	}
	else
	{
		return GetTreeHeight(tp->ChildPages[0], height);
	}
}

/// Soucast komplexnejsiho, ale zato mnohem hezciho a prehlednejsiho vizualniho zobrazeni stromu
void BTree::PTIMP()
{
	int height = this->GetTreeHeight();
	int spacing = this->CharsOnLevel(height - 1, this->root) - 1;
	for (int i = 0; i < spacing; i++)
	{
		cout << "=";
	}
	cout << endl;
	for (int i = 0; i < height; i++)
	{
		if (i != height - 1)
		{
			int indent = (spacing / 2) - ((this->CharsOnLevel(i, this->root) - 1) / 2);
			for (int j = 0; j < indent; j++)
			{
				cout << " ";
			}
		}
		PTIMP(i, this->root);
		cout << endl;
		if (i < height - 1) cout << endl;
	}
	for (int i = 0; i < spacing; i++)
	{
		cout << "=";
	}
	cout << endl;
}

/// Metoda, ktera vytiskne na standardni vystup urcitou uroven stromu (0. uroven = koren, 1. uroven = vsechny stranky, co jsou pod korenem, ...)
bool BTree::PTIMP(int level, TreePage* tp)
{
	if (level == 0)
	{
		cout << "|";
		for (int i = 0; i < tp->n; i++)
		{
			cout << tp->keys[i] << "|";
		}
		cout << " ";
	}
	else
	{
		for (int i = 0; i < tp->n + 1; i++)
		{
			this->PTIMP(level - 1, tp->ChildPages[i]);
		}
	}
	return true;
}

/// Metoda slouzici k zjisteni poctu znaku, ktery zabira dana uroven pri vypisu na obrazovku - soucast PTIMP()
int BTree::CharsOnLevel(int level, TreePage* tp)
{
	int numOfChars = 0;
	if (level == 0)
	{
		int tempChars = 0;
		for (int i = 0; i < tp->n; i++) /// Cast zjistujici pocet cifer daneho intu jsem prevzal a upravil pro sve potreby z: https://stackoverflow.com/questions/1489830/efficient-way-to-determine-number-of-digits-in-an-integer
		{
			int number = tp->keys[i];
			int digits = 0;
			if (number < 0) digits = 1;
			while (number) {
				number /= 10;
				digits++;
			}
			tempChars += digits;
		}
		return tempChars + tp->n + 2;
	}
	else
	{
		for (int i = 0; i < tp->n + 1; i++)
		{
			numOfChars += this->CharsOnLevel(level - 1, tp->ChildPages[i]);
		}
	}
	return numOfChars;
}

/// Metoda volajici jinou, ktera poskytne pocet klicu stromu - hlavne pro vyuziti v mainu bez parametru
int BTree::CountKeys()
{
	if (this->root != nullptr)
	{
		return this->CountKeys(this->root);
	}
	else
	{
		cout << "Strom nema zadne stranky." << endl;
		return -1;
	}
}

/// Metoda, ktera spocita vsechny klice stromu, aktualizuje this->keyCount a tuto hodnotu take vrati
int BTree::CountKeys(TreePage* tp)
{
	this->keyCount += tp->n;
	if (tp->leaf == false)
	{
		for (int i = 0; i < tp->n + 1; i++)
		{
			this->CountKeys(tp->ChildPages[i]);
		}
	}
	return this->keyCount;
}

/// Tato metoda vola dalsi metodu, ktera poskytne informaci o nalezeni/nenalezeni klice ve stromu - hlavne pro vyuziti v mainu
void BTree::FindKey(int value)
{
	if (this->root != nullptr)
	{
		cout << "Klic o hodnote " << value;
		if (this->FindKey(value, this->root) != nullptr)
		{
			cout << " nalezen.";
		}
		else
		{
			cout << " nenalezen.";
		}
		cout << endl;
	}
	else
	{
		cout << "Strom nema zadne stranky." << endl;
	}
}

/// Tato metoda hleda klic ve stromu - pokud ho najde, tak vrati list, ve kterem se nachazi; pokud ne, vrati misto listu nullptr
TreePage* BTree::FindKey(int value, TreePage* tp)
{
	int lastSmallerIdx = 0;
	for (int i = 0; i < tp->n; i++)
	{
		if (tp->keys[i] < value)
		{
			lastSmallerIdx = i + 1;
		}
		else if (tp->keys[i] == value)
		{
			return tp;
		}
	}
	if (tp->leaf)
	{
		return nullptr;
	}
	else
	{
		return this->FindKey(value, tp->ChildPages[lastSmallerIdx]);
	}
}

/// Metoda, ktera odstrani zvoleny klic ze stromu a provede veskerou dalsi cinnost, ktera zaruci, ze je strom po tomto zakroku spravne rozlozen
void BTree::Remove(int value, bool onlyRebalance)
{
	/// Hledani rodicovske stranky
	TreePage* r = FindParentPage(value, this->root, this->root);


	if (r != nullptr)
	{
		/// Urcovani stranky s klicem, ktery chceme smazat
		int lastSmallerIdx = 0;
		for (int i = 0; i < r->n; i++)
		{
			if (r->keys[i] < value) /// Nalezeni indexu, na kterem se nachazi p v potomcich rodice
			{
				lastSmallerIdx = i + 1;
			}
			else if (r->keys[i] == value) /// Pokud je klic v rodicovske strance, tak nastavime p jako rodice
			{
				lastSmallerIdx = -1;
				break;
			}
		}
		TreePage* p = r; /// Nastaveni p stejne jako rodice
		int pChildIdx = -1;
		if (lastSmallerIdx != -1) /// Nastaveni p, pokud je jine nez rodic
		{
			p = r->ChildPages[lastSmallerIdx];
			pChildIdx = lastSmallerIdx;
		}
		TreePage* s = nullptr;
		int sChildIdx;
		if (lastSmallerIdx + 1 > r->n) /// Nastaveni sourozence vlevo, pokud ten "vetsi" neexistuje
		{
			s = r->ChildPages[lastSmallerIdx - 1];
			sChildIdx = lastSmallerIdx - 1;
		}
		else
		{
			s = r->ChildPages[lastSmallerIdx + 1]; /// Nastaveni sourozence vpravo
			sChildIdx = lastSmallerIdx + 1;
		}

		bool justReplacing = false;

		if (p->leaf || onlyRebalance) /// Pokud je stranka list nebo jenom vyvazujeme strom
		{
			if (p->n > p->minKeys && !onlyRebalance) /// Stranka bude mit po odstraneni clena aspon tolik klicu, kolikateho je strom radu
			{
				p->SimpleRemove(value);
			}
			else if (s->n > s->minKeys) /// Sourozenec ma vice klicu, nez kolikateho je strom radu
			{
				if (!onlyRebalance)
				{
					p->SimpleRemove(value);
				}
				if (pChildIdx + 1 == sChildIdx) /// Kdyz je sourozenec vpravo
				{
					p->InsertUnconditionally(r->keys[pChildIdx]);
					p->ChildPages[p->n] = s->ChildPages[0]; /// Presunuti childPage ze sourozence
					r->keys[pChildIdx] = s->keys[0];
					s->SimpleRemove(s->keys[0]);
				}
				else /// Kdyz je sourozenec vlevo
				{
					p->InsertUnconditionally(r->keys[sChildIdx]);
					for (int i = p->n; i > 0; i--)
					{
						p->ChildPages[i] = p->ChildPages[i - 1];
					}
					p->ChildPages[0] = s->ChildPages[s->n]; /// Presunuti childPage ze sourozence TBD
					r->keys[sChildIdx] = s->keys[s->n - 1];
					s->SimpleRemove(s->keys[s->n - 1]);
				}
			}
			else if (s->n == s->minKeys) /// Kdyz ma sourozenec presne tolik klicu, kolikateho je strom radu
			{
				if (!onlyRebalance) p->SimpleRemove(value);
				TreePage* tempP;
				TreePage* tempS;
				int tempChildIdx;
				if (pChildIdx + 1 == sChildIdx) /// Kdyz je sourozenec vpravo
				{
					tempP = p;
					tempS = s;
					tempChildIdx = pChildIdx;
				}
				else /// Kdyz je sourozenec vlevo
				{
					tempP = s;
					tempS = p;
					tempChildIdx = sChildIdx;
				}
				/// Pridani klice z rodicovske stranky
				tempP->InsertUnconditionally(r->keys[tempChildIdx]);
				/// Tento loop prenese vsechny potomky ze sourozence
				for (int i = 0; i < tempS->n + 1; i++)
				{
					tempP->ChildPages[tempP->n + i] = tempS->ChildPages[i];
				}
				/// Presunuti vsech klicu ze sourozence
				for (int i = 0; i < tempS->n; i++)
				{
					tempP->n++;
					tempP->keys[tempP->n - 1] = tempS->keys[i];
				}

				for (int i = tempChildIdx + 1; i < r->n; i++)
				{
					r->keys[i - 1] = r->keys[i];
				}
				for (int i = tempChildIdx + 2; i < r->n + 1; i++)
				{
					r->ChildPages[i - 1] = r->ChildPages[i];
				}
				r->n--;
			}
		}
		else /// Pokud stranka neni list
		{
			/// Nalezeni nejblizsiho vetsiho klice
			int keyIndex = 0;
			for (int i = 0; i < p->n; i++)
			{
				if (p->keys[i] == value)
				{
					keyIndex = i;
				}
			}
			int closestGreater = this->FindFirstGreater(value, p->ChildPages[keyIndex + 1]);
			this->Remove(closestGreater, false); /// Transformace na problem odstraneni klice v listu
			
			TreePage* tmp = this->FindKey(value, this->root);
			for (int i = 0; i < tmp->n; i++)
			{
				if (tmp->keys[i] == value)
				{
					tmp->keys[i] = closestGreater;
					justReplacing = true;
					break;
				}
			}

			//p->keys[keyIndex] = closestGreater;
		}

		if (r != this->root && r->n < r->minKeys && !justReplacing) /// Pokud rodicovska stranka neni zaroven korenem stromu a obsahuje mene klicu, nez kolikateho je strom radu a zaroven se nejedna pouze o transformovany problem odstraneni klice v listu
		{
			this->Remove(r->keys[0], true); /// Dochazi k preusporadani
		}

		if (this->root->n == 0) /// Pokud nema koren zadne klice
		{
			if (pChildIdx + 1 == sChildIdx) /// Je za koren prohlasen 'p'
			{
				this->root = p;
			}
			else
			{
				this->root = s; /// Je za koren prohlasen 's'
			}
		}
	}
	else
	{
		cout << "Klic " << value << " nebyl nalezen, a proto nemuze byt odebran." << endl; /// Upozorneni v pripade mazani neexistujiciho klice
		return;
	}
	if (!onlyRebalance) /// Vypis statistiky o mazani
	{
		cout << "Mazani klice " << value << ":" << endl;
		this->PTIMP();
		cout << endl;
	}
}

/// Metoda, ktera nalezne rodice stranky, ve ktere se nachazi zvoleny klic
TreePage* BTree::FindParentPage(int value, TreePage* p, TreePage* r)
{
	int lastSmallerIdx = 0;
	for (int i = 0; i < p->n; i++)
	{
		if (p->keys[i] < value)
		{
			lastSmallerIdx = i + 1;
		}
		else if (p->keys[i] == value)
		{
			return r;
		}
	}
	if (p->leaf)
	{
		return nullptr;
	}
	else
	{
		return this->FindParentPage(value, p->ChildPages[lastSmallerIdx], p);
	}
}

/// Metoda, ktera najde prvni klic, ktery je vetsi nez zvolena hodnota - toto je vylozene pro pouziti uvnitr funkce Remove() a nikde jinde to asi nema smysl
int BTree::FindFirstGreater(int value, TreePage* tp) /// Je nutno spustit od "prave" stranky - to je ta, co ma vyssi hodnoty klicu, nez nami zadana hodnota (osetreno v Remove())
{
	int lastSmallerIdx = 0;
	for (int i = 0; i < tp->n; i++)
	{
		if (tp->keys[i] < value)
		{
			lastSmallerIdx = i + 1;
		}
	}
	if (tp->leaf)
	{
		return tp->keys[0];
	}
	else
	{
		return this->FindFirstGreater(value, tp->ChildPages[lastSmallerIdx]);
	}
}

/// Destruktor - uplne jednoduchy
BTree::~BTree()
{
	delete this->root;
	this->root = nullptr;
}
