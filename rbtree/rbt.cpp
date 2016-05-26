#include<iostream>
#include <cstddef>
namespace myDS {
/////////////////////////////////////rbTree/////////////////////////////////////
template <class TKey, class TVal> 
rbTree<TKey, TVal>::rbTree()
{

	mNil = new rbTreeNode<TKey, TVal>();
	mNil->mColor = nodeColor::BLACK;

	mRoot = mNil;
	mVerbose = 0;
}

template <class TKey, class TVal>
int    rbTree<TKey, TVal>  :: setVerbosity(int aLevel) 
{
	mVerbose = aLevel;
	return mVerbose;
}

////////////////////////////////////rbtree lookup///////////////////////////////////
template <class TKey, class TVal> 
TVal*    rbTree<TKey, TVal> :: find(TKey aKey)
{
	rbTreeNode<TKey, TVal>* node = mRoot;

	while (node != mNil) {
		if (aKey == node->mKey) {
			return &(node->mVal);
		}	
		else if (aKey <= node->mKey) {
			node = node->mLeft;
		}	
		else {
			node = node->mRight;
		}
	}
	//not found
	return nullptr;
}

template <class TKey, class TVal> 
rbTreeNode<TKey, TVal>*    rbTree<TKey, TVal> :: findNode(TKey aKey)
{
	rbTreeNode<TKey, TVal>* node = mRoot;

	while (node != mNil) {
		if (aKey == node->mKey) {
			return node;
		}	
		else if (aKey <= node->mKey) {
			node = node->mLeft;
		}	
		else {
			node = node->mRight;
		}
	}
	//not found
	return mNil;
}

template <class TKey, class TVal>
rbTreeNode<TKey, TVal>*    rbTree<TKey, TVal> :: max(rbTreeNode<TKey,TVal>* aNode)
{
	rbTreeNode<TKey, TVal>* node = aNode;
	rbTreeNode<TKey, TVal>* parent = mNil;

	if (!aNode) node = mRoot;
	else node = aNode;

	while (node != mNil) {
		parent = node;
		node = node->mRight;	
	}
	return parent;
}

template <class TKey, class TVal>
TKey    rbTree<TKey, TVal> :: max()
{
	rbTreeNode<TKey, TVal>* node = max(mRoot);
	return node->mKey;
}

template <class TKey, class TVal>
rbTreeNode<TKey, TVal>*    rbTree<TKey, TVal> :: min(rbTreeNode<TKey, TVal>* aNode)
{
	rbTreeNode<TKey, TVal>* node = aNode;
	rbTreeNode<TKey, TVal>* parent = mNil;

	if (!aNode) node = mRoot;
	while (node != mNil) {
		parent = node;
		node = node->mLeft;
	}
	return parent;
}

template <class TKey, class TVal>
TKey    rbTree<TKey, TVal> :: min()
{
	rbTreeNode<TKey, TVal>* node = min(mRoot);
	return node->mKey;
}

////////////////////////////////////rbtree modify///////////////////////////////////
template <class TKey, class TVal> 
bool    rbTree<TKey, TVal> :: insert(TKey aKey, TVal aVal) 
{
	rbTreeNode<TKey, TVal> *node, *parent;

	if (mRoot == mNil) {
		mRoot = new rbTreeNode<TKey, TVal>(aKey, aVal, nodeColor::BLACK, mNil, mNil, mNil);//root is black
		return true;
	}
	//return (insert (mRoot, aKey, aVal));
	node = mRoot;
	parent = mNil;
	while (node != mNil) {
		if (aKey < node->mKey) {
			parent = node;
			node = node->mLeft;
		}
		else if (aKey > node->mKey) {
			parent = node;
			node = node->mRight;
		}
		else /*(aKey == node->mKey)*/ {
			//duplicate keys not supported, replace node
			node->mVal = aVal;
			return false;
		}
	}
	if (node == mNil) {
		if (aKey < parent->mKey) {
			parent->mLeft = new rbTreeNode<TKey, TVal>(aKey, aVal, nodeColor::RED, mNil, mNil, parent);
			if (mVerbose >= 2) {
				std::cout << "Walk before fixup" << std::endl;
				treeNodePrintVisitor<TKey, TVal> printvisitor;
				preOrderWalk(printvisitor);
				std::cout << std::endl;
			}
			rbTreeInsertFixup(parent->mLeft);
			if (mVerbose >= 2) {
				std::cout << "Walk after fixup" << std::endl;
				treeNodePrintVisitor<TKey, TVal> printvisitor;
				preOrderWalk(printvisitor);
				std::cout << std::endl;
			}
		}
		else {
			parent->mRight = new rbTreeNode<TKey, TVal>(aKey, aVal, nodeColor::RED, mNil, mNil, parent);
			if (mVerbose >= 2) {
				std::cout << "Walk before fixup" << std::endl;
				treeNodePrintVisitor<TKey, TVal> printvisitor;
				preOrderWalk(printvisitor);
				std::cout << std::endl;
			}
			rbTreeInsertFixup(parent->mRight);
			if (mVerbose >= 2) {
				std::cout << "Walk after fixup" << std::endl;
				treeNodePrintVisitor<TKey, TVal> printvisitor;
				preOrderWalk(printvisitor);
				std::cout << std::endl;
			}
		}
	}
	return true;	
}


template <class TKey, class TVal> 
bool    rbTree<TKey, TVal> :: deleteKey(TKey aKey)
{
	rbTreeNode<TKey, TVal>* z = mRoot;
	rbTreeNode<TKey, TVal>* parent = mNil, **replacePtr = &mNil;
	rbTreeNode<TKey, TVal>* y, *x;

	z = findNode(aKey);
	if (z == mNil) return false;

	//if z is leaf, then just delete
	//if z has single child, then just splice parent to child
	//if z has both left and right child then 
	//find the max of the left subtree or min of the right subtree to replace the deleted node

	//find the replacement node y
	if (z->mLeft == mNil || z->mRight == mNil) {
		y = z;
	}
	else {
		//find the node predecessor of z, this node has Nil right child
		y = max(z->mLeft);
		// or y = min(z->mRight);
	}

	//copy contents of y to z and delete y

	//x is y's only child
	if (y->mLeft != mNil) 
		x = y->mLeft;
	else
		x = y->mRight;

	x->mParent = y->mParent;

	if (y == mRoot) {
		mRoot = x;
	}
	else {
		if (y == y->mParent->mLeft) 
			y->mParent->mLeft = x;
		else
			y->mParent->mRight = x;
	}

	if (y != z) {
		z->mKey = y->mKey;
		z->mVal = y->mVal;
	}

	if (y->mColor == nodeColor::BLACK) {
		//y will be removed and if it is RED it implies 
		//no change in BH, no adjacent red node, and it cannot be root
		rbTreeDeleteFixup(x);
	}
	return true;
}

//////////////////RB tree balancing routines/////////////////////
template <class TKey, class TVal> 
void    rbTree<TKey, TVal> :: leftRotate(rbTreeNode<TKey, TVal>* aNode)
{
	//x is parent, y is right child of x, rotate x

	rbTreeNode<TKey, TVal> *x, *y, *p_x;
	x = aNode;
	
	if (x == mNil || !x) return;

	if (mVerbose >= 1) {
		std::cout << "Left Rotate: " << x->mKey << std::endl;
	}

	if (x->mRight == mNil) {
		std::cout << "No right child, no left rotation possible" << std::endl;
		return;
	}

	y = x->mRight;
	//x is the rotation point, y is x's right child
	//change the parent of x to refer to y 

	p_x = x->mParent;
	
	if (p_x != mNil) {
		if (p_x->mLeft == x)
			p_x->mLeft = y;
		else
			p_x->mRight = y;
	}

	//fix x's pointers
	x->mParent = y;
	x->mRight =  y->mLeft;
	if (y->mLeft != mNil) {
		y->mLeft->mParent = x;
	}
	//fix y's pointers
	y->mParent = p_x;
	y->mLeft = x;

	if (x == mRoot) {
		mRoot = y;
	}
	
	return;	
}

template <class TKey, class TVal>
void    rbTree<TKey, TVal> :: rightRotate(rbTreeNode<TKey, TVal> * aNode)
{
	rbTreeNode<TKey, TVal> *x, *y, *p_y;
	//y is parent, x is left child of y, rotate y
	y = aNode;
	if (y==mNil || !y) {
		return;
	}

	if (mVerbose >= 1) {
		std::cout << "Right Rotate: " << y->mKey << std::endl;
	}

	if (y->mLeft == mNil) {
		std::cout << "No left child, no right rotation possible" << std::endl;
	}

	x = y->mLeft;
	p_y = y->mParent;

	if (p_y != mNil) {
		if (p_y->mLeft == y) {
			p_y->mLeft = x;
		}
		else {
			p_y->mRight = x;
		}
	}
	y->mParent = x;
	y->mLeft = x->mRight;
	if (x->mRight != mNil) {
		x->mRight->mParent = y;
	}
	x->mParent = p_y;
	x->mRight = y;

	if (y == mRoot) {
		mRoot = x;
	}
	return;
}

template <class TKey, class TVal>
void    rbTree<TKey, TVal> :: rbTreeInsertFixup (rbTreeNode<TKey, TVal>* aNode)
{
	rbTreeNode<TKey, TVal>* z = aNode;
	rbTreeNode<TKey, TVal>* y;
	if (z == mNil) return;

	while (z->mParent->mColor == nodeColor::RED) {
		//y is z's uncle 
		if (z->mParent == z->mParent->mParent->mLeft) {
			//y is RED => change z's parent and z's uncle to black and z's grand parent to red
			y = z->mParent->mParent->mRight;
			if (y->mColor == nodeColor::RED) {
				y->mColor = nodeColor::BLACK;
				z->mParent->mColor = nodeColor::BLACK;
				z->mParent->mParent->mColor = nodeColor::RED;
				z = z->mParent->mParent;
			}
			else {
				//y is black, then 2 cases
				//1. z is right child of its parent
				//2. z is a left child of its parent
				if (z == z->mParent->mRight) {
					//convert situation in 1. to 2.
					z = z->mParent;
					leftRotate(z);
				}
				z->mParent->mColor = nodeColor::BLACK;
				z->mParent->mParent->mColor = nodeColor::RED;
				rightRotate(z->mParent->mParent);
			}
		}
		else {
			//y is RED => change z's parent and z's uncle to black and z's grand parent to red
			y = z->mParent->mParent->mLeft;
			if (y->mColor == nodeColor::RED) {
				y->mColor = nodeColor::BLACK;
				z->mParent->mColor = nodeColor::BLACK;
				z->mParent->mParent->mColor = nodeColor::RED;
				z = z->mParent->mParent;
			}
			else {
				//y is black, then 2 cases
				//1. z is left child of its parent
				//2. z is right child of its parent
				if (z == z->mParent->mLeft) {
					z = z->mParent;
					rightRotate(z);
				}
				z->mParent->mColor = nodeColor::BLACK;
				z->mParent->mParent->mColor = nodeColor::RED;
				leftRotate(z->mParent->mParent);	
			}

		}
	}
	mRoot->mColor = nodeColor::BLACK;
	return;
}

template <class TKey, class TVal>
void    rbTree<TKey, TVal> :: rbTreeDeleteFixup (rbTreeNode<TKey, TVal> * aNode)
{
	rbTreeNode<TKey, TVal> *x, *y, *w;

	x = aNode;
	while (x != mRoot && x->mColor == nodeColor::BLACK) {
		if (x->mParent->mLeft == x) {
			//if x is its parent's left child
			w = x->mParent->mRight; //w is x's sibling
			//4 cases
			//case-1
			if (w->mColor == nodeColor::RED) {
				w->mColor = nodeColor::BLACK;
				x->mParent->mColor = nodeColor::RED;
				leftRotate(x->mParent);
				w = x->mParent->mRight;
			}
			//fall-through
			//sibling of w is now Black
			if (w->mLeft->mColor == nodeColor::BLACK && w->mRight->mColor == nodeColor::BLACK) {
				//case-2: both of w's children are black
				w->mColor = nodeColor::RED;
				x = x->mParent;
			}
			else { 
				if (w->mRight->mColor == nodeColor::BLACK) {
					//case-3: w's left is RED
					w->mLeft->mColor = nodeColor::BLACK;
					w->mColor = nodeColor::RED;
					rightRotate(w);
					w = x->mParent->mRight;
				}

				//fall-through
				//case-4: w's right is RED
				w->mColor = x->mParent->mColor;
				x->mParent->mColor = nodeColor::BLACK;
				w->mRight->mColor = nodeColor::BLACK;
				leftRotate(x->mParent);
				x = mRoot;
			}	
		}
		else {
			//if x is its parent's right child (symmetric case)
			w = x->mParent->mLeft; //w is x's sibling;
			//4 cases
			//case-1
			if (w->mColor == nodeColor::RED) {
				w->mColor = nodeColor::BLACK;
				x->mParent->mColor = nodeColor::RED;
				rightRotate(x->mParent);
				w = x->mParent->mLeft;
			}
			//fall-through
			//sibling of w is now Black
			if (w->mLeft->mColor == nodeColor::BLACK && w->mRight->mColor == nodeColor::BLACK) {
				//case-2: both of w's children are black
				w->mColor = nodeColor::RED;
				x = x->mParent;
			}
			else { 
				if (w->mLeft->mColor == nodeColor::BLACK) {
					//case-3: w's right is RED, and left is black
					w->mRight->mColor = nodeColor::BLACK;
					w->mColor = nodeColor::RED;
					leftRotate(w);
					w = x->mParent->mLeft;
				}
				//fall-through
				//case-4: w's left is RED
				w->mColor = x->mParent->mColor;
				x->mParent->mColor = nodeColor::BLACK;
				w->mLeft->mColor = nodeColor::BLACK;
				rightRotate(x->mParent);
				x = mRoot;
			}
		}
	}
	x->mColor = nodeColor::BLACK;
	return;
}

//////////////////Traversal//////////////////
template <class TKey, class TVal> 
void    rbTree<TKey, TVal> :: inOrderWalk(rbTreeNode<TKey, TVal>* aNode, size_t aLevel, treeNodeVisitor<TKey, TVal>& visitor)
{
	rbTreeNode<TKey, TVal>* node = aNode;

	if (node == mNil) {
		visitor();
		return;
	}

	inOrderWalk(node->mLeft, aLevel+1, visitor);

	visitor(node->mKey, node->mVal, ((node->mColor == nodeColor::RED) ? "R" : "B"), aLevel);

	inOrderWalk(node->mRight, aLevel+1, visitor);
	
	return;
}

template <class TKey, class TVal> 
void    rbTree<TKey, TVal> :: inOrderWalk(treeNodeVisitor<TKey, TVal>& visitor)
{
	rbTreeNode<TKey, TVal>* node = mRoot;

	inOrderWalk(node, 0, visitor);
}

template <class TKey, class TVal>
void    rbTree<TKey, TVal> :: preOrderWalk(rbTreeNode<TKey,TVal> *aNode, size_t aLevel, treeNodeVisitor<TKey, TVal>& visitor)
{
	if (aNode == mNil) {
		visitor();
		return;
	}

	visitor(aNode->mKey, aNode->mVal, ((aNode->mColor == nodeColor::RED) ? "R" : "B"), aLevel);
	preOrderWalk(aNode->mLeft, aLevel+1, visitor);
	preOrderWalk(aNode->mRight, aLevel+1, visitor);
	return;
}

template <class TKey, class TVal> 
void    rbTree<TKey, TVal> :: preOrderWalk(treeNodeVisitor<TKey, TVal>& visitor)
{
	rbTreeNode<TKey, TVal>* node = mRoot;

	preOrderWalk(node, 0, visitor);
}
/////////////////////verify Red-Black property/////////////
template <class TKey, class TVal>
bool    rbTree<TKey, TVal> :: verifyRB(rbTreeNode<TKey, TVal> * aNode, size_t & aBH) 
{
	if (aNode == mNil) {
		return true;
	}

	if ( (aNode->mColor == nodeColor::RED) &&
			(aNode->mLeft->mColor != nodeColor::BLACK || aNode->mRight->mColor != nodeColor::BLACK) ) {
		std::cout << "Node: " << aNode->mKey << " is RED but children are not BLACK" << std::endl;
		return false;
	}		

	size_t leftBH = 0, rightBH = 0;
	if (!verifyRB(aNode->mLeft, leftBH)) {
		return false;
	}
	if (!verifyRB(aNode->mRight, rightBH)) {
		return false;
	}

	if (leftBH != rightBH) {
		std::cout << "Black Height violation at Node " << aNode->mKey << " left black height = " 
				<< leftBH << " right black height = " << rightBH << std::endl;
		return false;
	}
	aBH = leftBH + ((aNode->mColor == nodeColor::BLACK) ? 1 : 0);
	return true;
}

template <class TKey, class TVal>
bool    rbTree<TKey, TVal> :: verifyRB() 
{
	if (mRoot->mColor != nodeColor::BLACK) {
		std::cout << "Root NOT BLACK" << std::endl;
		return false;
	}
	size_t blackHeight;
	if (!verifyRB(mRoot, blackHeight)) {
		std::cout << "RB tree violation" << std::endl;
		return false;
	}
	std::cout << "** Verified RB Tree! Black Height at root is: " << blackHeight << std::endl;
	return true;
}

#include <math.h>
template <class TKey, class TVal>
void    rbTree<TKey, TVal> :: printStats() 
{
	treeNodeStatVisitor<TKey, TVal> statvisitor;
	statvisitor.reset();
	inOrderWalk(statvisitor);
	std::cout << "Nodes = " << statvisitor.mNodes << " Height = " << statvisitor.mHeight << std::endl;
	std::cout << "Theoritical atmost rbtree height 2log(n+1): " << int(2 * log2(statvisitor.mNodes+1)) << std::endl;
}
}//namespace myDS
