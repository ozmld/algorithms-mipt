#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

class MinMaxHeap {
 public:
  vector<long long> dataset;
  long long heap_size;
  MinMaxHeap() {
    heap_size = 0;
    dataset = {};
  }
  long long CheckChildsMin(long long i, long long mi);
  long long CheckGrandChildsMin(long long i, long long mi, long long& parent,
                                long long& is_grandchild);
  void SiftDownMin(long long i);
  long long CheckChildsMax(long long i, long long mi);
  long long CheckGrandChildsMax(long long i, long long mi, long long& parent,
                                long long& is_grandchild);
  void SiftDownMax(long long i);
  void SiftDown(long long i);
  void SiftUpMin(long long i);
  void SiftUpMax(long long i);
  void SiftUp(long long i);
  void Insert(long long value);
  void GetMin();
  void ExtractMin();
  void GetMax();
  void ExtractMax();
  void Size();
  void Clear();
};

long long NodeLevel(long long i) { return int(log2(i + 1)); }

long long MinMaxHeap::CheckChildsMin(long long i, long long mi) {
  long long left_child_index = (i << 1) + 1;
  long long right_child_index = (i << 1) + 2;
  long long min_index = mi;
  if (left_child_index < heap_size &&
      dataset[left_child_index] < dataset[min_index]) {
    min_index = left_child_index;
  }
  if (right_child_index < heap_size &&
      dataset[right_child_index] < dataset[min_index]) {
    min_index = right_child_index;
  }
  return min_index;
}

long long MinMaxHeap::CheckGrandChildsMin(long long i, long long mi,
                                          long long& parent,
                                          long long& is_grandchild) {
  long long min_index = mi;
  if (min_index != CheckChildsMin((i << 1) + 1, min_index)) {
    min_index = CheckChildsMin((i << 1) + 1, min_index);
    is_grandchild = 1;
    parent = (i << 1) + 1;
  }
  if (min_index != CheckChildsMin((i << 1) + 2, min_index)) {
    min_index = CheckChildsMin((i << 1) + 2, min_index);
    is_grandchild = 1;
    parent = (i << 1) + 2;
  }
  return min_index;
}

void MinMaxHeap::SiftDownMin(long long i) {
  long long is_grandchild = 0, parent;
  while ((i << 1) + 1 < heap_size) {
    long long min_index = i;
    min_index = CheckChildsMin(i, min_index);
    min_index = CheckGrandChildsMin(i, min_index, parent, is_grandchild);
    if (dataset[min_index] < dataset[i]) {
      swap(dataset[min_index], dataset[i]);
      if (is_grandchild != 0) {
        if (dataset[min_index] > dataset[parent]) {
          swap(dataset[min_index], dataset[parent]);
        }
      }
    } else {
      break;
    }
    i = min_index;
  }
}

long long MinMaxHeap::CheckChildsMax(long long i, long long mi) {
  long long max_index = mi;
  long long left_child_index = (i << 1) + 1;
  long long right_child_index = (i << 1) + 2;
  if (left_child_index < heap_size &&
      dataset[left_child_index] > dataset[max_index]) {
    max_index = left_child_index;
  }
  if (right_child_index < heap_size &&
      dataset[right_child_index] > dataset[max_index]) {
    max_index = right_child_index;
  }
  return max_index;
}

long long MinMaxHeap::CheckGrandChildsMax(long long i, long long mi,
                                          long long& parent,
                                          long long& is_grandchild) {
  long long max_index = mi;
  if (max_index != CheckChildsMax((i << 1) + 1, max_index)) {
    max_index = CheckChildsMax((i << 1) + 1, max_index);
    is_grandchild = 1;
    parent = (i << 1) + 1;
  }
  if (max_index != CheckChildsMax((i << 1) + 2, max_index)) {
    max_index = CheckChildsMax((i << 1) + 2, max_index);
    is_grandchild = 1;
    parent = (i << 1) + 2;
  }
  return max_index;
}

void MinMaxHeap::SiftDownMax(long long i) {
  long long is_grandchild = 0, parent;
  while (2 * i + 1 < heap_size) {
    long long max_index = i;
    if (dataset[(i << 1) + 1] < dataset[max_index]) {
      max_index = (i << 1) + 1;
    }
    max_index = CheckChildsMax(i, max_index);
    max_index = CheckGrandChildsMax(i, max_index, parent, is_grandchild);
    if (dataset[max_index] > dataset[i]) {
      swap(dataset[max_index], dataset[i]);
      if (is_grandchild != 0) {
        if (dataset[max_index] < dataset[parent]) {
          swap(dataset[max_index], dataset[parent]);
        }
      }
    } else {
      break;
    }
    i = max_index;
  }
}

