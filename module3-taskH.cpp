#include <iostream>
#include <vector>

const int kMaxNumber = 1e5 + 1;
const int kMaxLevel = 17;

class SparseTable {
 private:
  long long sparse_table_[kMaxLevel][kMaxNumber][2];
  int Log(int n) const;
  std::pair<long long, long long> FindTwoMins(long long level, long long i);

 public:
  SparseTable(int number, std::vector<long long> data);
  long long GetMin(int l, int r) const;
  long long GetSecondMin(int l, int r) const;
};

int SparseTable::Log(int n) const {
  if (n == 1) {
    return 0;
  }
  return Log(n / 2) + 1;
}

void BubbleSort(std::vector<long long>& a) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (a[j] > a[j + 1]) {
        std::swap(a[j], a[j + 1]);
      }
    }
  }
}

std::pair<long long, long long> SparseTable::FindTwoMins(long long level,
                                                         long long i) {
  std::vector<long long> c = {
      sparse_table_[level - 1][i][0], sparse_table_[level - 1][i][1],
      sparse_table_[level - 1][i + (1 << (level - 1))][0],
      sparse_table_[level - 1][i + (1 << (level - 1))][1]};
  BubbleSort(c);
  return std::make_pair(c[0], c[1]);
}

SparseTable::SparseTable(int number, std::vector<long long> data) {
  // Initialization sparse table structure.
  for (int i = 0; i < number; ++i) {
    sparse_table_[0][i][0] = data[i];
    sparse_table_[0][i][1] = 1e10;
  }
  for (int i = number; i < kMaxNumber; ++i) {
    sparse_table_[0][i][0] = 1e10;
    sparse_table_[0][i][1] = 1e10;
  }
  for (int level = 1; level < kMaxLevel; ++level) {
    for (int i = 0; i < number - (1 << level) + 1; ++i) {
      std::pair<long long, long long> two_mins;
      two_mins = FindTwoMins(level, i);
      sparse_table_[level][i][0] = two_mins.first;
      sparse_table_[level][i][1] = two_mins.second;
    }
  }
}

long long SparseTable::GetMin(int l, int r) const {
  int level = Log(r - l + 1);
  // Minimum on [l, r] segment is evaluated in SparseTable structure by
  // following expression.
  return std::min(sparse_table_[level][l][0],
                  sparse_table_[level][r - (1 << level) + 1][0]);
}
long long SparseTable::GetSecondMin(int l, int r) const {
  int level = Log(r - l + 1);
  // Minimum on [l, r] segment is evaluated in SparseTable structure by
  // following expression.
  long long a1 = sparse_table_[level][l][0], b1 = sparse_table_[level][l][1];
  long long a2 = sparse_table_[level][r - (1 << level) + 1][0],
            b2 = sparse_table_[level][r - (1 << level) + 1][1];
  if (a1 != a2) {
    return a1 + a2 + std::min(b1, b2) - std::min(a1, a2) -
           std::max(std::max(a1, a2), std::min(b1, b2));
  }
  if (GetMin(r - (1 << level) + 1, l + (1 << level) - 1) == a1) {
    return std::min(b1, b2);
  }
  return a1;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<long long> data;
  for (int i = 0; i < n; ++i) {
    long long x;
    std::cin >> x;
    data.push_back(x);
  }
  SparseTable sparse_table = SparseTable(n, data);
  for (int i = 0; i < m; ++i) {
    int l, r;
    std::cin >> l >> r;
    std::cout << sparse_table.GetSecondMin(l - 1, r - 1) << "\n";
  }
}
