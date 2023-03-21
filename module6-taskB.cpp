// Вам требуется написать программу, которая по заданной последовательности
// находит максимальную невозрастающую её подпоследовательность

#include <algorithm>
#include <iostream>
#include <vector>

const long long kInf = 1e10;

int FindMaxPos(std::vector<long long>& numbers, long long pivot) {
  int l = 0, r = numbers.size();
  while ((r - l) > 1) {
    int m = (l + r) / 2;
    if (numbers[m] < pivot) {
      r = m;
    } else {
      l = m;
    }
  }
  return r;
}

std::vector<int> FindLongestNotIncreasingSubsequence(int n,
                                                     std::vector<int> numbers) {
  std::vector<long long> dp(n + 1, -kInf), pos(n + 1, -1), prev(n, -1);
  dp[0] = kInf;
  for (int i = 0; i < n; ++i) {
    int p = FindMaxPos(dp, numbers[i]);
    dp[p] = numbers[i];
    pos[p] = i;
    prev[i] = pos[p - 1];
  }
  int max_len = 0;
  for (int i = n; i >= 0; --i) {
    if (dp[i] != -kInf) {
      max_len = i;
      break;
    }
  }
  int current_index = pos[max_len];
  std::vector<int> indexes_of_longest_subsequence;
  while (current_index != -1) {
    indexes_of_longest_subsequence.push_back(current_index + 1);
    current_index = prev[current_index];
  }
  std::reverse(indexes_of_longest_subsequence.begin(),
               indexes_of_longest_subsequence.end());
  return indexes_of_longest_subsequence;
}
int main() {
  // dp[i] - min a[k] - exists a[t1] < a[t2] < ... < a[ti] : ti = k;
  int n;
  std::cin >> n;
  std::vector<int> numbers;
  for (int i = 0; i < n; ++i) {
    int x;
    std::cin >> x;
    numbers.push_back(x);
  }
  std::vector<int> indexes_of_longest_subsequence =
      FindLongestNotIncreasingSubsequence(n, numbers);
  std::cout << indexes_of_longest_subsequence.size() << "\n";
  for (size_t i = 0; i < indexes_of_longest_subsequence.size(); ++i) {
    std::cout << indexes_of_longest_subsequence[i] << " ";
  }
}
