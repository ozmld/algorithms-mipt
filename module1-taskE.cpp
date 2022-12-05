#include <iostream>
#include <string>

const int kMaxNumber = 2 * 1e5, kMaxLevel = 30;

void SparseTableInitialization(int number, int (&dataset)[kMaxNumber],
                               int (&sparse_table)[kMaxLevel][kMaxNumber]) {
  // Initialization sparse table structure.
  for (int i = 0; i < number; ++i) {
    sparse_table[0][i] = dataset[i];
  }
  for (int level = 1; level < kMaxLevel; ++level) {
    for (int i = 0; i < number - (1 << level) + 1; ++i) {
      sparse_table[level][i] =
          std::min(sparse_table[level - 1][i],
                   sparse_table[level - 1][i + (1 << (level - 1))]);
    }
  }
}

int Log(int x) {
  if (x == 1) {
    return 0;
  }
  return Log(x / 2) + 1;
}

int Minimum(int l, int r, int (&sparse_table)[kMaxLevel][kMaxNumber]) {
  int level = Log(r - l + 1);
  // Minimum on [l, r] segment is evaluated in SparseTable structure by
  // following expression.
  return std::min(sparse_table[level][l],
                  sparse_table[level][r - (1 << level) + 1]);
}

int OperationsInitialization(int operations_number,
                             std::string (&operations)[kMaxNumber],
                             int (&dataset)[kMaxNumber]) {
  std::string operation;
  int current = 0;
  for (int i = 0; i < operations_number; ++i) {
    std::cin >> operation;
    // Naming all operations for their first letter.
    operations[i] = operation.substr(0, 1);
    // If it is "enqueue" operation we need also to read new element.
    if (operation.substr(0, 1) == "e") {
      std::cin >> dataset[current++];
    }
  }
  return current;
}

void EnqueueOperation(int& right_pointer) {
  // Adding element to end means that right pointer increases by 1.
  right_pointer++;
  std::cout << "ok\n";
}

void DequeueOperation(int& left_pointer, int& right_pointer,
                      int (&dataset)[kMaxNumber]) {
  int queue_size = right_pointer - left_pointer + 1;
  // Deleting element from the beggining means that left pointer decreases by 1.
  if (queue_size > 0) {
    std::cout << dataset[left_pointer++] << "\n";
  } else {
    std::cout << "error\n";
  }
}

void FrontOperation(int& left_pointer, int& right_pointer,
                    int (&dataset)[kMaxNumber]) {
  int queue_size = right_pointer - left_pointer + 1;
  // Front element the one with the left pointer.
  if (queue_size > 0) {
    std::cout << dataset[left_pointer] << "\n";
  } else {
    std::cout << "error\n";
  }
}

void SizeOperation(int& left_pointer, int& right_pointer) {
  // Size of the queue is length of segment [left pointer; right pointer].
  int queue_size = right_pointer - left_pointer + 1;
  std::cout << queue_size << "\n";
}

void ClearOperation(int& left_pointer, int& right_pointer) {
  std::cout << "ok\n";
  // Clear operation means left pointer  to queue goes right after the right
  // one.
  left_pointer = right_pointer + 1;
}

void MinOperation(int& left_pointer, int& right_pointer,
                  int (&sparse_table)[kMaxLevel][kMaxNumber]) {
  int queue_size = right_pointer - left_pointer + 1;
  // Proceeding appropriate function for minimum.
  if (queue_size > 0) {
    std::cout << Minimum(left_pointer, right_pointer, sparse_table) << "\n";
  } else {
    std::cout << "error\n";
  }
}

void OperationParse(std::string operation, int& left_pointer,
                    int& right_pointer, int (&dataset)[kMaxNumber],
                    int (&sparse_table)[kMaxLevel][kMaxNumber]) {
  // Checking type of operation and proceeding appropriate function.
  if (operation == "e") {
    EnqueueOperation(right_pointer);
  }
  if (operation == "d") {
    DequeueOperation(left_pointer, right_pointer, dataset);
  }
  if (operation == "f") {
    FrontOperation(left_pointer, right_pointer, dataset);
  }
  if (operation == "s") {
    SizeOperation(left_pointer, right_pointer);
  }
  if (operation == "c") {
    ClearOperation(left_pointer, right_pointer);
  }
  if (operation == "m") {
    MinOperation(left_pointer, right_pointer, sparse_table);
  }
}

int main() {
  int operations_number;
  std::cin >> operations_number;
  int dataset[kMaxNumber];
  std::string operations[kMaxNumber];
  int number = OperationsInitialization(operations_number, operations, dataset);
  int sparse_table[kMaxLevel][kMaxNumber];
  SparseTableInitialization(number, dataset, sparse_table);
  int left_pointer = 0, right_pointer = -1;
  for (int i = 0; i < operations_number; ++i) {
    OperationParse(operations[i], left_pointer, right_pointer, dataset,
                   sparse_table);
  }
}
