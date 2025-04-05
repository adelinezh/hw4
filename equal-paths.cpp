#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
size_t getLeftLeafDepth(Node* root, size_t depth)
{
  if (!root)
  {
    return depth;
  }
  if (!root -> left && !root -> right)
  {
    return depth;
  }
  if (root -> left)
  {
    return getLeftLeafDepth(root -> left, depth + 1);
  }
  return getLeftLeafDepth(root -> right, depth + 1);
}

size_t getRightLeafDepth(Node* root, size_t depth)
{
  if (!root)
  {
    return depth;
  }
  if (!root -> left && !root -> right)
  {
    return depth;
  }
  if (root -> right)
  {
    return getRightLeafDepth(root -> right, depth + 1);
  }
  return getRightLeafDepth(root -> left, depth + 1);
} 

bool equalPaths(Node * root)
{
    bool equal = true;
    if (!root)
    {
      return equal;
    }
    if (getRightLeafDepth(root -> right, 1) == getLeftLeafDepth(root -> left, 1))
    {
      return equal;
    }
    return false;

}

