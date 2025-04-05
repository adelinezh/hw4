#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    AVLNode<Key, Value>* baseInsert(const std::pair<const Key, Value> &keyValuePair);
    void leftRotate(AVLNode<Key, Value>* node);
    void rightRotate(AVLNode<Key, Value>* node);
    int getBalance(AVLNode<Key, Value>* node);
    int height(AVLNode<Key, Value>* node) const;
    AVLNode<Key, Value>* baseRemove(const Key& key);
    //void baseRemove(const Key& key);
    AVLNode<Key, Value>* internalFind(const Key& k) const;
    AVLNode<Key, Value>* predecessor(AVLNode<Key, Value>* current);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* insertedNode = baseInsert(new_item);
   AVLNode<Key, Value>* current = insertedNode -> getParent();


   while(current)
   {
       int balance = getBalance(current);
       if (balance > 1)
       {
           int tempBalance = getBalance(current -> getLeft());
           if (tempBalance < 0)
           {
               leftRotate(current -> getLeft());
           }
           rightRotate(current);
       }
       else if (balance < -1)
       {
           int tempBalance = getBalance(current -> getRight());
           if (tempBalance > 0)
           {
               rightRotate(current -> getRight());
           }
           leftRotate(current);
       }
       current = current -> getParent();
   }

}

template<class Key, class Value>
int AVLTree<Key, Value>::getBalance(AVLNode<Key, Value>* node)
{
    if(!node)
    {
        return 0;
    }

    return (height(node -> getLeft()) - height(node -> getRight()));
}

template<typename Key, typename Value>
int AVLTree<Key, Value>::height(AVLNode<Key, Value>* node) const
{
    if (!node)
    {
        return 0;
    }


    int lHeight = height(node -> getLeft());
    int rHeight = height(node -> getRight());

    return std::max(lHeight, rHeight) + 1;
}

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value>* n)
{
    AVLNode<Key, Value>* tempRight = n -> getRight();
    if (!tempRight || !n)
    {
        return;
    }
    AVLNode<Key, Value>* tempLeft = tempRight -> getLeft();

    tempRight -> setLeft(n);
    n -> setRight(tempLeft);

    if(tempLeft)
    {
        tempLeft -> setParent(n);
    }

    tempRight -> setParent(n -> getParent());

    if (!(n -> getParent()))
    {
        this -> root_ = tempRight;
    }
    else if (n == n -> getParent() -> getLeft())
    {
        n -> getParent() -> setLeft(tempRight);
    }
    else
    {
        n -> getParent() -> setRight(tempRight);
    }

    n -> setParent(tempRight);
    int8_t leftBal = tempLeft ? tempLeft -> getBalance() : 0;
   int8_t rightBal = tempRight ? tempRight -> getBalance() : 0;
 
   if (rightBal <= 0)
   {
        if (tempLeft)
        {
            tempLeft -> setBalance(leftBal + 1 - std::min(leftBal, (int8_t) 0));
        }
        if (tempRight && tempLeft)
        {
            tempRight -> setBalance(rightBal + 1 - std::max(tempLeft -> getBalance(), (int8_t)0));
        }
       
       
   }
   else
   {
    if (tempLeft)
    {
        tempLeft -> setBalance(leftBal + 1);
    }
    if (tempRight)
    {
        tempRight -> setBalance(leftBal + 1);
    }
       
   } 
}

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value>* n)
{
    AVLNode<Key, Value>* tempLeft = n -> getLeft();
    if (!tempLeft || !n)
    {
        return;
    }

    AVLNode<Key, Value>* tempRight = tempLeft -> getRight();

    tempLeft -> setRight(n);
    n -> setLeft(tempRight);

    if(tempRight)
    {
        tempRight -> setParent(n);
    }

    tempLeft -> setParent(n -> getParent());

    if (!(n -> getParent()))
    {
        this -> root_ = tempLeft;
    }
    else if (n == n -> getParent() -> getRight())
    {
        n -> getParent() -> setRight(tempLeft);
    }
    else
    {
        n -> getParent() -> setLeft(tempLeft);
    }

    n -> setParent(tempLeft);
    int8_t leftBal = tempLeft ? tempLeft -> getBalance() : 0;
   int8_t rightBal = tempRight ? tempRight -> getBalance() : 0;
 
   if (rightBal <= 0)
   {
        if (tempLeft)
        {
            tempLeft -> setBalance(leftBal + 1 - std::min(leftBal, (int8_t) 0));
        }
        if (tempRight && tempLeft)
        {
            tempRight -> setBalance(rightBal + 1 - std::max(tempLeft -> getBalance(), (int8_t)0));
        }
   }
   else
   {
    if (tempLeft)
    {
        tempLeft -> setBalance(leftBal + 1);
    }
    if (tempRight)
    {
        tempRight -> setBalance(leftBal + 1);
    }
   }

}