void MinMaxHeap::SiftDown(long long i) {
  if (NodeLevel(i) % 2 == 0) {
    SiftDownMin(i);
  } else {
    SiftDownMax(i);
  }
}

void MinMaxHeap::SiftUpMin(long long i) {
  long long grandparent_index = ((i - 3) >> 2);
  while (i > 2 && dataset[i] < dataset[grandparent_index]) {
    swap(dataset[i], dataset[grandparent_index]);
    i = grandparent_index;
    grandparent_index = ((i - 3) >> 2);
  }
}

void MinMaxHeap::SiftUpMax(long long i) {
  long long grandparent_index = ((i - 3) >> 2);
  while (i > 2 && dataset[i] > dataset[grandparent_index]) {
    swap(dataset[i], dataset[grandparent_index]);
    i = grandparent_index;
    grandparent_index = ((i - 3) >> 2);
  }
}

void MinMaxHeap::SiftUp(long long i) {
  if (i == 0) {
    return;
  }
  long long parent_index = ((i - 1) >> 1);
  if (NodeLevel(i) % 2 == 0) {
    if (dataset[i] > dataset[parent_index]) {
      swap(dataset[i], dataset[parent_index]);
      SiftUpMax(parent_index);
    } else {
      SiftUpMin(i);
    }
    return;
  }
  if (dataset[i] < dataset[parent_index]) {
    swap(dataset[i], dataset[parent_index]);
    SiftUpMin(parent_index);
  } else {
    SiftUpMax(i);
  }
}

void MinMaxHeap::Insert(long long value) {
  heap_size++;
  dataset.push_back(value);
  cout << "ok\n";
  SiftUp(heap_size - 1);
}

void MinMaxHeap::GetMin() {
  if (heap_size > 0) {
    cout << dataset[0] << "\n";
  } else {
    cout << "error\n";
  }
}

void MinMaxHeap::ExtractMin() {
  if (heap_size == 0) {
    cout << "error\n";
    return;
  }
  cout << dataset[0] << "\n";
  if (heap_size == 1) {
    heap_size = 0;
    dataset = {};
    return;
  }
  swap(dataset[0], dataset[heap_size - 1]);
  dataset.pop_back();
  heap_size--;
  SiftDown(0);
}

void MinMaxHeap::GetMax() {
  if (heap_size > 1) {
    if (heap_size > 2) {
      cout << max(dataset[1], dataset[2]) << "\n";
    } else {
      cout << dataset[1] << "\n";
    }
  } else {
    if (heap_size > 0) {
      cout << dataset[0] << "\n";
    } else {
      cout << "error\n";
    }
  }
}

void MinMaxHeap::ExtractMax() {
  if (heap_size == 0) {
    cout << "error\n";
    return;
  }
  if (heap_size > 1) {
    if (heap_size > 2) {
      cout << max(dataset[1], dataset[2]) << "\n";
      if (dataset[2] > dataset[1]) {
        swap(dataset[2], dataset[heap_size - 1]);
        dataset.pop_back();
        heap_size--;
        SiftDown(2);
      } else {
        swap(dataset[1], dataset[heap_size - 1]);
        dataset.pop_back();
        heap_size--;
        SiftDown(1);
      }
    } else {
      cout << dataset[1] << "\n";
      dataset.pop_back();
      heap_size = 1;
    }
  } else {
    cout << dataset[0] << "\n";
    dataset = {};
    heap_size = 0;
  }
}

void MinMaxHeap::Size() { cout << heap_size << "\n"; }

void MinMaxHeap::Clear() {
  dataset = {};
  heap_size = 0;
  cout << "ok\n";
}

void ProceedOperation(string operation, MinMaxHeap& heap) {
  if (operation == "insert") {
    long long x;
    cin >> x;
    heap.Insert(x);
  }
  if (operation == "get_min") {
    heap.GetMin();
  }
  if (operation == "extract_min") {
    heap.ExtractMin();
  }
  if (operation == "get_max") {
    heap.GetMax();
  }
  if (operation == "extract_max") {
    heap.ExtractMax();
  }
  if (operation == "size") {
    heap.Size();
  }
  if (operation == "clear") {
    heap.Clear();
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);
  long long q;
  string operation;
  cin >> q;
  MinMaxHeap heap = MinMaxHeap();
  for (long long i = 0; i < q; ++i) {
    cin >> operation;
    ProceedOperation(operation, heap);
  }
}
