// If you change anything in this file, your changes will be ignored 
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
#include "../code/Merkle.h"
#include "../code/sha256.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class test_Merkle : public ::testing::Test {
protected:
	// This function runs only once before any TEST_F function
	static void SetUpTestCase(){
	}

	// This function runs after all TEST_F functions have been executed
	static void TearDownTestCase(){
	}
    
	// this function runs before every TEST_F function
	void SetUp() override {
    }

	// this function runs after every TEST_F function
	void TearDown() override {
	}
};



/*
##############################################################################################################
######################################   NOTE   ##############################################################
##############################################################################################################



 * This is the structure of the tree which is used to test how and where nodes are inserted into a tree
 * When nodes inserted in the order 1, 2, 3, 4, 5, the tree should be as follows:

                _______________________________
		 		[_____________root__r_________]
     	  		  |				            |
     	  		  |				            |
     	  		  |				            |
			_________                     __________ 		 
 		    [r->left]	  	              [r->right]
  		  	/	      \ 			      |		  |	
  		   / 	       \			      |		  |	
  		  /  	       |			      |		  |	
  		 /		       |			      |		  |
	__________	   	_________       __________   __________
    [r->l->l]]     [newleaf3]       [newleaf2]   [newleaf4]
	   |     \
	   |      \        
	   |       \        
__________      \__________	   
[newleaf1]       [newleaf5]


##############################################################################################################
##############################################################################################################
##############################################################################################################
*/


// Tests the constructor function for the Merkle Tree class
TEST_F(test_Merkle, TestConstructor){
    MerkleTree mytree; // New tree

    ASSERT_FALSE(mytree.GetRoot()); // root_ptr of new Merkle Tree should be NULL
}


// Tests the initialization of leaf nodes (InitLeaf)
TEST_F(test_Merkle, TestInitLeaf){
    MerkleTree mytree; // New tree

	string message1 = "grape";
	string hash1 = "0f78fcc486f5315418fbf095e71c0675ee07d318e5ac4d150050cd8e57966496"; // The SHA-256 hash for the string "grape"
	shared_ptr<Node> newleaf1 = mytree.InitLeaf("grape");

	string message2 = "Philip K. Dick";
	string hash2 = "9a5d804c19632cfe61dbc08250897d757c11f289fa363f8611180be180119ba4"; // The SHA-256 hash for the string "Philip K. Dick"
	shared_ptr<Node> newleaf2 = mytree.InitLeaf("Philip K. Dick");

	string message3 = "All the King's horses & all the King's men";
	string hash3 = "90a663d6116ade221b3bbacde19ef2ed551bc3def24a416f6ce54f36e78c692c"; // The SHA-256 hash for the string "All the King's horses & all the King's men"
	shared_ptr<Node> newleaf3 = mytree.InitLeaf("All the King's horses & all the King's men");

	string message4 = "285 thousand 3 hundred sixty 4";
	string hash4 = "840f557345f1f0705c0fe22c982cba54d0da90c15f957d6c376fcf8aa0916ade"; // The SHA-256 hash for the string "285 thousand 3 hundred sixty 4"
	shared_ptr<Node> newleaf4 = mytree.InitLeaf("285 thousand 3 hundred sixty 4");


	// Make sure all strings were hashed correctly
    ASSERT_EQ("0f78fcc486f5315418fbf095e71c0675ee07d318e5ac4d150050cd8e57966496", newleaf1->hashkey);
	ASSERT_EQ("9a5d804c19632cfe61dbc08250897d757c11f289fa363f8611180be180119ba4", newleaf2->hashkey);
	ASSERT_EQ("90a663d6116ade221b3bbacde19ef2ed551bc3def24a416f6ce54f36e78c692c", newleaf3->hashkey);
	ASSERT_EQ("840f557345f1f0705c0fe22c982cba54d0da90c15f957d6c376fcf8aa0916ade", newleaf4->hashkey);


	// Child node pointers should all be initialized to NULL
	ASSERT_FALSE(newleaf1->left);
	ASSERT_FALSE(newleaf1->right);
	ASSERT_FALSE(newleaf2->left);
	ASSERT_FALSE(newleaf2->right);
	ASSERT_FALSE(newleaf3->left);
	ASSERT_FALSE(newleaf3->right);
	ASSERT_FALSE(newleaf4->left);
	ASSERT_FALSE(newleaf4->right);
}


