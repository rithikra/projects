#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };


template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();
    char getBalance () const;
    void setBalance (char balance);
    void updateBalance(char diff);
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;


protected:
    char balance_;
};




template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}


template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}


template<class Key, class Value>
char AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}


template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(char balance)
{
    balance_ = balance;
}


template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(char diff)
{
    balance_ += diff;
}


template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}


template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}


template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}





template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item);
    virtual void remove(const Key& key);
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    
    void insertfix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child);
    void removefix(AVLNode<Key,Value>* parent, int diff);
    void rotateright(AVLNode<Key,Value>* node);
    void rotateleft(AVLNode<Key,Value>* node);



};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateleft(AVLNode<Key,Value>* node){
    AVLNode<Key,Value>* grand = node->getParent();
    AVLNode<Key,Value>* child = node->getRight();
    AVLNode<Key,Value>* left_grand = child->getLeft();
    node->setParent(child);
    child->setLeft(node);
    node->setRight(left_grand);
    if (left_grand != NULL){
        left_grand->setParent(node);
    }
    if (grand != NULL){
        if (grand->getRight() == node){
            grand->setRight(child);
            child->setParent(grand);

        }
        if (grand->getLeft() == node){
            grand->setLeft(child);
            child->setParent(grand);    
        }
    }
    else if (grand == NULL){
        this->root_ = child;
        child->setParent(NULL);
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateright(AVLNode<Key,Value>* node){
    AVLNode<Key,Value>* grand = node->getParent();
    AVLNode<Key,Value>* child = node->getLeft();
    AVLNode<Key,Value>* right_grand = (child->getRight());
    node->setParent(child);
    child->setRight(node);
    node->setLeft(right_grand);
    if (right_grand != NULL){
        right_grand->setParent(node);
    }
    if (grand != NULL){
        if (grand->getRight() == node){
            grand->setRight(child);
            child->setParent(grand);

        }
        if (grand->getLeft() == node){
            grand->setLeft(child);
            child->setParent(grand);    
        }
    }
    else if (grand == NULL){
        this->root_ = child;
        child->setParent(NULL);
    }

}


template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    AVLNode<Key, Value>* temp_node;
    int direction_added = 0;
    if (this->root_ == NULL){
        AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        new_node->setBalance(0);
        this->root_ = new_node;
    }
    else{
        Node<Key,Value>* temp= this->root_;
        temp_node = (AVLNode<Key,Value>*) temp;
        while (temp_node != NULL){
            AVLNode<Key, Value>* right_ = temp_node->getRight();
            AVLNode<Key, Value>* left_ = temp_node->getLeft();
            if (new_item.first > temp_node->getItem().first && right_ != NULL){
                temp_node = right_;
            }
            else if (new_item.first < temp_node->getItem().first && left_ != NULL){
                temp_node = left_;
            }
            else if (new_item.first == temp_node->getItem().first){
                temp_node->setValue(new_item.second);
                break;
            }
            else if (new_item.first > temp_node->getItem().first && right_ == NULL){
                AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, temp_node);
                new_node->setBalance(0);
                temp_node->setRight(new_node);
                direction_added = 1;
                break;
            }
            else if (new_item.first < temp_node->getItem().first && left_ == NULL){
                AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, temp_node);
                new_node->setBalance(0);
                temp_node->setLeft(new_node);
                direction_added = 2;
                break;
            }
        }
        if (direction_added != 0){
            if ((int) temp_node->getBalance() == 1 || (int) temp_node->getBalance() == -1){
                temp_node->setBalance(0);
            }
            else if ((int) temp_node->getBalance() == 0){
                if (direction_added == 1){
                    temp_node->setBalance(1);
                    insertfix(temp_node, temp_node->getRight());
                }
                else if (direction_added == 2){
                    temp_node->setBalance(-1);
                    insertfix(temp_node, temp_node->getLeft());
                }
            }
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertfix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child){
     AVLNode<Key,Value>* g_parent = parent->getParent();
    if (parent == NULL || g_parent == NULL){
        return;
    }
    if (g_parent != NULL){
        if (g_parent->getLeft() == parent){
            g_parent->updateBalance(-1);
            if (((int) g_parent->getBalance()) == 0){
                return;
            }
            else if (((int) g_parent->getBalance()) == -1){
                insertfix(g_parent, parent);
            }
            else if (((int) g_parent->getBalance()) == -2){
                if(parent->getLeft() == child){
                    rotateright(g_parent);
                    g_parent->setBalance(0);
                    parent->setBalance(0);
                }
                else if (parent->getRight() == child){
                    rotateleft(parent);
                    rotateright(g_parent);
                    if (((int) child->getBalance()) == -1){
                        parent->setBalance(0);
                        g_parent->setBalance(1);
                        child->setBalance(0);
                    }
                    else if (((int) child->getBalance()) == 0){
                        parent->setBalance(0);
                        g_parent->setBalance(0);
                        child->setBalance(0);
                    }
                    else if (((int) child->getBalance()) == 1){
                        parent->setBalance(-1);
                        g_parent->setBalance(0);
                        child->setBalance(0);
                    }
                }
            }
        }
        else if (g_parent->getRight() == parent){
            g_parent->updateBalance(1);
            if ((int) g_parent->getBalance() == 0){
                return;
            }
            else if ((int) g_parent->getBalance() == 1){
                insertfix(g_parent, parent);
            }
            else if (g_parent->getBalance() == 2){
                if(parent->getRight() == child){
                    rotateleft(g_parent);
                    g_parent->setBalance(0);
                    parent->setBalance(0);
                }
                else if (parent->getLeft() == child){
                    rotateright(parent);
                    rotateleft(g_parent);
                    if (((int) child->getBalance()) == 1){
                        parent->setBalance(0);
                        g_parent->setBalance(-1);
                        child->setBalance(0);
                    }
                    else if (((int) child->getBalance()) == 0){
                        parent->setBalance(0);
                        g_parent->setBalance(0);
                        child->setBalance(0);
                    }
                    else if (((int) child->getBalance()) == -1){
                        parent->setBalance(1);
                        g_parent->setBalance(0);
                        child->setBalance(0);
                    }
                }
            }
        }
    }

}


template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    if (this->root_ == NULL){
        return;
    }
    AVLNode<Key,Value>* remove_node = (AVLNode<Key,Value>*) BinarySearchTree<Key,Value>::internalFind(key);
    if (remove_node==NULL){
        return;
    }
    if (remove_node == this->root_ && remove_node->getLeft() == NULL && remove_node->getRight() == NULL){
        this->root_ = NULL;
        delete remove_node;
        return;
    }
    if (remove_node->getLeft() != NULL && remove_node->getRight() != NULL){
        AVLNode <Key, Value>* swapper = (AVLNode<Key,Value>*) BinarySearchTree<Key,Value>::predecessor(remove_node);
        this->nodeSwap(remove_node, swapper);
    }
    if (remove_node->getRight() == NULL && remove_node->getLeft() == NULL){
        AVLNode <Key, Value>* parent_ = remove_node->getParent();
        int direction = 0;
        if (parent_->getLeft() == remove_node){
            parent_->setLeft(NULL);
            direction = 1;
        }
        else if (parent_->getRight() == remove_node){
            parent_->setRight(NULL);
            direction = -1;
        }
        delete remove_node;
        removefix(parent_, direction);
    }
    else if (remove_node->getRight() == NULL){
        AVLNode <Key, Value>* parent_ = remove_node->getParent();
        AVLNode <Key, Value>* left_child = remove_node->getLeft();
        int direction = 0;
        if (parent_ == NULL){
            left_child->setParent(NULL);
            this->root_ = left_child;
        }
        else if (parent_->getRight() == remove_node){
            parent_->setRight(left_child);
            direction = -1;
        }
        else if (parent_->getLeft() == remove_node){
            parent_->setLeft(left_child);
            direction = 1;
        }
        left_child->setParent(parent_);
        delete remove_node;
        removefix(parent_, direction);
    }
     else if (remove_node->getLeft() == NULL){
        AVLNode <Key, Value>* parent_ = remove_node->getParent();
        AVLNode <Key, Value>* right_child = remove_node->getRight();
        int direction = 0;
        if (parent_ == NULL){
            right_child->setParent(NULL);
            this->root_ = right_child;
        }
        else if (parent_->getRight() == remove_node){
            parent_->setRight(right_child);
            direction = -1;
        }
        else if (parent_->getLeft() == remove_node){
            parent_->setLeft(right_child);
            direction = 1;
        }
        right_child->setParent(parent_);
        delete remove_node;
        removefix(parent_, direction);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removefix(AVLNode<Key,Value>* parent, int diff){
    if (parent == NULL){
        return;
    }
    int nextdiff = 0;
    AVLNode<Key,Value>* grand = parent->getParent();
    if (grand != NULL){
        if (grand->getLeft() == parent){
            nextdiff = 1;
        }
        else if (grand->getRight() == parent){
            nextdiff = -1;
        }
    }
    if (diff == -1){
        int current_diff = ((int) parent->getBalance()) + diff;
        if (current_diff == -1){
            parent->setBalance(-1);
            return;
        }
        else if(current_diff == 0){
            parent->setBalance(0);
            removefix(grand, nextdiff);
        }
        else if (current_diff == -2){
            AVLNode<Key,Value>* curr = parent->getLeft();
            if (((char)curr->getBalance()) == -1){
                rotateright(parent);
                curr->setBalance(0);
                parent->setBalance(0);
                removefix(grand, nextdiff);
            }
            else if (((char)curr->getBalance()) == 0){
                rotateright(parent);
                curr->setBalance(1);
                parent->setBalance(-1);
            }
            else if (((char)curr->getBalance()) == 1){
                AVLNode<Key,Value>* child = curr->getRight();
                rotateleft(curr);
                rotateright(parent);
                if (((int) child->getBalance())== 1){
                    child->setBalance(0);
                    parent->setBalance(0);
                    curr->setBalance(-1);
                }

                else if (((int) child->getBalance())== 0){
                    child->setBalance(0);
                    parent->setBalance(0);
                    curr->setBalance(0);
                }

                else if (((int) child->getBalance())== -1){
                    child->setBalance(0);
                    parent->setBalance(1);
                    curr->setBalance(0);
                }
                removefix(grand, nextdiff);
            }
        }
    }
    else if (diff == 1){
        int current_diff = ((int) parent->getBalance()) + diff;
        if (current_diff == 1){
            parent->setBalance(1);
            return;
        }
        else if(current_diff == 0){
            parent->setBalance(0);
            removefix(grand, nextdiff);
        }
        else if (current_diff == 2){
            AVLNode<Key,Value>* curr = parent->getRight();
            if (((char)curr->getBalance()) == 1){
                rotateleft(parent);
                curr->setBalance(0);
                parent->setBalance(0);
                removefix(grand, nextdiff);
            }
            else if (((char)curr->getBalance()) == 0){
                rotateleft(parent);
                curr->setBalance(-1);
                parent->setBalance(1);
            }

            else if (((char)curr->getBalance()) == -1){
                AVLNode<Key,Value>* child = curr->getLeft();
                rotateright(curr);
                rotateleft(parent);
                if (((int) child->getBalance())== -1){
                    child->setBalance(0);
                    parent->setBalance(0);
                    curr->setBalance(1);
                }

                else if (((int) child->getBalance())== 0){
                    child->setBalance(0);
                    parent->setBalance(0);
                    curr->setBalance(0);
                }

                else if (((int) child->getBalance())== 1){
                    child->setBalance(0);
                    parent->setBalance(-1);
                    curr->setBalance(0);
                }
                removefix(grand, nextdiff);
            }
        }
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
