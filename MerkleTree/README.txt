
Samuel Terry

Sina Aghli

CSPB 2270: Data Structures

11 December, 2020



Final Project: Merkle Tree Implementation
-----------------------------------------

Code Index ('MerkleTree/code')
	- Merkle.h -- header file; declarations for the 'MerkleTree' class & its member functions, as well as the 'Node' struct
	- Merkle.cpp -- definitions for member functions of the MerkleTree class
	- I decided to use SHA-256 as my hashing function for this project, as this hashing function was recommended by a number
		of sources I consulted while learning about Merkle Trees. I did not implement this algorithm myself, however. I found
		the code for SHA-256 at: http://www.zedwood.com/article/cpp-sha256-function. This site included a header file, declaring
		the SHA256 class and its member functions, as well as a .cpp file with the definitions of said member functions. There was also a file
		'LICENSE.txt' with credit to the author; I included the text of this license at the top of both sha256.cpp & sha256.h 

HOW TO BUILD:
	- From the command line, navigate to the 'build' directory within the main project directory
	- If the build directory is empty, run 'cmake ..' from within the build directory to create a Makefile
	- While in the build directory, run 'make' on the command line to build project, './run_tests' to run the tests, and './run_app' to run whatever code is in the 'app' directory
	- To test code, run the command './run_tests'



