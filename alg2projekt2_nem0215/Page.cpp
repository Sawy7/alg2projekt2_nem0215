#include "Page.h"
#include <iostream>
using namespace std;

Page::Page(int _order)
{
	this->order = _order;
	this->values = new int [this->order * 2];
	this->numberOfPossibleLinks = (this->order * 2) + 1;
	this->links = new Page * [this->numberOfPossibleLinks];
	for (int i = 0; i < this->numberOfPossibleLinks; i++)
	{
		this->links[i] = nullptr;
	}
}

void Page::PrintValues()
{
	cout << " | ";
	for (int i = 0; i < this->valueIdx; i++)
	{
		cout << this->values[i] << " | ";
	}
}

void Page::AddValue(int _v)
{
	if (true)
	{
		if (this->valueIdx == 0)
		{
			this->values[0] = _v;
			this->valueIdx++;
		}
		else
		{
			bool wasGreatest = true;
			int placeHere = -1;
			for (int i = 0; i < this->valueIdx; i++)
			{
				if (_v < this->values[i])
				{
					placeHere = i;
					wasGreatest = false;
					break;
				}
				else if (_v == this->values[i])
				{
					return;
				}
			}
			if (wasGreatest)
			{
				this->values[this->valueIdx] = _v;
			}
			else
			{
				for (int i = this->valueIdx; i > placeHere; i--)
				{
					this->values[i] = this->values[i - 1];
				}
				this->values[placeHere] = _v;
			}
			this->valueIdx++;
			if (this->valueIdx > (this->order * 2))
			{
				cout << "we're leaking, sir" << endl;
				ShiftDown();
			}
		}
	}
}

void Page::ShiftDown() //takhle se chova root
{
	this->links[0] = new Page(this->order);
	this->links[1] = new Page(this->order);

	for (int i = 0; i < this->order; i++)
	{
		this->links[0]->AddValue(this->values[i]);
	}
	for (int i = this->order + 1; i < this->valueIdx; i++)
	{
		this->links[1]->AddValue(this->values[i]);
	}
	this->values[0] = this->values[this->order];
	this->valueIdx = 1;

	
	//this->links[0]->PrintValues();
	//this->links[1]->PrintValues();
}

void Page::PrintAllValues()
{
	this->PrintValues();
	cout << endl;
	for (int i = 0; i < this->numberOfPossibleLinks; i++)
	{
		if (this->links[i] != nullptr)
		{
			this->links[i]->PrintValues();
			cout << "\t";
		}
	}
}

Page *Page::Search(int _v) // https://www.geeksforgeeks.org/introduction-of-b-tree-2/
{
	int searchHere = this->valueIdx;
	for (int i = 0; i < this->valueIdx; i++)
	{
		if (_v == this->values[i])
		{
			cout << "success" << endl;
			return this;
		}
		else if (_v < this->values[i])
		{
			searchHere = i;
		}
	}
	if (this->links[searchHere] != nullptr)
	{
		return links[searchHere]->Search(_v);
	}
}

bool Page::InsertToPageUnc(int _v)
{
	if (this->valueIdx < this->order*2)
	{
			if (this->valueIdx == 0)
			{
				this->values[0] = _v;
				this->valueIdx++;
			}
			else
			{
				this->InsertIntoArray(this->values, this->valueIdx, _v);
			}
			return true;
	}
	else
	{
		return false;
	}
}

int Page::SplitLink(int i, Page* p, int _v) //return value to pass up
{
	Page* newPage = new Page(this->order);
	int size = (this->order * 2);
	int* tempValues = new int[size + 1];
	
	int placeHere = this->InsertIntoArray(tempValues, size, _v);
	if (placeHere != -1)
	{
		//copy values
		for (int i = 0; i < this->order; i++)
		{
			newPage->values[i] = tempValues[i + this->order + 1];
		}
		for (int i = 0; i < this->order; i++)
		{
			newPage->links[i] = 
		}
	}
	

	

	
	return tempValues[this->order];
}

int Page::InsertIntoArray(int* array, int& filledSize, int _v)
{	
	bool wasGreatest = true;
	int placeHere = -1;
	for (int i = 0; i < filledSize; i++)
	{
		if (_v < array[i])
		{
			placeHere = i;
			wasGreatest = false;
			break;
		}
		else if (_v == array[i])
		{
			return -1;
		}
	}
	if (wasGreatest)
	{
		array[filledSize] = _v;
	}
	else
	{
		for (int i = filledSize; i > placeHere; i--)
		{
			array[i] = array[i - 1];
		}
		array[placeHere] = _v;
	}
	filledSize++;
	return placeHere;
}
