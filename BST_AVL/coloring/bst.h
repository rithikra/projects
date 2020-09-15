#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>


template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};




template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{

}


template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}


template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}


template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}


template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}


template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}


template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}


template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}


template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}


template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}


template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}


template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}


template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}


template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}




template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    virtual void insert(const std::pair<const Key, Value>& keyValuePair);
    virtual void remove(const Key& key);
    void clear();
    bool isBalanced() const;
    void print() const;
    bool empty() const;

    template<typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue> & tree);
public:
    
    class iterator
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;
    Value& operator[](const Key& key);
    Value const & operator[](const Key& key) const;

protected:
    Node<Key, Value>* internalFind(const Key& k) const;
    Node<Key, Value> *getSmallestNode() const;
    static Node<Key, Value>* predecessor(Node<Key, Value>* current);
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;
    void clearer(Node<Key,Value>* to_delete);
    bool isBalancedRecursive(Node<Key,Value>* root) const; 
    int isBalancedHelper(Node<Key,Value>* current_node) const; 



protected:
    Node<Key, Value>* root_;
};




template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr)
{
    current_ = ptr;
}


template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() 
{
    current_ = NULL;

}


template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}


template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}


template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return (current_ == (rhs.current_));
}


template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return (current_ != (rhs.current_));

}



template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
    Node<Key,Value>* temp = current_;
    Node<Key, Value>* curr_parent = current_->getParent();
    if (current_->getRight() == NULL){
        while (curr_parent != NULL && current_ == curr_parent->getRight()){
            current_ = curr_parent;
            if (curr_parent->getParent() == NULL){
                current_ = NULL;
                return *this;
            }
            curr_parent = curr_parent->getParent();
        }
        if (current_->getParent() != NULL && current_ == current_->getParent()->getLeft()){
            current_ = curr_parent;
            return *this;
        }

    }
    else if (current_->getRight() != NULL){
        current_ = current_->getRight();
        while (current_->getLeft() != NULL){
            current_ = current_->getLeft();
        }
        return *this;
    }
    if (current_ == temp){
        current_ = NULL;;
    }
    return *this;
}







template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() 
{
    root_ = NULL;
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    clear();

}


template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}


template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}


template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}


template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}


template<class Key, class Value>
Value& BinarySearchTree<Key, Value>::operator[](const Key& key)
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}
template<class Key, class Value>
Value const & BinarySearchTree<Key, Value>::operator[](const Key& key) const
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}


template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    if (root_ == NULL){
        Node<Key, Value>* new_node = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);
        root_ = new_node;
    }
    else{
        Node<Key, Value>* temp_node = root_;
        while (temp_node != NULL){
            Node<Key, Value>* right_ = temp_node->getRight();
            Node<Key, Value>* left_ = temp_node->getLeft();
            if (keyValuePair.first > temp_node->getItem().first && right_ != NULL){
                temp_node = right_;
            }
            else if (keyValuePair.first < temp_node->getItem().first && left_ != NULL){
                temp_node = left_;
            }
            else if (keyValuePair.first == temp_node->getItem().first){
                temp_node->setValue(keyValuePair.second);
                break;
            }
            else if (keyValuePair.first > temp_node->getItem().first && right_ == NULL){
                Node<Key, Value>* new_node = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, temp_node);
                temp_node->setRight(new_node);
                break;
            }
            else if (keyValuePair.first < temp_node->getItem().first && left_ == NULL){
                Node<Key, Value>* new_node = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, temp_node);
                temp_node->setLeft(new_node);
                break;
            }
        }
    }
}