// Tests the initialization of new tree nodes (InitTreeNode)
TEST_F(test_Merkle, TestInitTreeNode){
	MerkleTree mytree; // Create tree object

	shared_ptr<Node> l = mytree.InitLeaf("re-"); // Initialize two leaf nodes
	shared_ptr<Node> r = mytree.InitLeaf("-markable");

	string lhash = l->hashkey; // hashkey for "re-"
	string rhash = r->hashkey; // hashkey for "-markable"

	string concat = lhash + rhash; // Concatenation of left & right nodes' hashes
	string newhash = sha256(concat); // Hash of the concatenated hashkeys


	// Initialize new tree node with l and r as its child nodes
	shared_ptr<Node> treenode = mytree.InitTreeNode(l, r);


	// Check that treenode's hash should be the hash of the concatenated hashkeys of l & r
	ASSERT_EQ(treenode->hashkey, newhash); 

	// Check that treenode's children were set properly
	ASSERT_EQ(treenode->left, l);
	ASSERT_EQ(treenode->right, r);

	// Check that treenode has been set as the parent node of l & r
	ASSERT_EQ(l->parent, treenode);
	ASSERT_EQ(r->parent, treenode);

	// New tree node's parent should be undefined (nullptr)
	ASSERT_FALSE(treenode->parent);

}


// Test the tree/branch node counters (GetSize)
TEST_F(test_Merkle, TestGetSize) {
	MerkleTree mytreeR;
	MerkleTree mytreeL;
	MerkleTree mytree;

	shared_ptr<Node> n1 = mytree.InitLeaf("example"); // Initialize leaf nodes
	shared_ptr<Node> n2 = mytree.InitLeaf("example");
	shared_ptr<Node> n3 = mytree.InitLeaf("example");
	shared_ptr<Node> n4 = mytree.InitLeaf("example");
	shared_ptr<Node> n5 = mytree.InitLeaf("example");
	shared_ptr<Node> n6 = mytree.InitLeaf("example");
	shared_ptr<Node> n7 = mytree.InitLeaf("example");
	shared_ptr<Node> n8 = mytree.InitLeaf("example");


	// Test node counter on a right branch
	shared_ptr<Node> r1 = mytreeR.InitTreeNode(n1, n2); // +2 leaf nodes (n1 & n2)
	shared_ptr<Node> r2 = mytreeR.InitTreeNode(n3, r1); // +1 tree node (r1)
	shared_ptr<Node> r3 = mytreeR.InitTreeNode(n4, r2); // +1 tree node (r2), +1 leaf node (r2->left = n3)
	shared_ptr<Node> r4 = mytreeR.InitTreeNode(n5, r3); // +1 tree node (r3), +1 leaf node (r3->left = n4)
	shared_ptr<Node> rtest_root = mytreeR.InitTreeNode(n6, r4); // +1 tree node (r5), +1 leaf node (r5->left = n5)
	mytreeR.SetRoot(rtest_root); // Set root of the right test tree

	shared_ptr<Node> rRoot = mytreeR.GetRoot()->right;
	ASSERT_EQ( mytreeR.GetSize(rRoot), 9 ); // Total nodes in right branch should be 9



	// Test node counter on a left branch
	shared_ptr<Node> l1 = mytreeL.InitTreeNode(n1, n2); // +2 leaf nodes (n1 & n2)
	shared_ptr<Node> l2 = mytreeL.InitTreeNode(l1, n3); // +1 tree node (l1)
	shared_ptr<Node> l3 = mytreeL.InitTreeNode(l2, n4); // +1 tree node (l2), +1 leaf node (l2->right = n3)
	shared_ptr<Node> ltest_root = mytreeL.InitTreeNode(l3, n5); // +1 tree node (l3), +1 leaf node (l3->right = n4)
	mytreeL.SetRoot(ltest_root); // Set root of the left test tree

	shared_ptr<Node> lRoot = mytreeL.GetRoot()->left;
	ASSERT_EQ( mytreeL.GetSize(lRoot), 7 ); // Total nodes in left branch should be 7



	// Test node counter on an entire (assymetric) tree
	
	// Left branch = 11 nodes (6 leaves, 5 tree nodes)
		shared_ptr<Node> m1 = mytree.InitTreeNode(n1, n2); // +2 leaf nodes
		shared_ptr<Node> m2 = mytree.InitTreeNode(m1, n3); // +1 left tree node (m1), +1 right leaf (n3)
		shared_ptr<Node> m3 = mytree.InitTreeNode(n4, m2); // +1 left leaf (n4), +1 right tree node (m2)
		shared_ptr<Node> m4 = mytree.InitTreeNode(n5, m3); // +1 left leaf (n5), +1 right tree node (m3)
		shared_ptr<Node> m5 = mytree.InitTreeNode(m4, n6); // +1 left tree node (m4), +1 right leaf (n6)
														   // +1 left tree node (m5)


	// Right branch = 3 nodes (2 leaves, 1 tree node)
		shared_ptr<Node> m6 = mytree.InitTreeNode(n7, n8); // +2 leaf nodes (n7, n8)
														   // +1 right tree node (m6)
	// Root node
		shared_ptr<Node> m7 = mytree.InitTreeNode(m5, m6); // +1 tree node

		mytree.SetRoot(m7); // Set root of whole test tree

		ASSERT_EQ(mytree.GetSize(), 15); // Should be 15 nodes in tree (11 in left branch, 3 in right branch, 1 root node)
	


}


