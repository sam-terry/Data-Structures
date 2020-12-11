#include "Merkle.h"
#include "sha256.h" // Refer to ../README.txt for more information

#include <iostream>


// Constructor; initializes class variables/pointers 
MerkleTree::MerkleTree() {
    this->root_ptr = NULL;
}


// Deconstructor not implemented; shared pointers automatically keep track of memory to be freed
MerkleTree::~MerkleTree() {
    
}


// Returns a pointer to the root node in the Merkle Tree
shared_ptr<Node> MerkleTree::GetRoot() {
    return root_ptr;
}


// Set the tree root to the specified node
void MerkleTree::SetRoot(shared_ptr<Node> newroot) {
    root_ptr = newroot;
    newroot->parent = NULL;
}


// Return true if the specified node is at the root of the tree
bool MerkleTree::IsRoot(shared_ptr<Node> node) {
    if (MerkleTree::GetRoot() == node) {return true;}
    return false;
}



/*
 * InitLeaf allocates a new node; it takes a string, hashes it, and uses the result as the hashkey
 * The pointers to the node's left and right children are initialized to NULL
 * @param data The string whose hash is to be stored by the node
 * @return A pointer to the newly initialized leaf node
 */
shared_ptr<Node> MerkleTree::InitLeaf(string data) {
    shared_ptr<Node> newnode(new Node); // Allocate memory for a new node and get its address

    newnode->hashkey = sha256(data); // Hash the input string and store the hash in the new node
    
    newnode->parent = NULL;  // Initialize pointers to child/parent nodes to NULL
    newnode->left = NULL;
    newnode->right = NULL;

    return newnode;
}



/*
 * InitTreeNode allocates a new parent node (a tree node) for the two nodes passed as arguments
 * The new tree node's hashkey is the hash of its children's concatenated hashkeys; its parent is whatever the parent of the left child was
 * The left & right nodes' parent is set to the new node
 * @param left The left child of the new tree node; when inserting a node at a target, the target is the left child of the new internal node
 * @param right The right child of the new tree node; when inserting a node at a target, the newnode is the right child of the new internal node
 * @return A pointer to the newly initialized tree node
 */
shared_ptr<Node> MerkleTree::InitTreeNode(shared_ptr<Node> left, shared_ptr<Node> right) {
    shared_ptr<Node> treenode(new Node); // Allocate memory for a new tree node and get its address
    

    // If left's parent is null, then it was not previously a node in the tree, and the parent of the new treenode will be null
    if (!(left->parent)) {treenode->parent = NULL;}

    // If left's parent is a valid pointer, then left is the target position of an insert operation, and the new treenode is inserted at that target position:
    else {
        treenode->parent = left->parent; // Give treenode the target's parent

        // The new treenode becomes whichever child node the target was of its parent
        if (treenode->parent->left == left) {treenode->parent->left = treenode;}
        else {treenode->parent->right = treenode;}
    }

    

    treenode->left = left; // Set treenode's children 
	treenode->right = right;

	
    string concat = (left->hashkey) + (right->hashkey); // Concatenate hashkeys of left & right nodes
	treenode->hashkey = sha256(concat); // treenode's hashkey is the hash of the concatenation of the left & right nodes' hashshtrings

	
    left->parent = treenode; // left & right child nodes' parent node is the new tree node
	right->parent = treenode;

    return treenode;
}



/*
 * GetSize recursively counts the number of nodes in a subtree when the root node of that subtree is passed as an argument 
 * It is a helper function which enables InsertNode to limit asymmetry between branches
 * @param branchroot the root of the branch whose nodes are to be counted
 * @returns (int) Number of nodes in the branch, including the branch root
 */
int MerkleTree::GetSize(shared_ptr<Node> branchroot) {

    // Base case: root is a leaf node, return 1 for its count
    if (!(branchroot->left)) {return 1;}

    // Recursive case: root is an internal node
    else {
        // Size of branch is the left size + the right size + 1 for branch root 
        return GetSize(branchroot->left) + GetSize(branchroot->right) + 1;
    }
}



