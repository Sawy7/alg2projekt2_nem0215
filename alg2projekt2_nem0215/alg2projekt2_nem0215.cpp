// alg2projekt2_nem0215.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//

#include <iostream>
#include "BTree.h"
using namespace std;

int main()
{
    BTree* stromek = new BTree(2);
    
    //stromek->InsertInteractive();
    // vkladani
    stromek->Insert(3);
    stromek->Insert(22);
    stromek->Insert(10);
    stromek->Insert(2);
    stromek->Insert(17);
    stromek->Insert(5);
    stromek->Insert(66);
    stromek->Insert(68);
    stromek->Insert(50);
    stromek->Insert(7);
    stromek->Insert(55);
    stromek->Insert(45);
    stromek->Insert(70);
    stromek->Insert(44);
    stromek->Insert(6);
    stromek->Insert(21);
    stromek->Insert(67);
    stromek->Insert(1);
    stromek->Insert(4);
    stromek->Insert(8);
    stromek->Insert(9);
    stromek->Insert(12);
    stromek->Insert(15);
    stromek->Insert(46);

    stromek->Insert(100);
    stromek->Insert(150);
    stromek->Insert(140);
    stromek->Insert(130);
    stromek->Insert(125);
    stromek->Insert(123);
    stromek->Insert(69);

    // mazani
    stromek->Remove(3,false);
    stromek->Remove(7,false);
    stromek->Remove(8,false);
    stromek->Remove(6,false);
    stromek->Remove(22,false);
    stromek->Remove(46,false);
    stromek->Remove(1,false);
    stromek->Remove(17,false);
    stromek->Remove(55,false);
    stromek->Remove(44,false);
    stromek->Remove(4,false);
    stromek->Remove(45,false);
    stromek->Remove(15,false);
    stromek->Remove(9,false);
    stromek->Remove(10,false);
    stromek->Remove(67,false);
    stromek->Remove(2,false);
    stromek->Remove(70,false);
    stromek->Remove(21,false);
    stromek->Remove(5,false);

    delete stromek;

    return 0;
}

