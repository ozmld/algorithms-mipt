#include <iostream>
#include <vector>

using namespace std;

const long long kMaxNumber = 1e6 + 1;

struct Node {
  long long element;
  int insert_operation, element_heap_index;
  Node() {
    element = 0;
    insert_operation = 0;
    element_heap_index = 0;
  }
};

class Heap {
 private:
  Node heap_[kMaxNumber];
  // We also will save number of operation (when element was added to the heap)
  // and also will save element by operation number. To have accordance
  // element <-> number of operation
  long long heap_size_;

 public:
  Heap() {
    heap_size_ = 0;
    for (long long i = 0; i < kMaxNumber; ++i) {
      heap_[i] = Node();
    }
  }
  void Exchange(int i, int j);
  void SiftDown(int i);
  void SiftUp(int i);
  void Insert(long long value, int operation_number);
  long long GetMin() const;
  void ExtractMin();
  void DecreaseKey(int operation_number, long long delta);
};

void Heap::Exchange(int i, int j) {
  int insert_operation_1 = heap_[i].insert_operation,
      insert_operation_2 = heap_[j].insert_operation;
  // We exchanging i-element and j-element and all data
  // which respond to them
  swap(heap_[i].element, heap_[j].element);
  swap(heap_[i].insert_operation, heap_[j].insert_operation);
  swap(heap_[insert_operation_1].element_heap_index,
       heap_[insert_operation_2].element_heap_index);
}

void Heap::SiftDown(int i) {
  long long left_child_index, right_child_index, min_index;
  while (2 * i + 1 < heap_size_) {
    left_child_index = (i << 1) + 1;
    right_child_index = (i << 1) + 2;
    min_index = i;
    // Usual sift down
    if (heap_[left_child_index].element < heap_[min_index].element) {
      min_index = left_child_index;
    }
    if (right_child_index < heap_size_ &&
        heap_[right_child_index].element < heap_[min_index].element) {
      min_index = right_child_index;
    }
    if (min_index == i) {
      break;
    }
    Exchange(i, min_index);
    i = min_index;
  }
}

void Heap::SiftUp(int i) {
  // Usual sift up
  long long parent_index = ((i - 1) >> 1);
  while (i > 0 && heap_[i].element < heap_[parent_index].element) {
    Exchange(i, parent_index);
    i = parent_index;
    parent_index = ((i - 1) >> 1);
  }
}

void Heap::Insert(long long value, int operation_number) {
  // Inserting element and initializing all appropriate data
  heap_[heap_size_].element = value;
  heap_size_++;
  heap_[heap_size_ - 1].insert_operation = operation_number;
  heap_[operation_number].element_heap_index = heap_size_ - 1;
  SiftUp(heap_size_ - 1);
}

long long Heap::GetMin() const { return heap_[0].element; }

void Heap::ExtractMin() {
  Exchange(0, heap_size_ - 1);
  heap_size_--;
  SiftDown(0);
}

void Heap::DecreaseKey(int operation_number, long long delta) {
  int element_index = heap_[operation_number].element_heap_index;
  // Decreasing element and updating the heap
  heap_[element_index].element -= delta;
  SiftUp(element_index);
}

void ProceedOperation(string operation, Heap& heap, int operation_number) {
  if (operation == "insert") {
    long long x;
    cin >> x;
    heap.Insert(x, operation_number);
  }
  if (operation == "getMin") {
    cout << heap.GetMin() << "\n";
  }
  if (operation == "extractMin") {
    heap.ExtractMin();
  }
  if (operation == "decreaseKey") {
    long long operation_number, delta;
    cin >> operation_number >> delta;
    heap.DecreaseKey(operation_number - 1, delta);
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);
  int q;
  string operation;
  cin >> q;
  Heap heap;
  for (int i = 0; i < q; ++i) {
    cin >> operation;
    ProceedOperation(operation, heap, i);
  }
}
