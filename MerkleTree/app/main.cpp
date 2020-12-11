#include "../code/Merkle.h"
#include "../code/sha256.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(){
    

        
	MerkleTree mytree;

    shared_ptr<Node> newnode = mytree.InitLeaf("test");
    mytree.InsertNode(newnode);

    if (mytree.IsLeaf(mytree.GetRoot())) {
        cout << "node is a leaf" << endl;
    }
        
    return 0;
}