#include<iostream>
#include<algorithm>
namespace myDS {
//fwd decl
template <class TKey, class TVal> class rbTreeNode;
template <class TKey, class TVal> class rbTree;

enum class nodeColor {RED, BLACK};

///////////////////////////////Treenode visitors///////////////////////////////
template<class TKey, class TVal>
class treeNodeVisitor {
public: 
	virtual void operator () (TKey aKey, TVal aVal, std::string aColor, size_t aLevel) = 0 ;
	virtual void operator () () = 0;
};

template<class TKey, class TVal>
class treeNodePrintVisitor : public treeNodeVisitor<TKey,TVal> {
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
class treeNodeStatVisitor : public treeNodeVisitor<TKey, TVal> {
public:
	treeNodeStatVisitor() {
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
class rbTreeNode {

friend class rbTree <TKey, TVal>;

private:
	rbTreeNode () {
		mLeft = nullptr;
		mRight = nullptr;
		mParent = nullptr;
		mColor = nodeColor::BLACK;
	} 
	rbTreeNode (TKey aKey, TVal aVal, nodeColor aCol) : mKey(aKey), mVal(aVal), mColor(aCol) {
		mLeft = nullptr;
		mRight = nullptr;
		mParent = nullptr;
	} 
	rbTreeNode (TKey aKey, TVal aVal, nodeColor aCol, rbTreeNode* aLeft, rbTreeNode* aRight, rbTreeNode* aParent) 
						: mKey(aKey), mVal(aVal), mColor(aCol), mLeft(aLeft), mRight(aRight), mParent(aParent) 
	{
	} 
	rbTreeNode* mLeft;
	rbTreeNode* mRight;
	rbTreeNode* mParent;
	TKey mKey;
	TVal mVal;
	nodeColor mColor; 
};

/////////////////////////////////////rbTree/////////////////////////////////////
template <class TKey, class TVal>
class rbTree {
public:
	rbTree();
	TVal* find (TKey aKey);
	TKey min ();
	TKey max ();
	bool insert (TKey aKey, TVal aVal);
	bool deleteKey (TKey aKey);
	void inOrderWalk (treeNodeVisitor<TKey, TVal>& visitor);
	void preOrderWalk (treeNodeVisitor<TKey, TVal>& visitor);
	bool verifyRB() ;
	void printStats() ;
	int setVerbosity(int aLevel);
private:
	rbTreeNode<TKey, TVal>* mRoot;
	rbTreeNode<TKey, TVal>* mNil;
	int mVerbose;

	bool insert (rbTreeNode<TKey, TVal>*, TKey aKey, TVal aVal);
	rbTreeNode<TKey, TVal>* findNode (TKey aKey);
	rbTreeNode<TKey, TVal>* max (rbTreeNode<TKey, TVal>* aNode);
	rbTreeNode<TKey, TVal>* min (rbTreeNode<TKey, TVal>* aNode);
	void inOrderWalk (rbTreeNode<TKey, TVal>* aNode, size_t aLevel, treeNodeVisitor<TKey, TVal>& visitor);
	void preOrderWalk (rbTreeNode<TKey, TVal>* aNode, size_t aLevel, treeNodeVisitor<TKey, TVal>& visitor);
	bool verifyRB(rbTreeNode<TKey, TVal> * aNode, size_t & aBH) ;

	void leftRotate (rbTreeNode<TKey, TVal>* aNode);
	void rightRotate (rbTreeNode<TKey, TVal>* aNode);
	void rbTreeInsertFixup (rbTreeNode<TKey, TVal>* aNode);
	void rbTreeDeleteFixup (rbTreeNode<TKey, TVal>* aNode);

};
}//namespace myDS
#include "rbt.cpp"
