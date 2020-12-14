#pragma once

class Page
{
public:
	Page(int _order);
	void PrintValues();
	void AddValue(int _v);
	void ShiftDown();
	void PrintAllValues();
	Page* Search(int _v);
	bool InsertToPageUnc(int _v);
	int SplitLink(int i, Page * p, int _v);
	int InsertIntoArray(int* array, int& filledSize, int _v);
private:
	int order;
	int valueIdx = 0;
	int* values;
	int numberOfPossibleLinks;
	Page** links;
	friend class BTree;
};
