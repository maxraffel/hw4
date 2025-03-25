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
    void leftRotate(AVLNode<Key,Value>* node);
    void rightRotate(AVLNode<Key,Value>* node);
    void pRotate(AVLNode<Key,Value>* node, int side); // side: 1 for right, -1 for left

    void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node);
    void removeFix(AVLNode<Key,Value>* node, int diff);
};

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key,Value>* node) {
    if (node == nullptr) return;
    AVLNode<Key, Value>* child = node->getLeft();
    AVLNode<Key, Value>* parent = node->getParent();

    node->setLeft(child->getRight());
    child->setRight(node);
    child->setParent(parent);

    if (node->getParent() == nullptr) {
        this->root_ = child;
    } else {
        if (parent->getLeft() == node) {
            parent->setLeft(child);
        } else {
            parent->setRight(child);
        }
    }
    node->setParent(child);
}

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key,Value>* node) {
    if (node == nullptr) return;
    AVLNode<Key, Value>* child = node->getRight();
    AVLNode<Key, Value>* parent = node->getParent();
    node->setRight(child->getLeft());
    child->setLeft(node);
    child->setParent(node->getParent());
    if (node->getParent() == nullptr) {
        this->root_ = child;
    } else {
        if (parent->getLeft() == node) {
            parent->setLeft(child);
        } else {
            parent->setRight(child);
        }
    }
    node->setParent(child);
}

template<class Key, class Value>
void AVLTree<Key, Value>::pRotate(AVLNode<Key,Value>* node, int side) {
    if (side == -1) {
        leftRotate(node);
    } else {
        rightRotate(node);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node) {
    if (parent == nullptr || node == nullptr) return;
    AVLNode<Key,Value>* grandParent = parent->getParent();
    if (grandParent == nullptr) return;

    int side = 1;
    if (grandParent->getLeft() == parent) side = -1;

    grandParent->updateBalance(side);

    if (grandParent->getBalance() == 0) {
        return;
    } else if (grandParent->getBalance() == side) {
        insertFix(grandParent, parent);
    } else {
        int childSide = 1;
        if (parent->getLeft() == node) childSide = -1;
        if (childSide == side) { // zig zig
            pRotate(grandParent, -side);
        } else { // zig zag
            pRotate(parent, side);
            pRotate(grandParent, -side);

            if (node->getBalance() == side) {
                parent->setBalance(0);
                grandParent->setBalance(-side);
            } else if (node->getBalance() == 0) {
                parent->setBalance(0);
                grandParent->setBalance(0);
            } else {
                parent->setBalance(side);
                grandParent->setBalance(0);
            }

            node->setBalance(0);
        }
    }

}



/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{

    Key key = new_item.first;
    Value value = new_item.second;
    // TODO
    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key,Value>(key, value, nullptr);
        return;
    }

    // walk to leaf
    AVLNode<Key,Value>* parent = dynamic_cast<AVLNode<Key,Value>*>(this->root_);
    AVLNode<Key,Value>* leaf = parent;
    while (leaf != nullptr) {
        parent = leaf;
        if (key == parent->getKey()) {
            parent->setValue(value);
            return;
        } else if (key < parent->getKey()) {
            leaf = parent->getLeft();
        } else {
            leaf = parent->getRight();
        }
    }

    AVLNode<Key,Value>* newNode = new AVLNode<Key,Value>(key, value, parent);

    if (key < parent->getKey()) {
        parent->setLeft(newNode);
        parent->updateBalance(-1);
    } else {
        parent->setRight(newNode);
        parent->updateBalance(1);
    }

    if (parent->getBalance() != 0) {
        insertFix(parent, newNode);
    }

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO

    AVLNode<Key,Value>* deleteNode = dynamic_cast<AVLNode<Key,Value>*>(this->internalFind(key));
    if (deleteNode == nullptr) return;
    if (deleteNode->getLeft() != nullptr && deleteNode->getRight() != nullptr) {
        nodeSwap(deleteNode, dynamic_cast<AVLNode<Key,Value>*>(this->predecessor(deleteNode)));
    }

    AVLNode<Key,Value>* parent = deleteNode->getParent();
    AVLNode<Key,Value>* nextNode = nullptr;
    if (deleteNode->getLeft() != nullptr) {
        nextNode = deleteNode->getLeft();
    } else if (deleteNode->getRight() != nullptr) {
        nextNode = deleteNode->getRight();
    }

    if (parent == nullptr) {
        this->root_ = nextNode;
        nextNode->setParent(nullptr);
        delete deleteNode;
        return;
    }

    int diff;
    if (parent->getLeft() == deleteNode) {
        diff = 1;
        parent->setLeft(nextNode);
    } else {
        diff = -1;
        parent->setRight(nextNode);
    }
    if (nextNode != nullptr) {
        nextNode->setParent(parent);
    }
    
    delete deleteNode;
    removeFix(parent, diff);

    
}

template<class Key, class Value>
void AVLTree<Key,Value>::removeFix(AVLNode<Key,Value>* node, int diff) {
    if (node == nullptr) return;
    int nDiff;
    AVLNode<Key,Value>* nParent = node->getParent();
    if (nParent != nullptr) {
        if (nParent->getLeft() == node) {
            nDiff = 1;
        } else {
            nDiff = -1;
        }
    }

    if (node->getBalance() == diff) {
        AVLNode<Key,Value>* c;
        if (diff == -1) {
            c = node->getLeft();
        } else {
            c = node->getRight();
        }

        if (c->getBalance() == diff) { // zig zig
            pRotate(node, -diff);
            node->setBalance(0);
            c->setBalance(0);
            removeFix(nParent, nDiff);
        } else if (node->getBalance() == diff) {
            pRotate(node, -diff);
            node->setBalance(diff);
            c->setBalance(-diff);
        } else { // zig zag (-diff)
            AVLNode<Key,Value>* g;
            if (diff == 1) {
                g = node->getLeft();
            } else {
                g = node->getRight();
            }

            pRotate(c, diff);
            pRotate(node, -diff);

            if (g->getBalance() == -diff) {
                node->setBalance(0);
                c->setBalance(diff);
                g->setBalance(0);
            } else if (g->getBalance() == 0) {
                node->setBalance(0);
                c->setBalance(0);
                g->setBalance(0);
            } else {
                node->setBalance(-diff);
                c->setBalance(0);
                g->setBalance(0);
            }

            removeFix(nParent, nDiff);
        }
    } else if (node->getBalance() == 0) {
        node->setBalance(diff);
    } else {
        node->setBalance(0);
        removeFix(nParent, nDiff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