/*
 * GetSize, when called without an argument, recursively counts all the nodes in the tree by using the root pointer to identify the target branch
 * @returns (int) umber leaf nodes + number of internal nodes in the entire tree
 */
int MerkleTree::GetSize() {
    
    // If root pointer is null, tree is empty
    if (!root_ptr) {return 0;}

    // If root has no left child, it has no right child either; the tree's only node is the root
    else if (!(root_ptr->left)) {return 1;}


    // If the tree has more than just a root node, recursively count the nodes in each child branch
    else {
        // The no. of nodes in the entire tree is the total number of nodes the root's child branches, plus 1 for the tree's root
        return 1 + GetSize(root_ptr->left) + GetSize(root_ptr->right);
    }
    
}



/* When called with a node as an argument, GetDepth recursively searches for the deepest branch in the subtree starting at that node
 * The root pointer is passed as an argument in
 * @param subroot The root of the subtree whose depth is to be gauged
 * @return (int) The number of nodes in the subtree's deepest branch (not including the subtree's root node)
 */ 
int MerkleTree::GetDepth(shared_ptr<Node> subroot) {
    
    // Base case: root of the subtree is a leaf node; the subtree's max depth is 0
    if (!(subroot->left)) {return 0;}

    // Recursive case: subroot is an internal node
    else {
        int LeftDepth = GetDepth(subroot->left); // Recursively measure max depth of left & right subtrees
        int RightDepth = GetDepth(subroot->right);

        // Find the maximum of the two subtree depths
        int maxDepth = LeftDepth >= RightDepth ? LeftDepth : RightDepth; 

        // Add one to this depth (to measure the current level) & return
        return maxDepth + 1;
    }
}



/*
 * When GetDepth is called without an argument, GetDepth will find the depth of the deepest branch in the entire tree
 * This form of GetDepth will pass the tree's root pointer to the recursive GetDepth function above (which takes a start node as an argument) 
 * @return (int) number of levels in the Merkel Tree's deepest branch (including leaf nodes but excluding the root), or -1 if tree is empty
 */
int MerkleTree::GetDepth() {

    // An empty tree does not have a quantifiable depth; return a sentinel value
    if (!root_ptr) {return -1;}

    // If tree is not empty, call GetDepth recursively on the root node
    return MerkleTree::GetDepth(root_ptr);
}



/*
 * InsertNode takes a node object as an argument and adds it to the tree, minimizing asymmetry between the tree's main left & right branches
 * This version doesn't take a position parameter; the version which does is used to recursively insert the node on the correct side
 * @param newnode The node to be inserted
 */
void MerkleTree::InsertNode(shared_ptr<Node> newnode) {
    
    // If tree is empty, newnode is inserted at the root
    if (!root_ptr) {root_ptr = newnode; return;}


    // If tree's only node is a leaf at the root:
    if (!(root_ptr->left)) {

        // Create a new tree node w/ the root as the left child & the new node as the right (though order doesn't actually matter)
        shared_ptr<Node> newroot = InitTreeNode(root_ptr, newnode);

        // Set the new tree node as the root of the Merkle Tree
        SetRoot(newroot);

        return; // Node inserted successfully
    }

    
    // If tree size is greater than 1, compare sizes of root's left & right child branches to determine where the new node should be inserted
    int Lsize = MerkleTree::GetSize(root_ptr->left);
    int Rsize = MerkleTree::GetSize(root_ptr->right);

    // Insert nodes on the right if the right side currently has fewer nodes
    if (Lsize > Rsize) {
        InsertNode(newnode, root_ptr->right);
    }

    // If the left has the same number of nodes than the right or fewer, insert the new node on the left side
    else {
        InsertNode(newnode, root_ptr->left);
    }


    return;
}



/*
 * If an insert position is passed as an argument, InsertNode will start at that position and search recursively for a place to insert the new node
 * @param newnode The node to be inserted
 * @param target The node to start looking for an optimal insert position
 * @return False if the insert position is invalid, true if the node is successfully inserted
 */
