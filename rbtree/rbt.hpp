#include<iostream>
#include<algorithm>
namespace MyDS {
//fwd decl
template <class TKey, class TVal> class RBTreeNode;
template <class TKey, class TVal> class RBTree;

enum class NodeColor {RED, BLACK};

///////////////////////////////Treenode visitors///////////////////////////////
template<class TKey, class TVal>
class TreeNodeVisitor {
public: 
	virtual void operator () (TKey aKey, TVal aVal, std::string aColor, size_t aLevel) = 0 ;
	virtual void operator () () = 0;
};

template<class TKey, class TVal>
class TreeNodePrintVisitor : public TreeNodeVisitor<TKey,TVal> {
public:
	virtual void operator () (TKey aKey, TVal aVal, std::string aColor, size_t aLevel) {
		std::cout << aKey << "(" << aColor << ") ";
	}
	virtual void operator () () {
		//print Nil node
		std::cout << "# " ;
	}
};

template <class TKey, class TVal>
class TreeNodeStatVisitor : public TreeNodeVisitor<TKey, TVal> {
public:
	TreeNodeStatVisitor() {
		mNodes = 0;
		mHeight = 0;
		mBlackHeight = 0;
	}

	virtual void operator () (TKey aKey, TVal aVal, std::string aColor, size_t aLevel) {
		++mNodes;
		mHeight = std::max(mHeight, aLevel);
	}
	virtual void operator () () {
	}

	void reset() {
		mNodes = 0; mHeight = 0; mBlackHeight = 0;
	}

	size_t mNodes;
	size_t mHeight;
	size_t mBlackHeight;
};


///////////////////////////////////Treenode////////////////////////////////////
template<class TKey, class TVal>
struct RBTreeNodeMetaData {
	RBTreeNode<TKey, TVal>* mLeft;
	RBTreeNode<TKey, TVal>* mRight;
	RBTreeNode<TKey, TVal>* mParent;
	NodeColor mColor; 
};

template<class TKey, class TVal>
class RBTreeNode {

friend class RBTree <TKey, TVal>;

private:
	RBTreeNode () {
		mMetaData.mLeft = nullptr;
		mMetaData.mRight = nullptr;
		mMetaData.mParent = nullptr;
		mMetaData.mColor = NodeColor::BLACK;
	} 

	RBTreeNode (TKey aKey, TVal aVal, RBTreeNodeMetaData<TKey, TVal> aMetaData) : mKey(aKey), mVal(aVal), mMetaData(aMetaData)
	{
	} 
	
	TKey mKey;
	TVal mVal;
	RBTreeNodeMetaData<TKey, TVal> mMetaData;
};

/////////////////////////////////////RBTree/////////////////////////////////////
template <class TKey, class TVal>
class RBTree {
public:
	RBTree();
	TVal* find (TKey aKey);
	TKey min ();
	TKey max ();
	bool insert (TKey aKey, TVal aVal);
	bool deleteKey (TKey aKey);
	void inOrderWalk (TreeNodeVisitor<TKey, TVal>& visitor);
	void preOrderWalk (TreeNodeVisitor<TKey, TVal>& visitor);
	bool verifyRB() ;
	void printStats() ;
	int setVerbosity(int aLevel);
private:
	RBTreeNode<TKey, TVal>* mRoot;
	RBTreeNode<TKey, TVal>* mNil;
	int mVerbose;

	bool insert (RBTreeNode<TKey, TVal>*, TKey aKey, TVal aVal);
	RBTreeNode<TKey, TVal>* findNode (TKey aKey);
	RBTreeNode<TKey, TVal>* max (RBTreeNode<TKey, TVal>* aNode);
	RBTreeNode<TKey, TVal>* min (RBTreeNode<TKey, TVal>* aNode);
	void inOrderWalk (RBTreeNode<TKey, TVal>* aNode, size_t aLevel, TreeNodeVisitor<TKey, TVal>& visitor);
	void preOrderWalk (RBTreeNode<TKey, TVal>* aNode, size_t aLevel, TreeNodeVisitor<TKey, TVal>& visitor);
	bool verifyRB(RBTreeNode<TKey, TVal> * aNode, size_t & aBH) ;

	void leftRotate (RBTreeNode<TKey, TVal>* aNode);
	void rightRotate (RBTreeNode<TKey, TVal>* aNode);
	void rbTreeInsertFixup (RBTreeNode<TKey, TVal>* aNode);
	void rbTreeDeleteFixup (RBTreeNode<TKey, TVal>* aNode);

	//Accessor functions
	RBTreeNode<TKey,TVal>* leftOf(RBTreeNode<TKey,TVal>* aNode) const {
		return aNode->mMetaData.mLeft;
	}
	
	RBTreeNode<TKey,TVal>* rightOf(RBTreeNode<TKey,TVal>* aNode) const {
		return aNode->mMetaData.mRight;
	}

	RBTreeNode<TKey,TVal>* parentOf(RBTreeNode<TKey,TVal>* aNode) const {
		return aNode->mMetaData.mParent;
	}

	NodeColor colorOf(RBTreeNode<TKey,TVal>* aNode) const {
		return aNode->mMetaData.mColor;
	}

	bool isRED(RBTreeNode<TKey,TVal>* aNode) const {
		return (aNode->mMetaData.mColor == NodeColor::RED);
	}

	bool isBLACK(RBTreeNode<TKey,TVal>* aNode) const {
		return (aNode->mMetaData.mColor == NodeColor::BLACK);
	}

	//set functions
	void setParent(RBTreeNode<TKey, TVal>* aDest, RBTreeNode<TKey, TVal>* aSrc) {
		aDest->mMetaData.mParent = aSrc;
	}
	void setLeft(RBTreeNode<TKey, TVal>* aDest, RBTreeNode<TKey, TVal>* aSrc) {
		aDest->mMetaData.mLeft = aSrc;
	}
	void setRight(RBTreeNode<TKey, TVal>* aDest, RBTreeNode<TKey, TVal>* aSrc) {
		aDest->mMetaData.mRight = aSrc;
	}

	void setRED(RBTreeNode<TKey, TVal>* aNode) {
		aNode->mMetaData.mColor = NodeColor::RED;
	}
	void setBLACK(RBTreeNode<TKey, TVal>* aNode) {
		aNode->mMetaData.mColor = NodeColor::BLACK;
	}
	void setColor(RBTreeNode<TKey, TVal>* aNode, NodeColor aColor) {
		aNode->mMetaData.mColor = aColor;
	}
};
}//namespace MyDS
#include "rbt.cpp"
