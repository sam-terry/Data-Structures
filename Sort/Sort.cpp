#include "Sort.h"


// Constructor
Sorting::Sorting(){
  
}

// Deconstructor
Sorting::~Sorting(){
}

void Sorting::quicksort(std::vector<int>& data, int low_idx, int high_idx){
  if (low_idx < high_idx) {
    int p = quicksort_partition(data, low_idx, high_idx);
    quicksort(data, low_idx, p);
    quicksort(data, p+1, high_idx);
  }
  
}

// Partioning algorithm for quicksort
int Sorting::quicksort_partition(std::vector<int>& data, int low_idx, int high_idx){
  int midpoint = low_idx + ((high_idx - low_idx) / 2);
  int pivot = data[midpoint];
  int l = low_idx;
  int h = high_idx;
  bool done = false;

  while (!done) {
    while (data[l] < pivot) {
      l++;
    }

    while (data[h] > pivot) {
      h--;
    }

    if (l >= h) {
      done = true;
    }

    else {
      int temp = data[l];
      data[l] = data[h];
      data[h] = temp;

      l++;
      h--;
    }

  }

  return h;
}

// Bubblesort implementation; runs in O(n^2)
void Sorting::bubblesort(std::vector<int>& data){
  int size = data.size();
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (data[j] > data[j+1]) {
        swap(data,j,j+1);
      }
    }
  }
}

// Mergesort 
void Sorting::mergesort(std::vector<int>& data){
  std::vector<int> result; // Create empty result vector
  int dataSize = data.size(); // Determine vector size
  int midpoint = (dataSize - 1) / 2; // Determine midpoint

  std::vector<int> left; // Create empty vectors for the left and right sides of the midpoint
  std::vector<int> right;

  if (dataSize > 1) { // If there is more than one item in the vector
    for (int i = 0; i < midpoint + 1; i++) { // Fill in left vector
      left.push_back(data[i]);
    }

    for (int j = midpoint + 1; j < data.size(); j++) { // Fill in right vector
      right.push_back(data[j]);
    }

    mergesort(left); // Recursively sort left and right vectors
    mergesort(right);
    
    merge(left, right, result); // Merge sorted left and right vectors
    
  }
  
  for (int idx = 0; idx < result.size(); idx++) { // Copy sorted result back into input vector
    data[idx] = result[idx];
  }
  
}

/*
* Helper function for mergesort
* @param left Left vector, sorted in recursive call to merge sort
* @param right Right vector, sorted in recursive call to merge sort
* @param result Empty vector, where merged halves are stored
* Returns nothing; alters the empty result vector in place
*/
void Sorting::merge(std::vector<int>& left, std::vector<int>& right, std::vector<int>& result){
  int leftSize = left.size();
  int rightSize = right.size();
  int i = 0;
  int j = 0;

  while (i < leftSize && j < rightSize) { // Increment both indices; add smaller element to result
    if (left[i] < right[j]) {
      result.push_back(left[i]);
      i++;
    }

    else {
      result.push_back(right[j]);
      j++;
    }
  }

  while (i < leftSize) { // Add remaining elements in left vector
    result.push_back(left[i]);
    i++;
  }

  while (j < rightSize) { // Add remaining elements in right vector
    result.push_back(right[j]);
    j++;
  }


}

/*
* Helper function for bubble sort
* @param data Vector to be sorted, passed by reference
* @param left_idx first item to swap
* @param right_idx second item to swap
* returns nothing; alters vector in place
*/
void Sorting::swap(std::vector<int>& data, int left_idx, int right_idx) {
  int temp = data[left_idx];
  data[left_idx] = data[right_idx];
  data[right_idx] = temp;
}

/*
* Implementation of insertion sort
* @param data The vector to be sorted
* Sorts list in place (in O(n^2) time); returns nothing
*/
void Sorting::insertionsort(std::vector<int>& data){
  int dataSize = data.size(); // Determine size of vector
  for (int i = 1; i < dataSize; i++) { // For the second item in the list to the last item

    int current = data[i]; // Mark the current element and its index
    int position = i;

    while (position > 0 && data[position - 1] > current) { // Until position is 0 or the previous item is <= the current element
      data[position] = data[position - 1]; // Move larger element over
      position--; // Decrement index
    }

    data[position] = current; // Move original element being compared
  }
  
}