bool MerkleTree::InsertNode(shared_ptr<Node> newnode, shared_ptr<Node> target) {
    
    // Null case: return false if insert position is invalid (i.e. the position pointer is null)
    if (!target) {
        return false;
    }


    // Base case: target position is a leaf node--replace it with a new internal node w/ target node & new node as children
    if (!(target->left)) {

        shared_ptr<Node> treenode = InitTreeNode(target, newnode); // Rotate target down and to the left; 
        Resolve(newnode);
        return true; // Insertion successful

    }


    // Recursive case: target is an internal node (subtree size > 1); recursively search for an insertion point
    else {

        // Get size of left & right subtrees
        int Lsize = MerkleTree::GetSize(target->left);
        int Rsize = MerkleTree::GetSize(target->right);

        // If left subtree has more nodes, insert on the right
        if (Lsize > Rsize) {
            return InsertNode(newnode, target->right);
        }

        // If the left subtree has the same number of nodes as the right or fewer, insert on the left
        else {
            return InsertNode(newnode, target->left);
        }
    }

    // False if insertion otherwise fails
    return false;
}



/*
 * Deletes the specified leaf from the tree, then updates the structure & hash values of the tree
 * @param The leaf node to be deleted. If an internal node is specified, return without modifying the tree, since only leaf nodes can be removed directly
 */
void MerkleTree::DeleteLeaf(shared_ptr<Node> deletion) {

    // If node is null, return
    if (!deletion) {return;}

    // Internal nodes can't be removed directly
    if (deletion->left != NULL) {return;}
    

    // If deletion is the only node in the tree, the tree becomes empty
    else if (deletion == root_ptr) {
        root_ptr = NULL;
    }

    else { // Node is a leaf

        shared_ptr<Node> sibling = GetSibling(deletion); // deletion's sibling will be moved up the tree
        shared_ptr<Node> oldParent = deletion->parent; // parent of deleted node will become obsolete & removed
        shared_ptr<Node> newParent = oldParent->parent; // This will be sibling's  new parent node

        if (oldParent == root_ptr) { // If the parent being deleted is the root, just replace it
            
            oldParent->left = NULL; // Nullify pointers of deleted nodes
            oldParent->right = NULL;
            
            deletion->parent = NULL;

            sibling->parent = NULL;
            SetRoot(sibling); // Remaining node is the root
            return;
        }


        // If the old parent node has a parent node of its own, sibling replaces the old parent in the tree
        string newPos = GetPosition(oldParent); // Figure out which child the sibling will become 
    

        // Remove selected leaf node & its parent
        oldParent->parent = NULL;
        oldParent->left = NULL;
        oldParent->right = NULL;
        deletion->parent = NULL;

        
        // Sibling leaf replaces its old parent in the tree
        sibling->parent = newParent;
        if (newPos == "Left") { // Update the appropriate child pointer for the new parent
            newParent->left = sibling;
        }

        else {
            newParent->right = sibling;
        }

        Resolve(sibling); // Update hashkeys for ancestors
    }

    return;
}



/*
 * Returns true if tree contains a leaf corresponding to the specified data
 * @param data the string data to be sought
 * @return true if the data is hashed in the tree
 */
bool MerkleTree::ContainsData(string data) {
    
    string key = sha256(data); // Hash the data

    // If tree is empty, it doesn't contain the data
    if (!root_ptr) {return false;}

    // If root is a leaf (N = 1), return true only if its hashkey matches the data
    else if (!(root_ptr->left)) {return (key == (root_ptr->hashkey) );}

    // Otherwise, recursively search the root's child branches for a matching hash
    else {
        if (ContainsData(data, root_ptr->left) || ContainsData(data, root_ptr->right)) {return true;}
        else {return false;}
    }

}