template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::baseInsert(const std::pair<const Key, Value> &keyValuePair)
{
    // TODO
    AVLNode <Key, Value>* newNode = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, nullptr);
    if (!this -> root_)
    {
        this -> root_ = newNode;
        return newNode;
    }

    AVLNode <Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this -> root_);
    AVLNode <Key, Value>* parent = current;
    
    while (current)
    {
        parent = current;
        if (keyValuePair.first > current -> getKey())
        {
            current = current -> getRight();
        }
        else if (keyValuePair.first < current -> getKey())
        {
            current = current -> getLeft();
        }
        else
        {
            current -> getValue() = keyValuePair.second;
            delete newNode;
            return current;
        }
    }

    newNode -> setParent(parent);
    if (keyValuePair.first < parent -> getKey())
    {
        parent -> setLeft(newNode);
    }
    if (keyValuePair.first > parent -> getKey())
    {
        parent -> setRight(newNode);
    }
    return newNode;
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* toRemove = internalFind(key);
    if (!toRemove)
    {
        return;
    }
    
    AVLNode<Key, Value>* current = baseRemove(key);
    //int balance_ = getBalance(current);

    while(current)
    {
        int balance_ = getBalance(current);
        if (balance_ > 1)
        {
            if (getBalance(current -> getLeft()) < 0)
            {
                leftRotate(current -> getLeft());
            }
            rightRotate(current);
            //balance_ = getBalance(current);
        }
        
        else if (balance_ < -1)
        {
            if (getBalance(current -> getRight()) > 0)
            {
                rightRotate(current -> getRight());
            }
            leftRotate(current);
            //balance_ = getBalance(current);
        }
        current = current -> getParent();
    }
}

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::baseRemove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* current = internalFind(key);
    
    if (!current)
    {
        return nullptr;
    }
    AVLNode<Key, Value>* parent = current -> getParent();
    
    if (current -> getLeft() && current -> getRight())
    {
        //swap with predecessor
        Node<Key, Value>* pred = predecessor(current);
        if (pred)
        {
          nodeSwap(static_cast<AVLNode<Key, Value>*>(pred), static_cast<AVLNode<Key, Value>*>(current));
          //current -> setParent(current -> parent);
        }
    }

    
    AVLNode<Key, Value>* child = nullptr;
    if (!(current -> getLeft() && current -> getRight()))
    {
        if (current -> getLeft())
        {
            child = current -> getLeft();
        }
        else if (current -> getRight())
        {
            child = current -> getRight();
        }
    }
  
    if (current -> getParent())
    {
        if (current -> getParent() -> getLeft() == current)
        {
            current -> getParent() -> setLeft(child);
        }
        else
        {
            current -> getParent() -> setRight(child);
        }
    }
    else
    {
        this -> root_ = child;
    }
    if (child)
    {
        child -> setParent(current -> getParent());
    }
    delete current;
    return parent;
}

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::internalFind(const Key& key) const
{
    // TODO
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this -> root_);
    while (current)
    {
        if (key == current -> getKey())
        {
            return current;
        }
        else if (key > current -> getKey())
        {
            current = current -> getRight();
        }
        else
        {
            current = current -> getLeft();
        }
    }
    return nullptr;
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
AVLNode<Key, Value>*AVLTree<Key, Value>::predecessor(AVLNode<Key, Value>* current)
{
    // TODO
    
    if (!current || !current -> getLeft())
    {
        return nullptr;
    }
    current = current -> getLeft();
    while (current -> getRight())
    {
        current = current -> getRight();
    }
    return current;
}
#endif