// Test the gauge of tree/subtree depth (GetDepth)
TEST_F(test_Merkle, TestGetDepth) {
	
	MerkleTree mytree;
	MerkleTree emptyTree;

	// Empty tree does not have a depth; should return the sentinel value (-1)
	int dEmpty = emptyTree.GetDepth();
	ASSERT_EQ(dEmpty, -1);

	shared_ptr<Node> n1 = mytree.InitLeaf("example"); // Initialize leaf nodes
	shared_ptr<Node> n2 = mytree.InitLeaf("example");
	shared_ptr<Node> n3 = mytree.InitLeaf("example");
	shared_ptr<Node> n4 = mytree.InitLeaf("example");
	shared_ptr<Node> n5 = mytree.InitLeaf("example");
	shared_ptr<Node> n6 = mytree.InitLeaf("example");
	shared_ptr<Node> n7 = mytree.InitLeaf("example");
	shared_ptr<Node> n8 = mytree.InitLeaf("example");


	
	// Left subtree
	// 11 nodes (6 leaves, 5 tree nodes); Subtree depth = 5
	shared_ptr<Node> m1 = mytree.InitTreeNode(n1, n2); // +2 leaf nodes
	shared_ptr<Node> m2 = mytree.InitTreeNode(m1, n3); // +1 left tree node (m1), +1 right leaf (n3)
	shared_ptr<Node> m3 = mytree.InitTreeNode(n4, m2); // +1 left leaf (n4), +1 right tree node (m2)
	shared_ptr<Node> m4 = mytree.InitTreeNode(n5, m3); // +1 left leaf (n5), +1 right tree node (m3)
	shared_ptr<Node> m5 = mytree.InitTreeNode(m4, n6); // +1 left tree node (m4), +1 right leaf (n6)
													   // +1 left tree node (m5)
	int dLeft = mytree.GetDepth(m5);
	ASSERT_EQ(dLeft, 5);


	// Right subtree
	// 3 nodes (2 leaves, 1 tree node); Subtree depth = 1
	shared_ptr<Node> m6 = mytree.InitTreeNode(n7, n8); // +2 leaf nodes (n7, n8)
													   // +1 right tree node (m6)
	int dRight = mytree.GetDepth(m6);
	ASSERT_EQ(dRight, 1);


	// Full tree (combine left & right subtrees by adding a root node)
	// 15 nodes (8 leaves, 7 tree nodes); Tree depth = 6
	shared_ptr<Node> m7 = mytree.InitTreeNode(m5, m6); // +1 tree node
	mytree.SetRoot(m7); 
	int dFull = mytree.GetDepth();

	ASSERT_EQ(dFull, 6); // Depth of full tree should be 1 greater than the depth of the deeper subtree
	

}