/* 
 * Recursive helper for ContainsData; narrows scope of the search by specifying the root of the subtree to be searched
 * Can be used independently 
 * @param data The data to be sought
 * @param subtree The root node of the subtree to be searched
 * @return true if the data is hashed in the specified subtree
 */
bool MerkleTree::ContainsData(string data, shared_ptr<Node> subtree) {

    string key = sha256(data); // Hash of the data

    // Null case
    if (!(subtree)) {return false;}

    // Base case: leaf node
    if (!(subtree->left)) {
        if (key == subtree->hashkey) {return true;} // Return true if the leaf's hashkey is the same as the hashed data
        else {return false;}                        // If no match, return false
    }

    // Recursive case: internal node
    else {
        if (ContainsData(data, subtree->left) || ContainsData(data, subtree->right)) {return true;}
        else {return false;}

    }
}



/*
 * Returns true if tree contains a leaf with the specified hash
 * @param hash The hashkey to be sought
 * @return true if the hashkey in the tree
 */
bool MerkleTree::ContainsHash(string hash){

    // If tree is empty, it doesn't contain the data
    if (!root_ptr) {return false;}

    // If root is a leaf (N = 1), return true only if its hashkey matches the argument
    else if (!(root_ptr->left)) {return (hash == (root_ptr->hashkey) );}

    // Otherwise, recursively search the root's child branches for a matching hash
    else {

        // Return true if either branch contains a matching leaf
        return ( ContainsHash(hash, root_ptr->left) || ContainsHash(hash, root_ptr->right) );
    }
}



/* 
 * Recursive helper for ContainsHash; narrows scope of the search by specifying the root of the subtree to be searched
 * Can be used independently 
 * @param hash The hashkey to be sought
 * @param subtree The root node of the subtree to be searched
 * @return true if the hash is in the specified subtree
 */
bool MerkleTree::ContainsHash(string hash, shared_ptr<Node> subtree) {
    
    // Null case
    if (!(subtree)) {return false;}

    // Base case: leaf node
    if (!(subtree->left)) {
        if (hash == subtree->hashkey) {return true;} // Return true if the leaf's hashkey is the same as the hash argument
        else {return false;}                        // If no match, return false
    }

    // Recursive case: internal node
    else {

        // Return true if either child branch contains the hashed data, false otherwise
        return ( ContainsHash(hash, subtree->left) || ContainsHash(hash, subtree->left)); 
    }
}



/*
 * Searches entire tree for--and, if found, returns--a node with the specified data
 * Recrusive step uses parameter specifying the root of the subtree--in this case, the main tree's root
 * @param data Data to be searched for in the tree
 * @returns Pointer to the leaf node containing the specified data, or null if no such node exists in the tree
 */
shared_ptr<Node> MerkleTree::FindData(string data){
    
    string key = sha256(data); // Hash the data

    // Null case: If tree is empty, the data is not hashed in the table
    if (!(root_ptr)) {return NULL;}


    // Base case: If N=1 (root is a leaf), return root_ptr if the root's hashkey is a match
    else if (!(root_ptr->left)) {
        shared_ptr<Node> match = (root_ptr->hashkey == key) ? root_ptr : NULL;
        return match;
    }

    // Recursive case: N > 1
    else {

        // Recursively search for a matching leaf
        shared_ptr<Node> matchLeft = FindData(data, root_ptr->left);
        shared_ptr<Node> matchRight = FindData(data, root_ptr->right);
        
        // Return NULL if neither branch contains a match
        if (!matchLeft && !matchRight) {return NULL;} 
        else if (matchLeft && !matchRight) {return matchLeft;} // If one branch contains a match, return that match
        else if (!matchLeft && matchRight) {return matchRight;}

        // If both branches contain a match, return the leftmost match
        else {return matchLeft;}
        
    }
}



/*
 * Recursively searches for and returns the first node in the subtree whose hashkey encodes the specified data
 * @param data The string whose hashkey is being searched for
 * @param subtree The subtree to search for the data's hash
 */
