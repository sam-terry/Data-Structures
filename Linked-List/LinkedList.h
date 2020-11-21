#ifndef LINKEDLIST_H__
#define LINKEDLIST_H__

#include <string>
#include <memory>

struct node {
  int data;               // data: the value of this node
  std::shared_ptr<node> next;  // next: pointer to the next node in the list, or NULL if this is the last node.
};

/*
  Following the 'next' link for each element in the linked list will always
  eventually lead to a null pointer; no circular reference are allowed
*/

class LinkedList {
  public:
    
    LinkedList(); // Constructor; create pointer to top of list & initialize to NULL

    ~LinkedList(); // Deconstructor; shared pointers make this implementation unecessary 

    // Creates new node object from heap memory
    // Node contains the specified data and initially points to NULL
    std::shared_ptr<node> InitNode(int data);

    // Returns report (a string) of the list's data, separated by spaces and starting from the top
    // Data is reported in the same order in which the corresponding nodes occur in list
    std::string Report();

    // Creates node object for the data and inserts it at the end of the list
    void AppendData(int data);

    // Insert a node object at the end of the linked list
    void Append(std::shared_ptr<node> new_node);

    // Creates node object for the data, then inserts it at the specified offset
    // Does nothing if offset is invalid, i.e. negative or greater than list length
    void InsertData(int offset, int data);

    // Inserts a node object into the linked list at the specified offset
    // Does nothing if offset is invalid
    void Insert(int offset, std::shared_ptr<node> new_node);

    // Removes the node at the specified integer offset from the list
    // Does nothing if offset is invalid
    void Remove(int offset);

    // Returns number of nodes in the linked list (excluding the final NULL link)
    int Size();

    // Returns true if the linked list contains the specified data
    bool Contains(int data);

    // Returns the top pointer
    std::shared_ptr<node> GetTop();

    // Sets a given pointer as the top pointer
    void SetTop(std::shared_ptr<node> top_ptr);


  private:
    std::shared_ptr<node> top_ptr_;

  };

#endif  // LINKEDLIST_H__