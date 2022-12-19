#include <iostream>
#include <list>
#include <string>
#include <vector>

class HashTable {
 public:
  explicit HashTable(int a = 77, int b = 111, int mod = 100500) {
    a_ = a;
    b_ = b;
    mod_ = mod;
    for (int _ = 0; _ < mod; ++_) {
      hash_table_.push_back({});
    }
  }
  bool Find(int element) const;
  void Insert(int element);
  void Erase(int element);

 private:
  int a_, b_, mod_;
  std::vector<std::list<int>> hash_table_;
  int Hash(int value) const { return (a_ * value + b_) % mod_; }
};

bool HashTable::Find(int element) const {
  int index = Hash(element);
  for (auto v : hash_table_[index]) {
    if (v == element) {
      return true;
    }
  }
  return false;
}

void HashTable::Insert(int element) {
  int index = Hash(element);
  if (!Find(element)) {
    hash_table_[index].push_back(element);
  }
}

void HashTable::Erase(int element) {
  int index = Hash(element);
  hash_table_[index].remove(element);
}

std::string ParseOperation(std::string operation, int element,
                           HashTable& hash_table) {
  if (operation == "+") {
    hash_table.Insert(element);
  }
  if (operation == "-") {
    hash_table.Erase(element);
  }
  if (operation == "?") {
    if (hash_table.Find(element)) {
      return "YES\n";
    }
    return "NO\n";
  }
  return "";
}

int main() {
  int n;
  std::cin >> n;
  HashTable hash_table;
  for (int i = 0; i < n; ++i) {
    std::string operation;
    std::cin >> operation;
    int element;
    std::cin >> element;
    std::cout << ParseOperation(operation, element, hash_table);
  }
}