shared_ptr<Node> MerkleTree::FindData(string data, shared_ptr<Node> subtree){
    string key = sha256(data); // Hash the data

    // Null case: If subtree is empty, the data is not hashed in the table
    if (!(subtree)) {return NULL;}

    // Base case: If N=1 (root is a leaf), return subtree pointer if its hashkey is a match
    else if (!(subtree->left)) {
        shared_ptr<Node> match = (subtree->hashkey == key) ? subtree : NULL;
        return match;
    }

    // Recursive case: N > 1
    else {

        // Recursively search for a matching leaf
        shared_ptr<Node> matchLeft = FindData(data, subtree->left);
        shared_ptr<Node> matchRight = FindData(data, subtree->right);
        
        // Return NULL if neither branch contains a match
        if (!matchLeft && !matchRight) {return NULL;} 
        else if (matchLeft && !matchRight) {return matchLeft;} // If one branch contains a match, return that match
        else if (!matchLeft && matchRight) {return matchRight;}

        // If both branches contain a match, return the leftmost match
        else {return matchLeft;}
        
    }
}



/*
 * Searches entire tree, starting from root_ptr, for a leaf with a matching hashkey
 * @param hash The hashkey searched for
 * @return Pointer to the leftmost matching node, if it exists; nullptr if there is no match in the tree
 */
shared_ptr<Node> MerkleTree::FindHash(string hash) {
    
    // Null case: If tree is empty, the data is not hashed in the table
    if (!(root_ptr)) {return NULL;}


    // Base case: If N=1 (root is a leaf), return root_ptr if the root's hashkey is a match
    else if (!(root_ptr->left)) {
        shared_ptr<Node> match = (root_ptr->hashkey == hash) ? root_ptr : NULL;
        return match;
    }

    // Recursive case: N > 1
    else {

        // Recursively search for a matching leaf
        shared_ptr<Node> matchLeft = FindHash(hash, root_ptr->left);
        shared_ptr<Node> matchRight = FindHash(hash, root_ptr->right);
        
        // Return NULL if neither branch contains a match
        if (!matchLeft && !matchRight) {return NULL;} 
        else if (matchLeft && !matchRight) {return matchLeft;} // If one branch contains a match, return that match
        else if (!matchLeft && matchRight) {return matchRight;}

        // If both branches contain a match, return the leftmost match
        else {return matchLeft;}
        
    }
}



/*
 * Recursively search subtree, starting from given node for the leftmost node with a matching hashkey
 * @param hash The hashkey searched for
 * @param subtree The root node of the subtree to be searched
 * @return Pointer to leftmost match, or NULL if tree contains no matching nodes
 */
shared_ptr<Node> MerkleTree::FindHash(string hash, shared_ptr<Node> subtree) {
    // Null case: If tree is empty, the data is not hashed in the table
    if (!(subtree)) {return NULL;}


    // Base case: If N=1 (root is a leaf), return subtree ptr if the root's hashkey is a match
    else if (!(subtree->left)) {
        shared_ptr<Node> match = (subtree->hashkey == hash) ? subtree : NULL;
        return match;
    }

    // Recursive case: N > 1
    else {

        // Recursively search for a matching leaf
        shared_ptr<Node> matchLeft = FindHash(hash, subtree->left);
        shared_ptr<Node> matchRight = FindHash(hash, subtree->right);
        
        // Return NULL if neither branch contains a match
        if (!matchLeft && !matchRight) {return NULL;} 
        else if (matchLeft && !matchRight) {return matchLeft;} // If one branch contains a match, return that match
        else if (!matchLeft && matchRight) {return matchRight;}

        // If both branches contain a match, return the leftmost match
        else {return matchLeft;}
        
    }
}



/*
 * Updates hashkeys for all ancestors of a modified leaf node
 * @param mod The modified leaf node defining the update
 */
