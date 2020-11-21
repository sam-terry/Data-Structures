#ifndef SORT_H__
#define SORT_H__

#include <vector>

class Sorting {
public:
  // Constructor
  Sorting();

  // Deconstructor
  ~Sorting();
  
  /*
    Implementation of Quicksort, a recursive sorting algorithm
    Sorts a vector of integers in-place; returns nothing
    @param data -- a vector of integers to be sorted (passed by reference)
    @param low_idx -- algorithm's current low index 
    @param low_idx -- algorithm's current high index 
  */
  void quicksort(std::vector<int>& data, int low_idx, int high_idx);
  
  /*
    Quicksort partitioning algorithm  
    @return pivot -- returns index of pivot element
  */
  int quicksort_partition(std::vector<int>& data, int low_idx, int high_idx);

  /*
    Implementation of the (relatively inefficient) Bubblesort algorithm
    @param data -- vector of integers (passed by reference) to be sorted in-place
    @return N/A
  */
  void bubblesort(std::vector<int>& data);

  /*
    Mergesort implementation; makes use of merge()
    @param data -- vector of ints, passed by reference, to be sorted in-place
  */
  void mergesort(std::vector<int>& data);

  // Insertion sort implementation; 'data' parameter is the same as above
  void insertionsort(std::vector<int>& data);
  
private:
  /*
    Helper function for mergesort; combines the two vectors produced by mergesort's recursive step
    @params left, right -- integer vectors to be merged
    @param result -- stores the merger of the left & right vectors; this result will be sorted in ascending order
    @return Nothing
  */
  void merge(std::vector<int>& left, std::vector<int>& right, std::vector<int>& result);

  /*
    Helper function for bubblesort
    @params left, right -- integer indices of the elements to be swapped
    @data -- vector of ints, containing the elements to be swapped
  */
  void swap(std::vector<int>& data, int left, int right);

};

#endif  // SORT_H__