// Test the insertion of new nodes into the tree
TEST_F(test_Merkle, TestInsertNode) {
	
	// Token object to initialize leaf nodes
	MerkleTree mytree;

	ASSERT_FALSE(mytree.GetRoot()); // Sanity check; root pointer of empty tree should be null

	shared_ptr<Node> n1 = mytree.InitLeaf("example"); // Initialize set of leaf nodes
	shared_ptr<Node> n2 = mytree.InitLeaf("example");
	shared_ptr<Node> n3 = mytree.InitLeaf("example");
	shared_ptr<Node> n4 = mytree.InitLeaf("example");
	shared_ptr<Node> n5 = mytree.InitLeaf("example");
	shared_ptr<Node> n6 = mytree.InitLeaf("example");
	shared_ptr<Node> n7 = mytree.InitLeaf("example");
	shared_ptr<Node> n8 = mytree.InitLeaf("example");


	// Test insertion into empty tree
		MerkleTree mytree_testEmpty;
		mytree_testEmpty.InsertNode(n1);
		shared_ptr<Node> tEmptyRoot = mytree_testEmpty.GetRoot();
		int treesize = mytree_testEmpty.GetSize();

		ASSERT_EQ(treesize, 1); // Tree should now contain one node
		ASSERT_TRUE( mytree_testEmpty.IsRoot(n1) );  // Node should have been inserted at the root


	
	// Test placement of new nodes by insert function
		MerkleTree mytree_testPlacement;
		mytree_testPlacement.InsertNode(n1); // Insert node 1; should become tree root
			ASSERT_TRUE(mytree_testPlacement.IsRoot(n1));
			ASSERT_EQ(mytree_testPlacement.GetSize(), 1);
		mytree_testPlacement.InsertNode(n2); // Insert node 2; tree size should be 3; n1 should be root_ptr->left; n2 should be root_ptr->right;
			shared_ptr<Node> root = mytree_testPlacement.GetRoot();
			treesize = mytree_testPlacement.GetSize();
			ASSERT_EQ(n1, root->left);
			ASSERT_EQ(n2, root->right);
			ASSERT_EQ(treesize, 3);
		mytree_testPlacement.InsertNode(n3, root); // Insert node 3; since branches are equal, should be placed on the left (size of left branch should increase by 2); n3 == root_ptr->left->right
			treesize = mytree_testPlacement.GetSize();
			ASSERT_EQ(treesize, 5);
			ASSERT_EQ(n1, root->left->left);
			ASSERT_EQ(n3, root->left->right);
		mytree_testPlacement.InsertNode(n4); // Insert node 4; left side should be larger, so n4 should go right; GetSize(root->right) should now be 3 (with new internal node); n4 == root_ptr->right->right
			treesize = mytree_testPlacement.GetSize();
			ASSERT_EQ(treesize, 7);
			int rightsize = mytree_testPlacement.GetSize(root->right);
			int leftsize = mytree_testPlacement.GetSize(root->left);
			ASSERT_EQ(rightsize, leftsize);
			ASSERT_EQ(n2, root->right->left);
			ASSERT_EQ(n4, root->right->right);

	
	// Test InsertNode null case
		MerkleTree mytree_testNull;
		shared_ptr<Node> insertPosition = n8->left; // n8 was initialized, but not added to the tree; its left child should point to NULL
		bool insertSuccess = mytree_testNull.InsertNode(n7, insertPosition); // Attempt to insert n7 should fail and return false
		ASSERT_FALSE(insertSuccess);

}