void MerkleTree::Resolve(shared_ptr<Node> mod){

    shared_ptr<Node> ancestor = mod->parent; // Initialize an iterator for current & ancestor nodes
    shared_ptr<Node> current = mod;
    while (ancestor) { // For all ancestors of the modified leaf

        string SibHash = GetSibling(current)->hashkey; // Get hash value of current node's sibling
        string concat;

        // Concatenate in whatever order the nodes appear left to right
        if (GetPosition(current) == "Left") {
            concat = (current->hashkey) + (SibHash); // Concatenate current and sibling hashkeys
        }

        else {
            concat = SibHash + (current->hashkey); // Concatenate current and sibling hashkeys
        }
        

        ancestor->hashkey = sha256(concat); // Update the hash of the parent node

        current = ancestor;
        ancestor = ancestor->parent;
    }

    return;
}



/*
 * Checks for changes in the current tree's data in the specified copy of the tree 
 * @param copy The copy of the current Merkle Tree to be checked
 * @return true if the copy has not been modified; false if any data was altered
 */
bool MerkleTree::Verify(MerkleTree copy){
    if (root_ptr->hashkey != copy.GetRoot()->hashkey) {return false;} // If the root hashkeys differ, then at least one data element was 
                                                    // modified before its node was inserted into the copy

    // Identical root hashes mean identical Merkle Trees
    return true; 
}       



/* 
 * Confirm a main tree's data modifications by updating the source tree
 * @param copy The tree whose changes are to be accepted
 */
void MerkleTree::AcceptMods(MerkleTree copy){
    
    // If trees are the same, there's nothing to be done
    if (Verify(copy)) {return;}

    else {
        
        shared_ptr<Node> sourceNode = root_ptr; // Mark nodes where update is to start
        shared_ptr<Node> copyNode = copy.GetRoot();

        return AcceptMods(copy, sourceNode, copyNode); // Update the trees

    }
    
}



/*
 * Recursive helper for AcceptMods; updates the source tree (starting from sourceNode) to match the hashed data in the copy tree
 * @param copy The copy tree whose changes are being accepted
 * @param sourceNode The current node in the source tree
 * @param copyNode The corresponding node in the copy tree
 */
void MerkleTree::AcceptMods(MerkleTree copy, shared_ptr<Node> sourceNode, shared_ptr<Node> copyNode) {


    
    // Null case: this branch does not contain any discrepancies (nothing to change)
    if (sourceNode->hashkey == copyNode->hashkey) {return;}



    // Base case 1: leaves are reached
    if (!(sourceNode->left) && !(copyNode->left)) {

        
        // If hashkeys differ, update the hash in the source tree, as well as the hashes of its ancestors
        if (sourceNode->hashkey != copyNode->hashkey) {
            
            sourceNode->hashkey = copyNode->hashkey;
            Resolve(sourceNode);
        }
        return;
    }


    // Base case 2: tree structure differs (one node is a leaf and the other is not)
    if (IsLeaf(sourceNode) != (copy.IsLeaf(copyNode))) {

        if (!(IsLeaf(sourceNode))) { // If the node in the source tree has children, remove its children
            sourceNode->left = NULL;
            sourceNode->right = NULL;
        }

        else { // If the node source tree does not have the same child branches, add copies of those branches
            sourceNode->left = copy.CopyBranch(copyNode->left);
            sourceNode->left->parent = sourceNode;

            sourceNode->right = copy.CopyBranch(copyNode->right);
            sourceNode->right->parent = sourceNode;
        }

        sourceNode->hashkey = copyNode->hashkey;  // Copy data over

        Resolve(sourceNode); // Update ancestral hashes in source tree

        return;
    }

    // Recursive case: the selected branch in the two trees contains at least one discrepancy
    else {

        
            
        // Look at the top of the child branches in each tree
        shared_ptr<Node> sourceLeft = sourceNode->left;
        shared_ptr<Node> sourceRight = sourceNode->right;

        shared_ptr<Node> copyLeft = copyNode->left;
        shared_ptr<Node> copyRight = copyNode->right;


        // If there is a discrepancy between the left branches, look for it and update the source tree
        if (sourceLeft->hashkey != copyLeft->hashkey) {
            AcceptMods(copy, sourceLeft, copyLeft);
        }

        // If discrepancy on the right
        if (sourceRight->hashkey != copyRight->hashkey) {
            AcceptMods(copy, sourceRight, copyRight);
        }

        return;

        }
    



}



