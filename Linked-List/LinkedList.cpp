#include "LinkedList.h"

// List constructor
// Creates a pointer (initially null) to the top of the list 
LinkedList::LinkedList(){
  top_ptr_ = nullptr;
}

// Deconstructor (not implemented)
LinkedList::~LinkedList(){
}

std::shared_ptr<node> LinkedList::InitNode(int data){
  std::shared_ptr<node> ret(new node);
  ret->data = data;
  ret->next = nullptr;
  return ret;
}

std::string LinkedList::Report(){
   std::string ret;
   std::shared_ptr<node> current = top_ptr_;
   while (current) {
     ret += std::to_string(current->data);
     ret += " ";
     current = current->next;
   }
  return ret;
}

void LinkedList::AppendData(int data){
  std::shared_ptr<node> newTail(new node);
  newTail->data = data;
  newTail->next = nullptr;

  if (top_ptr_ == NULL) {
    top_ptr_ = newTail;
  }

  else {
    std::shared_ptr<node> current = top_ptr_;
    std::shared_ptr<node> next = current->next;
    while (next) {
      current = next;
      next = current->next;
    }
    current->next = newTail;
  }

}

void LinkedList::Append(std::shared_ptr<node> new_node){
  if (top_ptr_ == NULL) {
    top_ptr_ = new_node;
  }

  else {
    std::shared_ptr<node> current = top_ptr_;
    std::shared_ptr<node> next = current->next;
    while (next) {
      current = next;
      next = current->next;
    }
    current->next = new_node;
  }
  
}

void LinkedList::InsertData(int offset, int data){
  std::shared_ptr<node> newNode(new node);
  newNode->data = data;
  newNode->next = nullptr;

  if (offset == 0) {
    newNode->next = top_ptr_;
    top_ptr_ = newNode;
  }

  else {
    std::shared_ptr<node> currentnode = top_ptr_;
    std::shared_ptr<node> nextnode = currentnode->next;
    int current_idx = 1;
    while (current_idx < offset) {
      currentnode = nextnode;
      nextnode = nextnode->next;
      current_idx++;
    }
    newNode->next = nextnode;
    currentnode->next = newNode;
  }
}

void LinkedList::Insert(int offset, std::shared_ptr<node> new_node){
  if (offset == 0) {
    new_node->next = top_ptr_;
    top_ptr_ = new_node;
  }

  else {
    std::shared_ptr<node> currentnode = top_ptr_;
    std::shared_ptr<node> nextnode = currentnode->next;
    int curr_idx = 1;
    while (curr_idx < offset ) {
      currentnode = nextnode;
      nextnode = nextnode->next;
      curr_idx++;
    }
    new_node->next = nextnode;
    currentnode->next = new_node;
  }
}

void LinkedList::Remove(int offset){
  if (offset == 0) {
    top_ptr_ = top_ptr_->next;
  }

  else if ((offset > 0) && (offset < this->Size())) {  
    std::shared_ptr<node> currentnode = top_ptr_;
    std::shared_ptr<node> nextnode = currentnode->next;
    int curr_idx = 1;
    while (curr_idx < offset ) {
      currentnode = nextnode;
      nextnode = nextnode->next;
      curr_idx++;
    }
    currentnode->next = nextnode->next;
  }
}

int LinkedList::Size(){
  int ret = 0;
  if (top_ptr_ != NULL) {
    std::shared_ptr<node> current = top_ptr_;
    while (current != NULL) {
      current = current->next;
      ret++;
    }
  }
  return ret;
}

bool LinkedList::Contains(int data){
  bool ret = false;
  if (top_ptr_ != NULL) {
    std::shared_ptr<node> current = top_ptr_;
    while (current != NULL) {
      if (current->data == data) {
        ret = true;
      }
      current = current->next;
    }
  }
  return ret;
}

// Returns the top pointer
std::shared_ptr<node> LinkedList::GetTop(){
  return top_ptr_;
}

// Sets the pointer to the top of the list to the given pointer
void LinkedList::SetTop(std::shared_ptr<node> top_ptr){
  top_ptr_ = top_ptr;
}