template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    if (root_ == NULL){
        return;
    }
    Node<Key,Value>* remove_node = internalFind(key);
    if (remove_node==NULL){
        return;
    }
    while (remove_node->getLeft() != NULL && remove_node->getRight() != NULL){
        Node <Key, Value>* swapper = predecessor(remove_node);
        nodeSwap(remove_node, swapper);
    }
    if (remove_node == root_ && remove_node->getLeft() == NULL && remove_node->getRight() == NULL){
        root_ = NULL;
        delete remove_node;
    }
    else if (remove_node->getRight() == NULL && remove_node->getLeft() == NULL){
        Node <Key, Value>* parent_ = remove_node->getParent();
        if (parent_->getLeft() == remove_node){
            parent_->setLeft(NULL);
        }
        else if (parent_->getRight() == remove_node){
            parent_->setRight(NULL);
        }
        delete remove_node;
    }
    else if (remove_node->getRight() == NULL){
        Node <Key, Value>* parent_ = remove_node->getParent();
        Node <Key, Value>* left_child = remove_node->getLeft();
        if (parent_ == NULL){
            left_child->setParent(NULL);
            root_ = left_child;
        }
        else if (parent_->getRight() == remove_node){
            parent_->setRight(left_child);
        }
        else if (parent_->getLeft() == remove_node){
            parent_->setLeft(left_child);
        }
        left_child->setParent(parent_);
        delete remove_node;
    }
     else if (remove_node->getLeft() == NULL){
        Node <Key, Value>* parent_ = remove_node->getParent();
        Node <Key, Value>* right_child = remove_node->getRight();
        if (parent_ == NULL){
            right_child->setParent(NULL);
            root_ = right_child;
        }
        else if (parent_->getRight() == remove_node){
            parent_->setRight(right_child);
        }
        else if (parent_->getLeft() == remove_node){
            parent_->setLeft(right_child);
        }
        right_child->setParent(parent_);
        delete remove_node;
    }
}



template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    Key key = current->getItem().first;
    Node<Key, Value>* temp_node = current;
    while (temp_node != NULL){
        if (temp_node->getItem().first == key){
            if (temp_node->getLeft() != NULL){
                temp_node = temp_node->getLeft();
                while (temp_node->getRight() != NULL){
                    temp_node = temp_node->getRight();
                }
                break;
            }
        }
        else if (temp_node->getItem().first > key){
            temp_node = temp_node->getLeft();
        }
        else{
            temp_node = temp_node->getRight();
        }
    }
    return temp_node;
}



template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    if (empty()){
        return;
    }
    clearer(root_);
    root_ = NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearer(Node<Key,Value>* to_delete){
    if (to_delete == NULL){
        return;
    }
    if (to_delete->getLeft() != NULL){
        clearer(to_delete->getLeft());
    }
    if (to_delete->getRight() != NULL){
        clearer(to_delete->getRight());
    }
    delete to_delete;
}



template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    Node<Key,Value>* current = root_;
    while (current->getLeft() != NULL){
        current = current->getLeft();
    }
    return current;
}


template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    if (root_ == NULL){
        return NULL;
    }
    else if (root_->getItem().first == key){
        return root_;
    }
    else{
        Node<Key, Value>* temp_node = root_;
        while (temp_node != NULL){
            Node<Key, Value>* right_ = temp_node->getRight();
            Node<Key, Value>* left_ = temp_node->getLeft();
            if (key > temp_node->getItem().first && right_ != NULL){
                temp_node = right_;
            }
            else if (key < temp_node->getItem().first && left_ != NULL){
                temp_node = left_;
            }
            else if (key == temp_node->getItem().first){
                return temp_node;
            }
            else if (key > temp_node->getItem().first && right_ == NULL){
                return NULL;
            }
            else if (key < temp_node->getItem().first && left_ == NULL){
                return NULL;
            }
        }
        return temp_node;
    }
}


template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    bool balance = isBalancedRecursive(root_);
    return balance;
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalancedRecursive(Node<Key,Value>* root) const
{
    if(root == NULL){
        return true; 
    }
    
    int left; 
    int right;
    left=isBalancedHelper(root->getLeft()); 
    right=isBalancedHelper(root->getRight());
    if ((abs(left - right) <= 1) 
     && isBalancedRecursive(root->getLeft()) && isBalancedRecursive(root->getRight())) {
        return true; 
    }
    return false;

}

template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::isBalancedHelper(Node<Key,Value>* current_node) const
{
    if (current_node == NULL){
        return 0;
    }
    int maximum = (std::max(isBalancedHelper(current_node->getLeft()), isBalancedHelper(current_node->getRight())));
    return 1 + maximum; 
}



template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }

}


#include "print_bst.h"



#endif