// Test detection of data within hash tree
TEST_F(test_Merkle, TestContains) {
	MerkleTree mytree;

	string m1 = "grape";
	string hash1 = sha256(m1); // The SHA-256 hash for the string "grape"
	shared_ptr<Node> newleaf1 = mytree.InitLeaf("grape");

	string m2 = "Philip K. Dick";
	string hash2 = sha256(m2); // The SHA-256 hash for the string "Philip K. Dick"
	shared_ptr<Node> newleaf2 = mytree.InitLeaf("Philip K. Dick");

	string m3 = "All the King's horses & all the King's men";
	string hash3 = sha256(m3); // The SHA-256 hash for the string "All the King's horses & all the King's men"
	shared_ptr<Node> newleaf3 = mytree.InitLeaf("All the King's horses & all the King's men");

	string m4 = "285 thousand 3 hundred sixty 4";
	string hash4 = sha256(m4); // The SHA-256 hash for the string "285 thousand 3 hundred sixty 4"
	shared_ptr<Node> newleaf4 = mytree.InitLeaf("285 thousand 3 hundred sixty 4");

	string m5 = "Oh sweet Lord in Heaven I am tired!";
	string hash5 = sha256(m5); // The hash for string m5
	shared_ptr<Node> newleaf5 = mytree.InitLeaf("Oh sweet Lord in Heaven I am tired!");

	string m6 = "This string doesn't get put in the tree";
	string hash6 = sha256(m6);
	



	// When inserted in the order 1, 2, 3, 4, 5, the tree should be as follows:
	//		 			[_______r_______]
	//      	  		  |				|
	//  		  [r->left]			    [r->right]
	//   		|		   |			|		  |	
	//     [r->l->l]	   newleaf3     newleaf2  newleaf4
	//	   |       |
	//  newleaf1  newleaf5


	mytree.InsertNode(newleaf1);
	mytree.InsertNode(newleaf2);
	mytree.InsertNode(newleaf3);
	mytree.InsertNode(newleaf4);
	mytree.InsertNode(newleaf5);
	shared_ptr<Node> treeroot = mytree.GetRoot();

	
	
	// Test detection of data in tree
		ASSERT_TRUE(mytree.ContainsData("Oh sweet Lord in Heaven I am tired!")); // Should be true
		ASSERT_TRUE(mytree.ContainsData("Philip K. Dick")); // Should also be true

		ASSERT_FALSE(mytree.ContainsData("This string is not in the hash tree")); // Should be false
		ASSERT_FALSE(mytree.ContainsData("All the King's horses & all the King' men")); // Should be false (missing 's')

	// Test detection of data in subtree
		ASSERT_TRUE(mytree.ContainsData("Oh sweet Lord in Heaven I am tired!", treeroot->left)); // Should be true
		ASSERT_TRUE(mytree.ContainsData("Philip K. Dick", treeroot->right)); // Should also be true

		ASSERT_TRUE(mytree.ContainsData("285 thousand 3 hundred sixty 4", treeroot->right)); // Should be true; this string is in the right branch
		ASSERT_FALSE(mytree.ContainsData("All the King's horses & all the King's men", treeroot->left->left)); // Should be false; this string is in the branch root->left->right
		ASSERT_FALSE(mytree.ContainsData("All the King's horse & all the King's men", treeroot->left->right)); // Should be false; another typo


	// Test detection of hashkey in tree
		ASSERT_FALSE(mytree.ContainsHash("9a5d804c19632cfe61dbc08250897d757c11f288fa363f8611180be180119ba4")); // Incorrect hashkey for "Philip K. Dick"
		ASSERT_TRUE(mytree.ContainsHash(hash1)); // Hashkey for "grape" (should be true)
		ASSERT_FALSE(mytree.ContainsHash(hash6)); // This hash is not inserted into the tree

	// Test detection of hashkey in subtree
		ASSERT_TRUE(mytree.ContainsHash("90a663d6116ade221b3bbacde19ef2ed551bc3def24a416f6ce54f36e78c692c", treeroot->left->right)); // Hashkey for "All the king's horses..." (should be true)
		ASSERT_FALSE(mytree.ContainsHash("7840305e8ef5484a740fab4e4f112f557ee61b8bd4f443f26a967777ebd8a0b4", treeroot->right)); // Hashkey is in the tree, but not in the subtree
		ASSERT_FALSE(mytree.ContainsHash("840f557345f1f0705c0fe22b982cba54d0da90c15f957d6c376fcf8aa0916ade", treeroot->right->right)); // This is the proper branch, but one of the hashkey chars is wrong

}


