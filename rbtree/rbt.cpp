#include<iostream>
#include <cstddef>
namespace MyDS {
/////////////////////////////////////RBTree/////////////////////////////////////
template <class TKey, class TVal> 
RBTree<TKey, TVal>::RBTree()
{

	mNil = new RBTreeNode<TKey, TVal>();
	setBLACK(mNil);

	mRoot = mNil;
	mVerbose = 0;
}

template <class TKey, class TVal>
int    RBTree<TKey, TVal>  :: setVerbosity(int aLevel) 
{
	mVerbose = aLevel;
	return mVerbose;
}

////////////////////////////////////rbtree lookup///////////////////////////////////
template <class TKey, class TVal> 
TVal*    RBTree<TKey, TVal> :: find(TKey aKey)
{
	RBTreeNode<TKey, TVal>* node = mRoot;

	while (node != mNil) {
		if (aKey == node->mKey) {
			return &(node->mVal);
		}	
		else if (aKey <= node->mKey) {
			node = leftOf(node);
		}	
		else {
			node = rightOf(node);
		}
	}
	//not found
	return nullptr;
}

template <class TKey, class TVal> 
RBTreeNode<TKey, TVal>*    RBTree<TKey, TVal> :: findNode(TKey aKey)
{
	RBTreeNode<TKey, TVal>* node = mRoot;

	while (node != mNil) {
		if (aKey == node->mKey) {
			return node;
		}	
		else if (aKey <= node->mKey) {
			node = leftOf(node);
		}	
		else {
			node = rightOf(node);
		}
	}
	//not found
	return mNil;
}

template <class TKey, class TVal>
RBTreeNode<TKey, TVal>*    RBTree<TKey, TVal> :: max(RBTreeNode<TKey,TVal>* aNode)
{
	RBTreeNode<TKey, TVal>* node = aNode;
	RBTreeNode<TKey, TVal>* parent = mNil;

	if (!aNode) node = mRoot;
	else node = aNode;

	while (node != mNil) {
		parent = node;
		node = rightOf(node);
	}
	return parent;
}

template <class TKey, class TVal>
TKey    RBTree<TKey, TVal> :: max()
{
	RBTreeNode<TKey, TVal>* node = max(mRoot);
	return node->mKey;
}

template <class TKey, class TVal>
RBTreeNode<TKey, TVal>*    RBTree<TKey, TVal> :: min(RBTreeNode<TKey, TVal>* aNode)
{
	RBTreeNode<TKey, TVal>* node = aNode;
	RBTreeNode<TKey, TVal>* parent = mNil;

	if (!aNode) node = mRoot;
	while (node != mNil) {
		parent = node;
		node = leftOf(node);
	}
	return parent;
}

template <class TKey, class TVal>
TKey    RBTree<TKey, TVal> :: min()
{
	RBTreeNode<TKey, TVal>* node = min(mRoot);
	return node->mKey;
}

////////////////////////////////////rbtree modify///////////////////////////////////
template <class TKey, class TVal> 
bool    RBTree<TKey, TVal> :: insert(TKey aKey, TVal aVal) 
{
	RBTreeNode<TKey, TVal> *node, *parent;

	if (mRoot == mNil) {
		mRoot = new RBTreeNode<TKey, TVal>(aKey, aVal, NodeColor::BLACK, mNil, mNil, mNil);//root is black
		return true;
	}
	//return (insert (mRoot, aKey, aVal));
	node = mRoot;
	parent = mNil;
	while (node != mNil) {
		if (aKey < node->mKey) {
			parent = node;
			node = leftOf(node);
		}
		else if (aKey > node->mKey) {
			parent = node;
			node = rightOf(node);
		}
		else /*(aKey == node->mKey)*/ {
			//duplicate keys not supported, replace node
			node->mVal = aVal;
			return false;
		}
	}
	if (node == mNil) {
		if (aKey < parent->mKey) {
			setLeft(parent, new RBTreeNode<TKey, TVal>(aKey, aVal, NodeColor::RED, mNil, mNil, parent));
			if (mVerbose >= 2) {
				std::cout << "Walk before fixup" << std::endl;
				TreeNodePrintVisitor<TKey, TVal> printvisitor;
				preOrderWalk(printvisitor);
				std::cout << std::endl;
			}
			rbTreeInsertFixup(leftOf(parent));
			if (mVerbose >= 2) {
				std::cout << "Walk after fixup" << std::endl;
				TreeNodePrintVisitor<TKey, TVal> printvisitor;
				preOrderWalk(printvisitor);
				std::cout << std::endl;
			}
		}
		else {
			setRight(parent, new RBTreeNode<TKey, TVal>(aKey, aVal, NodeColor::RED, mNil, mNil, parent));
			if (mVerbose >= 2) {
				std::cout << "Walk before fixup" << std::endl;
				TreeNodePrintVisitor<TKey, TVal> printvisitor;
				preOrderWalk(printvisitor);
				std::cout << std::endl;
			}
			rbTreeInsertFixup(rightOf(parent));
			if (mVerbose >= 2) {
				std::cout << "Walk after fixup" << std::endl;
				TreeNodePrintVisitor<TKey, TVal> printvisitor;
				preOrderWalk(printvisitor);
				std::cout << std::endl;
			}
		}
	}
	return true;	
}


template <class TKey, class TVal> 
bool    RBTree<TKey, TVal> :: deleteKey(TKey aKey)
{
	RBTreeNode<TKey, TVal>* z = mRoot;
	RBTreeNode<TKey, TVal>* parent = mNil, **replacePtr = &mNil;
	RBTreeNode<TKey, TVal>* y, *x;

	z = findNode(aKey);
	if (z == mNil) return false;

	//if z is leaf, then just delete
	//if z has single child, then just splice parent to child
	//if z has both left and right child then 
	//find the max of the left subtree or min of the right subtree to replace the deleted node

	//find the replacement node y
	if (leftOf(z) == mNil || rightOf(z) == mNil) {
		y = z;
	}
	else {
		//find the node predecessor of z, this node has Nil right child
		y = max(leftOf(z));
		// or y = min(rightOf(z));
	}

	//copy contents of y to z and delete y

	//x is y's only child
	if (leftOf(y) != mNil) 
		x = leftOf(y);
	else
		x = rightOf(y);

	setParent(x, parentOf(y));

	if (y == mRoot) {
		mRoot = x;
	}
	else {
		if (y == leftOf(parentOf(y)))
			setLeft(parentOf(y), x);
		else
			setRight(parentOf(y), x);
	}

	if (y != z) {
		z->mKey = y->mKey;
		z->mVal = y->mVal;
	}

	if (isBLACK(y)) {
		//y will be removed and if it is RED it implies 
		//no change in BH, no adjacent red node, and it cannot be root
		rbTreeDeleteFixup(x);
	}
	return true;
}

//////////////////RB tree balancing routines/////////////////////
template <class TKey, class TVal> 
void    RBTree<TKey, TVal> :: leftRotate(RBTreeNode<TKey, TVal>* aNode)
{
	//x is parent, y is right child of x, rotate x

	RBTreeNode<TKey, TVal> *x, *y, *p_x;
	x = aNode;
	
	if (x == mNil || !x) return;

	if (mVerbose >= 1) {
		std::cout << "Left Rotate: " << x->mKey << std::endl;
	}

	if (rightOf(x) == mNil) {
		std::cout << "No right child, no left rotation possible" << std::endl;
		return;
	}

	y = rightOf(x);
	//x is the rotation point, y is x's right child
	//change the parent of x to refer to y 

	p_x = parentOf(x);
	
	if (p_x != mNil) {
		if (leftOf(p_x) == x)
			setLeft(p_x, y);
		else
			setRight(p_x, y);
	}

	//fix x's pointers
	setParent(x, y);
	setRight(x, leftOf(y));
	if (leftOf(y) != mNil) {
		setParent(leftOf(y), x);
	}
	//fix y's pointers
	setParent(y, p_x);
	setLeft(y, x);

	if (x == mRoot) {
		mRoot = y;
	}
	
	return;	
}

template <class TKey, class TVal>
void    RBTree<TKey, TVal> :: rightRotate(RBTreeNode<TKey, TVal> * aNode)
{
	RBTreeNode<TKey, TVal> *x, *y, *p_y;
	//y is parent, x is left child of y, rotate y
	y = aNode;
	if (y==mNil || !y) {
		return;
	}

	if (mVerbose >= 1) {
		std::cout << "Right Rotate: " << y->mKey << std::endl;
	}

	if (leftOf(y) == mNil) {
		std::cout << "No left child, no right rotation possible" << std::endl;
	}

	x = leftOf(y);
	p_y = parentOf(y);

	if (p_y != mNil) {
		if (leftOf(p_y) == y) {
			setLeft(p_y, x);
		}
		else {
			setRight(p_y, x);
		}
	}
	setParent(y, x);
	setLeft(y, rightOf(x));
	if (rightOf(x) != mNil) {
		setParent(rightOf(x), y);
	}
	setParent(x, p_y);
	setRight(x, y);

	if (y == mRoot) {
		mRoot = x;
	}
	return;
}

template <class TKey, class TVal>
void    RBTree<TKey, TVal> :: rbTreeInsertFixup (RBTreeNode<TKey, TVal>* aNode)
{
	RBTreeNode<TKey, TVal>* z = aNode;
	RBTreeNode<TKey, TVal>* y;
	if (z == mNil) return;

	while (isRED(parentOf(z))) {
		//y is z's uncle 
		if (parentOf(z) == leftOf(parentOf(parentOf(z)))) {
			//y is RED => change z's parent and z's uncle to black and z's grand parent to red
			y = rightOf(parentOf(parentOf(z))); 
			if (isRED(y)) {
				setBLACK(y);
				setBLACK(parentOf(z));
				setRED(parentOf(parentOf(z)));
				z = parentOf(parentOf(z));
			}
			else {
				//y is black, then 2 cases
				//1. z is right child of its parent
				//2. z is a left child of its parent
				if (z == rightOf(parentOf(z))) {
					//convert situation in 1. to 2.
					z = parentOf(z);
					leftRotate(z);
				}
				setBLACK(parentOf(z));
				setRED(parentOf(parentOf(z)));

				rightRotate(parentOf(parentOf(z)));
			}
		}
		else {
			//y is RED => change z's parent and z's uncle to black and z's grand parent to red
			y = leftOf(parentOf(parentOf(z))); 
			if (isRED(y)) {
				setBLACK(y);
				setBLACK(parentOf(z));
				setRED(parentOf(parentOf(z)));

				z = parentOf(parentOf(z));
			}
			else {
				//y is black, then 2 cases
				//1. z is left child of its parent
				//2. z is right child of its parent
				if (z == leftOf(parentOf(z))) {
					z = parentOf(z);
					rightRotate(z);
				}
				setBLACK(parentOf(z));
				setRED(parentOf(parentOf(z)));
				leftRotate(parentOf(parentOf(z)));	
			}

		}
	}
	setBLACK(mRoot);
	return;
}

template <class TKey, class TVal>
void    RBTree<TKey, TVal> :: rbTreeDeleteFixup (RBTreeNode<TKey, TVal> * aNode)
{
	RBTreeNode<TKey, TVal> *x, *y, *w;

	x = aNode;
	while (x != mRoot && isBLACK(x)) {
		if (leftOf(parentOf(x)) == x) {
			//if x is its parent's left child
			w = rightOf(parentOf(x)); //w is x's sibling
			//4 cases
			//case-1
			if (isRED(w)) {
				setBLACK(w);
				setRED(parentOf(x));
				leftRotate(parentOf(x));
				w = rightOf(parentOf(x));
			}
			//fall-through
			//sibling of w is now Black
			if (isBLACK(leftOf(w)) && isBLACK(rightOf(w))) {
				//case-2: both of w's children are black
				setRED(w);
				x = parentOf(x);
			}
			else { 
				if (isBLACK(rightOf(w))) {
					//case-3: w's left is RED
					setBLACK(leftOf(w));
					setRED(w);
					rightRotate(w);
					w = rightOf(parentOf(x)); 
				}

				//fall-through
				//case-4: w's right is RED
				setColor(w, colorOf(parentOf(x)));
				setBLACK(parentOf(x));
				setBLACK(rightOf(w));
				leftRotate(parentOf(x));
				x = mRoot;
			}	
		}
		else {
			//if x is its parent's right child (symmetric case)
			w = leftOf(parentOf(x)); //w is x's sibling;
			//4 cases
			//case-1
			if (isRED(w)) {
				setBLACK(w);
				setRED(parentOf(x));
				rightRotate(parentOf(x));
				w = leftOf(parentOf(x));
			}
			//fall-through
			//sibling of w is now Black
			if (isBLACK(leftOf(w)) && isBLACK(rightOf(w))) {
				//case-2: both of w's children are black
				setRED(w);
				x = parentOf(x);
			}
			else { 
				if (isBLACK(leftOf(w))) {
					//case-3: w's right is RED, and left is black
					setBLACK(rightOf(w));
					setRED(w);
					leftRotate(w);
					w = leftOf(parentOf(x));
				}
				//fall-through
				//case-4: w's left is RED
				setColor(w, colorOf(parentOf(x)));
				setBLACK(parentOf(x));
				setBLACK(leftOf(w));
				rightRotate(parentOf(x));
				x = mRoot;
			}
		}
	}
	setBLACK(x);
	return;
}

//////////////////Traversal//////////////////
template <class TKey, class TVal> 
void    RBTree<TKey, TVal> :: inOrderWalk(RBTreeNode<TKey, TVal>* aNode, size_t aLevel, TreeNodeVisitor<TKey, TVal>& visitor)
{
	RBTreeNode<TKey, TVal>* node = aNode;

	if (node == mNil) {
		visitor();
		return;
	}

	inOrderWalk(leftOf(node), aLevel+1, visitor);

	visitor(node->mKey, node->mVal, ((isRED(node)) ? "R" : "B"), aLevel);

	inOrderWalk(rightOf(node), aLevel+1, visitor);
	
	return;
}

template <class TKey, class TVal> 
void    RBTree<TKey, TVal> :: inOrderWalk(TreeNodeVisitor<TKey, TVal>& visitor)
{
	RBTreeNode<TKey, TVal>* node = mRoot;

	inOrderWalk(node, 0, visitor);
}

template <class TKey, class TVal>
void    RBTree<TKey, TVal> :: preOrderWalk(RBTreeNode<TKey,TVal> *aNode, size_t aLevel, TreeNodeVisitor<TKey, TVal>& visitor)
{
	if (aNode == mNil) {
		visitor();
		return;
	}

	visitor(aNode->mKey, aNode->mVal, (isRED(aNode) ? "R" : "B"), aLevel);
	preOrderWalk(leftOf(aNode), aLevel+1, visitor);
	preOrderWalk(rightOf(aNode), aLevel+1, visitor);
	return;
}

template <class TKey, class TVal> 
void    RBTree<TKey, TVal> :: preOrderWalk(TreeNodeVisitor<TKey, TVal>& visitor)
{
	RBTreeNode<TKey, TVal>* node = mRoot;

	preOrderWalk(node, 0, visitor);
}
/////////////////////verify Red-Black property/////////////
template <class TKey, class TVal>
bool    RBTree<TKey, TVal> :: verifyRB(RBTreeNode<TKey, TVal> * aNode, size_t & aBH) 
{
	if (aNode == mNil) {
		return true;
	}

	if ( isRED(aNode) && (!isBLACK(leftOf(aNode)) || !isBLACK(rightOf(aNode)))) {
		std::cout << "Node: " << aNode->mKey << " is RED but children are not BLACK" << std::endl;
		return false;
	}		

	size_t leftBH = 0, rightBH = 0;
	if (!verifyRB(leftOf(aNode), leftBH)) {
		return false;
	}
	if (!verifyRB(rightOf(aNode), rightBH)) {
		return false;
	}

	if (leftBH != rightBH) {
		std::cout << "Black Height violation at Node " << aNode->mKey << " left black height = " 
				<< leftBH << " right black height = " << rightBH << std::endl;
		return false;
	}
	aBH = leftBH + (isBLACK(aNode) ? 1 : 0);
	return true;
}

template <class TKey, class TVal>
bool    RBTree<TKey, TVal> :: verifyRB() 
{
	if (!isBLACK(mRoot)) {
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
void    RBTree<TKey, TVal> :: printStats() 
{
	TreeNodeStatVisitor<TKey, TVal> statvisitor;
	statvisitor.reset();
	inOrderWalk(statvisitor);
	std::cout << "Nodes = " << statvisitor.mNodes << " Height = " << statvisitor.mHeight << std::endl;
	std::cout << "Theoritical atmost rbtree height 2log(n+1): " << int(2 * log2(statvisitor.mNodes+1)) << std::endl;
}
}//namespace MyDS
