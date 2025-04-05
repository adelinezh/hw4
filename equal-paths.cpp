#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
/*size_t getLeftLeafDepth(Node* root, size_t depth)
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
*/
int leafDepth(Node* root)
{
  if(!root)
  {
    return 0;
  }
  if (!root -> left && !root -> right)
  {
    return 0;
  }

  int leftDepth = leafDepth(root -> left);
  int rightDepth = leafDepth(root -> right);

  //one child missing
  if (leftDepth == -1)
  {
    return rightDepth + 1;
  }
  if (rightDepth == -1)
  {
    return leftDepth + 1;
  }
  if (leftDepth != rightDepth)
  {
    return -2;
  }
  return leftDepth + 1;
}


bool equalPaths(Node * root)
{
    bool equal = true;
    if (!root)
    {
      return equal;
    }
    if (leafDepth(root) >= 0)
    {
      return equal;
    }
    return false;
}