// Test functions for finding data & hash values in trees & subtrees
TEST_F(test_Merkle, TestFind) {

	MerkleTree mytree;

	string m1 = "grape";
	string hash1 = "0f78fcc486f5315418fbf095e71c0675ee07d318e5ac4d150050cd8e57966496"; // The SHA-256 hash for the string "grape"
	shared_ptr<Node> newleaf1 = mytree.InitLeaf("grape");

	string m2 = "Philip K. Dick";
	string hash2 = "9a5d804c19632cfe61dbc08250897d757c11f289fa363f8611180be180119ba4"; // The SHA-256 hash for the string "Philip K. Dick"
	shared_ptr<Node> newleaf2 = mytree.InitLeaf("Philip K. Dick");

	string m3 = "All the King's horses & all the King's men";
	string hash3 = "90a663d6116ade221b3bbacde19ef2ed551bc3def24a416f6ce54f36e78c692c"; // The SHA-256 hash for the string "All the King's horses & all the King's men"
	shared_ptr<Node> newleaf3 = mytree.InitLeaf("All the King's horses & all the King's men");

	string m4 = "285 thousand 3 hundred sixty 4";
	string hash4 = "840f557345f1f0705c0fe22c982cba54d0da90c15f957d6c376fcf8aa0916ade"; // The SHA-256 hash for the string "285 thousand 3 hundred sixty 4"
	shared_ptr<Node> newleaf4 = mytree.InitLeaf("285 thousand 3 hundred sixty 4");

	string m5 = "Oh sweet Lord in Heaven I am tired!";
	string hash5 = "7840305e8ef5484a740fab4e4f112f557ee61b8bd4f443f26a967777ebd8a0b4"; // The hash for string m5
	shared_ptr<Node> newleaf5 = mytree.InitLeaf("Oh sweet Lord in Heaven I am tired!");

	string m6 = "This string doesn't get put in the tree";
	string hash6 = sha256(m6);
	

	// When inserted in the order 1, 2, 3, 4, 5, the tree should be as follows:
	//		 			[_______r_______]
	//      	  		  |				|
	//  		  [r->left]			    [r->right]
	//   		|		   |			|		  |	
	//     [r->l->l]	   newleaf3     newleaf2  newleaf4
	//	   |       |
	//  newleaf1  newleaf5


	mytree.InsertNode(newleaf1);
	mytree.InsertNode(newleaf2);
	mytree.InsertNode(newleaf3);
	mytree.InsertNode(newleaf4);
	mytree.InsertNode(newleaf5);
	shared_ptr<Node> treeroot = mytree.GetRoot();
	
	// Test location of data in tree
		ASSERT_EQ(mytree.FindData("Philip K. Dick"), treeroot->right->left);
		ASSERT_NE(mytree.FindData("grape"), treeroot->right->right);
		ASSERT_FALSE(mytree.FindData("This isn't in the tree"));

	// Test location of data in subtree
		ASSERT_EQ(mytree.FindData("All the King's horses & all the King's men", treeroot->left), treeroot->left->right);
		ASSERT_FALSE(mytree.FindData("Oh sweet Lord in Heaven I am tired!", treeroot->left) == treeroot->left->left->left);
		ASSERT_FALSE(mytree.FindData("Philip K. Dick", treeroot->left));

	// Test location of hashkey in tree
		ASSERT_EQ(mytree.FindHash("840f557345f1f0705c0fe22c982cba54d0da90c15f957d6c376fcf8aa0916ade"), treeroot->right->right);
		ASSERT_FALSE(mytree.FindHash("0f78fcc486f5315418fbf095e71c0675ee07d318e5ac4d150050cd8e57966496") == treeroot->left->left->right);

	// Test location of hashkey in tree
		ASSERT_EQ(mytree.FindHash("9a5d804c19632cfe61dbc08250897d757c11f289fa363f8611180be180119ba4", treeroot->right), treeroot->right->left);
		ASSERT_FALSE(mytree.FindHash("840f557345f1f0705c0fe22c982cba54d0da90c15f957d6c376fcf8aa0916ade", treeroot->right) == treeroot->right->left);


}

