#include <iostream>
#include <vector>

using namespace std;

const long long kMaxNumber = 1e6 + 1;

class Heap {
 private:
  vector<long long> dataset_;
  long long insert_operation_[kMaxNumber], element_heap_index_[kMaxNumber];
  long long heap_size_;

 public:
  Heap() {
    heap_size_ = 0;
    dataset_ = {};
    for (long long i = 0; i < kMaxNumber; ++i) {
      insert_operation_[i] = 0;
      element_heap_index_[i] = 0;
    }
  }
  void Exchange(long long i, long long j);
  void SiftDown(long long i);
  void SiftUp(long long i);
  void Insert(long long value, long long operation_number);
  long long GetMin();
  void ExtractMin();
  void DecreaseKey(long long operation_number, long long delta);
};

void Heap::Exchange(long long i, long long j) {
  long long insert_operation_1 = insert_operation_[i],
            insert_operation_2 = insert_operation_[j];
  swap(dataset_[i], dataset_[j]);
  swap(insert_operation_[i], insert_operation_[j]);
  swap(element_heap_index_[insert_operation_1],
       element_heap_index_[insert_operation_2]);
}

void Heap::SiftDown(long long i) {
  long long left_child_index, right_child_index, min_index;
  while (2 * i + 1 < heap_size_) {
    left_child_index = (i << 1) + 1;
    right_child_index = (i << 1) + 2;
    min_index = i;
    if (dataset_[left_child_index] < dataset_[min_index]) {
      min_index = left_child_index;
    }
    if (right_child_index < heap_size_ &&
        dataset_[right_child_index] < dataset_[min_index]) {
      min_index = right_child_index;
    }
    if (min_index == i) {
      break;
    }
    Exchange(i, min_index);
    i = min_index;
  }
}

void Heap::SiftUp(long long i) {
  long long parent_index = ((i - 1) >> 1);
  while (i > 0 && dataset_[i] < dataset_[parent_index]) {
    Exchange(i, parent_index);
    i = parent_index;
    parent_index = ((i - 1) >> 1);
  }
}

void Heap::Insert(long long value, long long operation_number) {
  heap_size_++;
  dataset_.push_back(value);
  insert_operation_[heap_size_ - 1] = operation_number;
  element_heap_index_[operation_number] = heap_size_ - 1;
  SiftUp(heap_size_ - 1);
}

long long Heap::GetMin() { return dataset_[0]; }

void Heap::ExtractMin() {
  Exchange(0, heap_size_ - 1);
  dataset_.pop_back();
  heap_size_--;
  SiftDown(0);
}

void Heap::DecreaseKey(long long operation_number, long long delta) {
  long long element_index = element_heap_index_[operation_number];
  dataset_[element_index] -= delta;
  SiftUp(element_index);
}

void ProceedOperation(string operation, Heap& heap,
                      long long operation_number) {
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
  long long q;
  string operation;
  cin >> q;
  Heap heap;
  for (long long i = 0; i < q; ++i) {
    cin >> operation;
    ProceedOperation(operation, heap, i);
  }
}
