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
  int qq = pos[max_len];
  std::vector<int> ans;
  while (qq != -1) {
    ans.push_back(qq + 1);
    qq = prev[qq];
  }
  std::reverse(ans.begin(), ans.end());
  std::cout << max_len << "\n";
  for (int i = 0; i < max_len; ++i) {
    std::cout << ans[i] << " ";
  }
}
