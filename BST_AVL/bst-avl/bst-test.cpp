#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests

    AVLTree<int, int> testTree;

    testTree.print();

    testTree.insert(std::make_pair(-140, 9));
    testTree.print();

    testTree.insert(std::make_pair(-109, 8));
    testTree.print();

    testTree.insert(std::make_pair(-77, 9));
    testTree.print();

    testTree.insert(std::make_pair(-37, 8));
    testTree.print();

    testTree.insert(std::make_pair(12, 8));
    testTree.print();

    testTree.insert(std::make_pair(14, 159));
    testTree.print();

    testTree.insert(std::make_pair(16, 159));
    testTree.print();

    testTree.insert(std::make_pair(43, 159));
    testTree.print();

    testTree.insert(std::make_pair(48, 159));
    testTree.print();

    testTree.insert(std::make_pair(51, 159));
    testTree.print();

    testTree.insert(std::make_pair(57, 159));
    testTree.print();

    testTree.insert(std::make_pair(73, 159));
    testTree.print();

    testTree.insert(std::make_pair(83, 159));
    testTree.print();

    testTree.insert(std::make_pair(86, 159));
    testTree.print();

    testTree.insert(std::make_pair(88, 159));
    testTree.print();

    testTree.insert(std::make_pair(107, 159));

    testTree.print();
    testTree.insert(std::make_pair(118, 159));

    testTree.print();
    testTree.insert(std::make_pair(122, 159));

    testTree.print();

    testTree.insert(std::make_pair(135, 159));

    testTree.print();


    testTree.remove(43);

    testTree.print();

    testTree.remove(-77);

    testTree.print();

    testTree.remove(-37);

    testTree.print();

    testTree.remove(86);
    testTree.print();

    testTree.remove(57);
    testTree.print();

    testTree.insert(std::make_pair(50, 159));

    testTree.print();

    testTree.insert(std::make_pair(120, 159));

    testTree.print();
   

    

    BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('a',1));
    bt.insert(std::make_pair('b',2));
    
    cout << "Binary Search Tree contents:" << endl;
    for(AVLTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(bt.find('b') != bt.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    bt.remove('b');

    // AVL Tree Tests
    AVLTree<char,int> at;
    at.insert(std::make_pair('a',1));
    at.insert(std::make_pair('b',2));

    cout << "\nAVLTree contents:" << endl;
    for(AVLTree<char,int>::iterator it = at.begin(); it != at.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(at.find('b') != at.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    at.remove('b');

    return 0;
}
