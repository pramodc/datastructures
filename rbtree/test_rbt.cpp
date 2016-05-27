#include <iostream>
#include "rbt.hpp"

int test2() 
{

	int key = 1;
	MyDS::RBTree<int, int> bst;
	MyDS::TreeNodePrintVisitor<int, int> printvisitor;

	bst.insert(key,10);
	
	bst.insert(key+1,20);
	
	bst.insert(key+2,30);
	
	bst.insert(key+3,40);
	bst.insert(key+4,50);
	bst.insert(key+5,60);
	bst.insert(key+6,60);
	bst.insert(key+7,70);
	bst.insert(key+8,80);
	bst.insert(key+9,90);
	bst.insert(key+10,100);

	if(!bst.verifyRB()) 
		return -1;

	std::cout << "preorder walk with printvisitor" << std::endl;
	bst.preOrderWalk(printvisitor);
	std::cout << std::endl;
	
	int *p = bst.find(key);
	if (!p) std::cout << key << " not found" << std::endl;
	else std::cout << "found: " << *p << " @ key: " << key << std::endl; 

	key = bst.max();
	std::cout << "max key: " << key << std::endl;
	key = bst.min();
	std::cout << "min key: " << key << std::endl;
	
	bst.printStats();

	key = 1;
	std::cout << "removing key: " << key << std::endl;
	if(!bst.deleteKey(key)) {
		std::cout << "failed to delete key: " << key << std::endl;
	}

	if(!bst.verifyRB()) 
		return -1;

	bst.printStats();

	std::cout << "preorder walk" << std::endl;
	bst.preOrderWalk(printvisitor);
	std::cout << std::endl;

	std::cout << "Inserting key 0" << std::endl;
	bst.insert(0, 100);

	if(!bst.verifyRB()) 
		return -1;

	key = 6;
	std::cout << "removing key: " << key << std::endl;
	if(!bst.deleteKey(key)) {
		std::cout << "failed to delete key: " << key << std::endl;
	}
	
	if(!bst.verifyRB()) 
		return -1;

	bst.printStats();

	std::cout << "preorder walk" << std::endl;
	bst.preOrderWalk(printvisitor);
	std::cout << std::endl;

	return 0;
}

int test1(size_t aNum) 
{
	int k;
	std::string v;
	srand(time(NULL));
	MyDS::RBTree<int, std::string> rbt;
	MyDS::TreeNodePrintVisitor<int, std::string> printvisitor;

	int i = 0;
	while (i<aNum) {
		k = rand()%aNum;
		v = std::to_string(k);
		if (rbt.find(k) == nullptr) {
			//std::cout << "insert: " << k << std::endl;
			rbt.insert(k,v);
			++i;
		}
	}

	rbt.printStats();

	if (!rbt.verifyRB()) {
		return -1;
	}	

	i = 0;
	int ndelete = 0;
	while (i < aNum/2) {
		k = rand()%aNum;
		if (rbt.deleteKey(k)) {
			++ndelete;
		}
		++i;
	}
	std::cout << "Num nodes deleted = " << ndelete << std::endl;

	rbt.printStats();

	if (!rbt.verifyRB()) {
		return -1;
	}	
	return 0;
}

int main (int argc, char* argv[])
{
	if (test1(4000) != 0) {
		std::cout << "**********Test1 failed*********" << std::endl;
		return -1;
	}
	std::cout << "**********Test1 passed*********" << std::endl;
	if (test2() != 0) {
		std::cout << "**********Test2 failed*********" << std::endl;
		return -1;
	}
	std::cout << "**********Test2 passed*********" << std::endl;

	return 0;
}
