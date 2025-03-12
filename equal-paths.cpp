#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool pathExplorer(Node * root, int level, int& highestLevel);

bool equalPaths(Node * root)
{
    int highestLevel = -1;
    // Add your code below
    return pathExplorer(root, 0, highestLevel);
}

bool pathExplorer(Node* root, int level, int& highestLevel) {
    if (root == nullptr) return true; // this means we were given an empty tree
    // if leaf
    if (root->left == nullptr && root->right == nullptr) {
        // if first leaf found set the path length
        if (highestLevel == -1) {
            highestLevel = level;
            // cout << highestLevel << " first" << endl;
            return true;
        } else {
            // cout << level << " leaf" << endl;
            return level == highestLevel;
        }
    } else if (root->right == nullptr) {
        return pathExplorer(root->left, level + 1, highestLevel);
    } else if (root->left == nullptr) {
        return pathExplorer(root->right, level + 1, highestLevel);
    } else {
        // otherwise must have both children
        return pathExplorer(root->left, level + 1, highestLevel) && pathExplorer(root->right, level + 1, highestLevel);
    }

    
}