/*
 * Updates the data/hashkey of a specified leaf node and updates the hash values of its ancestors
 * Actually replaces the old node with a new node, one with an updated hash
 * @param leafnode The leaf node whose data/hash key is to be updated
 * @param newdata The updated string data for the leaf node
 */
void MerkleTree::ModifyLeaf(shared_ptr<Node> target, string newdata){
    
    // Do nothing for internal nodes or nodes without parents
    if (target->left != NULL) {return;}

    else {
        shared_ptr<Node> newnode = InitLeaf(newdata); // Create replacement node

        // Update parent when target has a parent
        if (target->parent != NULL) {
            
            newnode->parent = target->parent;
            
            // Determine which of the parents children should be replaced
            string which_child = GetPosition(target);

            if (which_child == "Left") {newnode->parent->left = newnode;}
            else if (which_child == "Right") {newnode->parent->right = newnode;}
            else {return;}

            Resolve(newnode); // Update ancestral hashes
        }


    }
    

    return;


}



/*
 * Returns pointer to the sibling of the specified node, or a nullpointer if the specified node is null or the tree root
 * @param node Node whose sibling is to be retrieved
 * @return Pointer to the sibling node, if it exists
 */
shared_ptr<Node> MerkleTree::GetSibling(shared_ptr<Node> node) {
    
    // If parameter node is null or the root, it has no sibling
    if (!node || (node == root_ptr)) {return NULL;}

    else {
        if (node->parent->left == node) {return node->parent->right;} // If current node is the left child of its parent, the sibling is its parent's right child
        else {return node->parent->left;} // Otherwise, the sibling is the parent's left child
    }
}



/*
 * Finds the position of a node relative to its parent
 * @param node The node whose position is to be checked
 * @return A string; "Left" if node is a left child, "Right" if a right child, and "None" if the node is null or a root node
 */
string MerkleTree::GetPosition(shared_ptr<Node> node) {

    // No node / no parent
    if (!node || !(node->parent)) {return "None";}

    // Left child 
    else if (node == node->parent->left) {return "Left";}

    // Right child
    else {return "Right";}

}



/*
 * Checks whether given node is a leaf node
 * @param node The node to be checked
 * @return True if node is a leaf, false if node is an internal node or is NULL
 */
bool MerkleTree::IsLeaf(shared_ptr<Node> node) {

    if (!node) {return false;} // NULL

    else if (!(node->left)) {return true;} // Node is a leaf

    else {return false;} // Node is an internal node
}



/*
 * Recursively create a copy of a branch and return a pointer to the copy
 * @param branch Pointer to the top of the branch to be copied
 * @return Pointer to the branch copy; data hashes in the copy are identical to the original, but the nodes do not point to the original branch
 */
shared_ptr<Node> MerkleTree::CopyBranch(shared_ptr<Node> branch) {

    // Base case: branch is a leaf
    if (IsLeaf(branch)) {

        shared_ptr<Node> leafcopy(new Node); // Allocate new node pointer
        
        leafcopy->hashkey = branch->hashkey; // Copy data hash
        leafcopy->left = NULL;
        leafcopy->right = NULL;
        leafcopy->parent = NULL;

        return leafcopy;
    }


    else {

        shared_ptr<Node> branchcopy(new Node); // Allocate new treenode pointer

        branchcopy->hashkey = branch->hashkey; // Copy its data 

        branchcopy->left = CopyBranch(branch->left); // Recursively copy children 
        branchcopy->right = CopyBranch(branch->right);

        branchcopy->left->parent = branchcopy; // Set childrens' parent
        branchcopy->right->parent = branchcopy;

        branchcopy->parent = NULL; // Leave parent unspecified so the copy of the branch can be inserted elsewhere

        return branchcopy;

    }
}