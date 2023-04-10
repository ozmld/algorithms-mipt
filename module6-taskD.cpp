// Вам предстоит узнать, падение с какой высоты самолётик переживет.
// Известно, что если его уронить с высоты >=x от 1 до n метров,
// то он разобьется, что вызовет перезапуск миссии и ощущение всемирной
// несправедливости у играющего. Задача осложняется тем, что вам
// предоставлено всего k образцов самолетика. Каждый самолетик
// можно бросать сколько угодно раз, пока он не сломается; после этого
// использовать его для экспериментов больше не удастся.

#include <iostream>
#include <vector>

int CountMinNumberOfExperements(int n, int k) {
  if (n == 1) {
    return 0;
  }
  if (k == 0) {
    return -1;
  }
  std::vector<std::vector<int>> dp(
      k + 1, {0});  // dp[i][j]- max height i-planes j-attempts
  for (int j = 1; j <= n; ++j) {
    dp[0].push_back(0);
    for (int i = 1; i <= k; ++i) {
      dp[i].push_back(dp[i - 1][j - 1] + dp[i][j - 1] +
                      1);  // plane saved or not saved
    }
    if (dp[k][j] >= (n - 1)) {
      return j;
      break;
    }
  }
  return 0;
}

int main() {
  int n, k;
  std::cin >> n >> k;
  std::cout << CountMinNumberOfExperements(n, k);
}