// Test the verification of data in tree copies
TEST_F(test_Merkle, TestVerify) {
	MerkleTree mytree;
	MerkleTree copy1;
	MerkleTree copy2;

	string m1 = "grape";
	string hash1 = sha256(m1); // The SHA-256 hash for the string "grape"
	shared_ptr<Node> newleaf1 = mytree.InitLeaf("grape");

	string m2 = "Philip K. Dick";
	string hash2 = sha256(m2); // The SHA-256 hash for the string "Philip K. Dick"
	shared_ptr<Node> newleaf2 = mytree.InitLeaf("Philip K. Dick");

	string m3 = "All the King's horses & all the King's men";
	string hash3 = sha256(m3); // The SHA-256 hash for the string "All the King's horses & all the King's men"
	shared_ptr<Node> newleaf3 = mytree.InitLeaf("All the King's horses & all the King's men");

	string m4 = "285 thousand 3 hundred sixty 4";
	string hash4 = sha256(m4); // The SHA-256 hash for the string "285 thousand 3 hundred sixty 4"
	shared_ptr<Node> newleaf4 = mytree.InitLeaf("285 thousand 3 hundred sixty 4");

	string m5 = "Oh sweet Lord in Heaven I am tired!";
	string hash5 = sha256(m5); // The hash for string m5
	shared_ptr<Node> newleaf5 = mytree.InitLeaf("Oh sweet Lord in Heaven I am tired!");

	string m6 = "This string doesn't get put in the tree";
	string hash6 = sha256(m6);

	


	// Add nodes to mytree
	mytree.InsertNode(newleaf1);
	mytree.InsertNode(newleaf2);
	mytree.InsertNode(newleaf3);
	mytree.InsertNode(newleaf4);
	mytree.InsertNode(newleaf5);


	// Add nodes to copy1 (should be the same as mytree)
	copy1.InsertNode(newleaf1);
	copy1.InsertNode(newleaf2);
	copy1.InsertNode(newleaf3);
	copy1.InsertNode(newleaf4);
	copy1.InsertNode(newleaf5);

	// Change data for copy2 (modified tree)
	shared_ptr<Node> otherleaf1 = copy2.InitLeaf("grape");
	shared_ptr<Node> otherleaf2 = copy2.InitLeaf("Philip K. Dick");
	shared_ptr<Node> otherleaf3 = copy2.InitLeaf("All the King's horses & all the King's men");
	shared_ptr<Node> otherleaf4 = copy2.InitLeaf("eight hundred sixty 4");
	shared_ptr<Node> otherleaf5 = copy2.InitLeaf("Oh golly jeepers I am pooped!!");
	copy2.InsertNode(otherleaf1); // Add to list in same order
	copy2.InsertNode(otherleaf2);
	copy2.InsertNode(otherleaf3);
	copy2.InsertNode(otherleaf4);
	copy2.InsertNode(otherleaf5);


	ASSERT_TRUE(mytree.Verify(copy1)); // copy1 is identical, so should return true
	ASSERT_FALSE(mytree.Verify(copy2)); // copy2 is modified, so should return false


	


}


// Test functions which update hash keys in the tree (Update)
TEST_F(test_Merkle, TestFixHash) {


	MerkleTree t1;
	MerkleTree t2;

	string m1 = "grape";
	string hash1 = sha256(m1); // The SHA-256 hash for the string "grape"
	shared_ptr<Node> newleaf1 = t1.InitLeaf("grape");
	shared_ptr<Node> cleaf1 = t2.InitLeaf(m1);

	string m2 = "Philip K. Dick";
	string hash2 = sha256(m2); // The SHA-256 hash for the string "Philip K. Dick"
	shared_ptr<Node> newleaf2 = t1.InitLeaf("Philip K. Dick");
	shared_ptr<Node> cleaf2 = t2.InitLeaf(m2);

	string m3 = "All the King's horses & all the King's men";
	string hash3 = sha256(m3); // The SHA-256 hash for the string "All the King's horses & all the King's men"
	shared_ptr<Node> newleaf3 = t1.InitLeaf("All the King's horses & all the King's men");
	shared_ptr<Node> cleaf3 = t2.InitLeaf(m3);


	string m4 = "285 thousand 3 hundred sixty 4";
	string hash4 = sha256(m4); // The SHA-256 hash for the string "285 thousand 3 hundred sixty 4"
	shared_ptr<Node> newleaf4 = t1.InitLeaf("285 thousand 3 hundred sixty 4");
	shared_ptr<Node> cleaf4 = t2.InitLeaf(m4);

	string m5 = "Oh sweet Lord in Heaven I am tired!";
	string hash5 = sha256(m5); // The hash for string m5
	shared_ptr<Node> newleaf5 = t1.InitLeaf("Oh sweet Lord in Heaven I am tired!");
	shared_ptr<Node> cleaf5 = t2.InitLeaf(m5);


	// This is the data whose hash replaces hash5 in the other tree
	string m6 = "This string doesn't get put in the tree";
	string hash6 = sha256(m6);


	// Add nodes to t1
	t1.InsertNode(newleaf1);
	t1.InsertNode(newleaf2);
	t1.InsertNode(newleaf3);
	t1.InsertNode(newleaf4);
	t1.InsertNode(newleaf5);
	ASSERT_EQ( t1.GetSize(), 9); // Should have 5 leaves & 4 internal nodes


	shared_ptr<Node> t1Root = t1.GetRoot();

	// Add nodes to t2
	t2.InsertNode(cleaf1);
	t2.InsertNode(cleaf2);
	t2.InsertNode(cleaf3);
	t2.InsertNode(cleaf4);
	t2.InsertNode(cleaf5);

	shared_ptr<Node> t2Root = t2.GetRoot();

	ASSERT_TRUE(t1.Verify(t2));	// Should be identical



	// Test ModifyLeaf & Resolve


		t2.ModifyLeaf(t2Root->left->left->right, m6); // Update data



		ASSERT_FALSE(t1.Verify(t2)); // Should no longer be identical
		ASSERT_EQ(t2Root->left->left->right->hashkey, hash6); // Check if data was updated properly
		ASSERT_EQ(t2Root->left->left->right->parent->hashkey, sha256(hash1 + hash6)); // Check that parent was correctly updated by Resolve()


	// Test AcceptMods

		t1.AcceptMods(t2); // Update t1 with the changes made to t2

		ASSERT_TRUE(t1.Verify(t2)); // Check that the trees are identical
		ASSERT_EQ(t1Root->left->left->right->hashkey, hash6); // Make sure t1 got t2's data and not vice versa
}