About:

		This project is an implementation of the Merkle Tree data structure in C++. A Merkle Tree is a tree-type data 
		structure in which leaf nodes contain hashes of some form of data (strings, in this case), while tree nodes
		contain hashes of the concatenated hashes of their children. For instance:

                        _________________
                        |  (root node)   |    <----------- tree node
                        | m3 = h(m1 + m2)|
                        _________________
                        /               \
                       /                  \
                      /                     \
            ______________              ______________
            |  leaf node  |             |  leaf node  |
            | m1 = h(d1)  |             | m2 = h(d2)  |    <--------- leaf nodes
            ______________              ______________ 
                |                           |
            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   <-------------- hash encoding of data
                |                           |
            ______________              ______________
            |data  (d1)   |             |  data (d2)  |    <-------------- data 
            ______________              ______________ 


		The above diagram depicts an example of a Merkle Tree. In the bottom boxes, d1 and d2 represent some data--strings, at least in the case of this implementation. The data elements themselves are not leaves of the tree,
	but each datum corresponds to exactly one leaf node in the tree. The leaf nodes contain their own values, m1 & m2, each of which is a hash of the string to which that leaf node corresponds. This diagram contains only one
	tree node (the root of the tree). The hash value m3 in the tree node is itself a hash of the concatenated hash strings m1 & m2. In summary, each tree node's hash value is determined by the hash values of its children,
	and each leaf node's hash value is determined by the data to which that leaf node corresponds. 

		In a tree data structure, leaf nodes do not have child nodes of their own. In this implementation, the Merkle Tree is a binary tree, so a tree node can have no more than two children. Additionally, however, 
	tree nodes in a Merkle Tree cannot have only one child; there is no reason to create a tree node with only one child, as the tree node's hash would just be a hash of another individual hash. Consequently, in this
	Merkle tree implementation, a node's type is easily identified by the number of children that node has. A tree node must have 2 children, and a leaf node can have no children. So, for example, if a Merkle Tree consists only
	of one node (the root), that node must be a leaf node and will have no children. If, however, another leaf node is added to the tree, a tree node is created by hashing the hash keys of the two leaf nodes, and this tree
	node becomes the new root. Leaf nodes cannot become tree nodes, nor can tree nodes become leaves. In this implementation, a Merkle Tree must have an odd number of nodes or no nodes at all; a tree can consist of just a leaf
	node at the root, but any time a new leaf node is added, a new tree node must also be added to serve as the parent for the new node and one of the old leaf nodes.

		Merkle Trees are useful because they make it easy to detect changes to a set of data. For instance, if we were to add L = 4 leaf nodes--corresponding to strings d1, d2, d3, and d4--to an empty Merkle Tree with 
	hash function h, the tree would have 2L-1 nodes, since every leaf added after the first would also require the addition of a tree node. If, once we had created our tree, we then changed the value of d2, we could create 
	another Merkel Tree of the same size; however, since the value of d2 changes, so does the hash value of corresponding leaf node l_2. Since l_2 determines the hash key of its parent node, and the hash of that parent determines
	the hash value of its parent, the hash key will change for every node that is an ancestor of node l_2--including the root. Thus, if we have a Merkle Tree of some set of data, and someone else presents us with another Merkel
	Tree made from the same dataset, we can determine if any of the data from the new tree has been altered by simply comparing the hash keys of the two trees' root nodes. If there is a discrepancy, then at least one datum
	in the second tree has been altered. If this is the case, we can also easily determine which node or nodes has/have been altered by comparing the hash keys of the root's child nodes. If there is a discrepancy between the
	hash values of two corresponding nodes in a pair of Merkel Trees, it means that at least one of the data items in that node's branch has been altered.

		In this implementation, a MerkleTree object is essentially a binary tree consisting of "Node" structs. Each Node struct contains a pointer to the Node's parent (or a null pointer if the node is the root), pointers to
	the node's left & right children (or null pointers, if the node is a leaf), and, critically, a string object known as the "hashkey." For leaf nodes, the hashkey is the hashed encoding of the string to which each leaf node corresponds.
	For internal tree nodes, the hashkey is a hash of its child nodes' concatenated hashkeys. The initialization of leaf nodes is rather straightforward; after allocating memory for a Node object, the Node's parent & child node pointers
	are initialized to NULL, the string data for that node is passed through a hash function. The string output by this function serves as they hashkey value for that Node.  By contrast, the initialization of internal tree nodes
	requires two other node objects, since the purpose of this type of node is to encode the hashkeys of child nodes. The initialization of tree nodes requires only the pointers to the two nodes that are to become the child nodes of the new
	internal node. Once allocated, the 'parent' fields in the child node objects are set with a pointer to the new internal node, and pointers to those child nodes are used to set the 'left' & 'right' child pointer fields for the internal node.
	Lastly, the hashkey strings of the child nodes are concatenated together into a new string; this new string is encoded by the hash function, and the hashed result is used as the hashkey for the new parent node. 

		The MerkleTree class itself contains only one private data member--a pointer to the tree's root so that the tree can be located and operated upon. The class also contains a diverse assortment of public member functions which allow
	for different operations on a Merkle Tree. To name a few:
			- InitLeaf will take string data, hash it, and store this hash key in a new Merkle Tree node

			- InsertNode adds an initialized leaf node to the tree, updating pointers to and from this new leaf. It also updates the hash keys of all the new leaf's ancestors. 

				* If no target argument is passed, then the node can be inserted anywhere in the tree; if a target is passed, it will be inserted within the subtree starting at that target node. 

				* InsertNode attempts to keep the tree balanced by recursively searching for an optimal leaf at which to insert the node. If two branches have the same number of nodes, the node is inserted in the left
					branch by default, but will be inserted in the right branch if the left branch currently has more nodes
			
			- DeleteLeaf removes the specified leaf node. It also removes the (now obsolete) parent of this node, replacing it with the sibling of the deleted leaf & updating the tree's hash keys accordingly. 

			- ContainsHash recursively searches for the specified hash key in the tree, returning true if a matching node is found. ContainsData takes some string data & recursively searches for a node
				containing the hash of this data. FindData/FindHash also search for particular hashes/strings & return the first matching node if any exist in the tree.

			- Verify compares two copies of a tree & returns true only if the data they contain is identical. This could be done recursively with an ordinary binary tree, but for a Merkle tree, 
				this can be accomplished just by comparing the root hashes. 

			- Resolve updates the hash keys of a node's ancestors if the node in question has been changed. It is used in the ModifyLeaf function, which changes the data hashed in a particular leaf node.
				It concatenates the hash of the specified node with the hash of that node's sibling, hashes the concatenated string, and stores the hashed concatenation as the hashkey in the parent.

			- AcceptMods accepts any data modifications of a tree within the specified copy of that tree. It resolves inconsistencies between tree data. If the tree being updated contains a branch that was removed
				in the other tree, that branch is deleted. If the tree being updated does not contain a branch which is in the other tree, a copy of this branch is made and inserted into the updated tree
 
		The hash function used in this implementation is SHA-256 (an abbreviation for "Secure Hash Algorithm 256"). 


Sources: 
	- http://www.zedwood.com/article/cpp-sha256-function (code for SHA-256 implementation, imported into my project as a library

	- https://en.wikipedia.org/wiki/Merkle_tree

	- https://www.geeksforgeeks.org/introduction-to-merkle-tree/