// Test deletion of leaves
TEST_F(test_Merkle, TestDeleteLeaf) {

	// Initialize tree
	MerkleTree mytree;

	string m1 = "grape";
	string hash1 = sha256(m1); // The SHA-256 hash for the string "grape"
	shared_ptr<Node> newleaf1 = mytree.InitLeaf("grape");

	string m2 = "Philip K. Dick";
	string hash2 = sha256(m2); // The SHA-256 hash for the string "Philip K. Dick"
	shared_ptr<Node> newleaf2 = mytree.InitLeaf("Philip K. Dick");

	string m3 = "All the King's horses & all the King's men";
	string hash3 = sha256(m3); // The SHA-256 hash for the string "All the King's horses & all the King's men"
	shared_ptr<Node> newleaf3 = mytree.InitLeaf("All the King's horses & all the King's men");

	string m4 = "285 thousand 3 hundred sixty 4";
	string hash4 = sha256(m4); // The SHA-256 hash for the string "285 thousand 3 hundred sixty 4"
	shared_ptr<Node> newleaf4 = mytree.InitLeaf("285 thousand 3 hundred sixty 4");

	string m5 = "Oh sweet Lord in Heaven I am tired!";
	string hash5 = sha256(m5); // The hash for string m5
	shared_ptr<Node> newleaf5 = mytree.InitLeaf("Oh sweet Lord in Heaven I am tired!");



	mytree.InsertNode(newleaf1);
	mytree.InsertNode(newleaf2);
	mytree.InsertNode(newleaf3);
	mytree.InsertNode(newleaf4);
	mytree.InsertNode(newleaf5);

	ASSERT_EQ(mytree.GetSize(), 9); // Should now have nine nodes

	shared_ptr<Node> root = mytree.GetRoot();


	// Test that internal nodes aren't removed
		
		mytree.DeleteLeaf(root); // Should do nothing
		ASSERT_EQ(mytree.GetSize(), 9);

		mytree.DeleteLeaf(root->left); // Should also do nothing
		ASSERT_EQ(mytree.GetSize(), 9);

	// Test Deletion of leaves


		shared_ptr<Node> sibling = mytree.GetSibling(root->left->left->right); // Keep track of which node will be moved up

		mytree.DeleteLeaf(root->left->left->right); // Remove node 5

		ASSERT_EQ(mytree.GetSize(), 7); // Two nodes should have been removed
		ASSERT_EQ(sibling->parent, root->left); // Parent of updated node should be the left child of root
		ASSERT_EQ(mytree.GetSibling(sibling), root->left->right); // Node 1's new sibling should be Node 3
		ASSERT_EQ(root->left->hashkey, sha256(hash1 + hash3)); // Make sure ancestral hashes were updated properly

		mytree.DeleteLeaf(root->right->left); // Delete nodes 2 and 4
		mytree.DeleteLeaf(root->right);

		ASSERT_EQ(mytree.GetSize(), 3); // Only 3 nodes should remain 

		ASSERT_FALSE(mytree.GetRoot() == root); // Tree's root should have been deleted and the root_ptr updated


		root = mytree.GetRoot(); // reset the root

		mytree.DeleteLeaf(root->right); // remove node 3


		ASSERT_EQ(sibling, mytree.GetRoot()); // Node one should now be the root
		ASSERT_EQ(mytree.GetSize(), 1); // Root should be the only node in tree


		// Remove final node
		mytree.DeleteLeaf(sibling);

		ASSERT_EQ(mytree.GetSize(), 0); // No nodes should remain
		ASSERT_FALSE(mytree.GetRoot()); // Root pointer should now be null

}