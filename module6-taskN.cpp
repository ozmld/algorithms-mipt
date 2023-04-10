// Протокол передачи сообщений позволяет при передаче текста сделать не более K
// преобразований следующего вида: вставка в произвольное место произвольного
// символа, удаление из произвольного места из произвольного места и замена символа
// на произвольном месте на произвольный. Известно, что было отправлено слово α,
// а должно было прийти слово β. Найдите слово γ, пришедшее по протоколу, если
// известно, что длины γ и β совпадают, а также число возможных позиций i таких,
// что γ[i]≠β[i] минимально.Если же такой γ не существует, то дайте знать.

#include <iostream>
#include <string>
#include <vector>

int MinNumberOfIndexes(std::string s1, std::string s2, int k) {
  std::vector<std::vector<std::vector<int>>> dp(s2.size() + 1, std::vector<std::vector<int>>(k + 1, std::vector<int>(k + 1, 0)));
  // dp[i][add][[del] - number of indexes j that s2[j] == (string with len i made by s1 by add additions and del delitions)[j]
  for (uint64_t i = 1; i < s2.size() + 1; ++i) {
    for (int add = 0; add <= k; ++add) {
      for (int del = 0; del <= k; ++del) {
        if (add > 0) {
          dp[i][add][del] = std::max(dp[i][add][del], dp[i - 1][add - 1][del] + 1);
        }
        if (del > 0) {
          dp[i][add][del] = std::max(dp[i][add][del], dp[i][add][del - 1]);
        }
        if (0 < (i - add + del) && s1.size() >= (i + del - add)) {
          dp[i][add][del] = std::max(dp[i][add][del], dp[i - 1][add][del] + static_cast<int>((s1[i - add + del - 1] == s2[i - 1])));
        }
      }
    }
  }
  int ans = 0;
  for (int add = 0; add <= k; ++add) {
    for (int del = 0; del <= k; ++del) {
      if (add + del > k) {
        continue;
      }
      if (s1.size() + add != s2.size() + del) {
        continue;
      }
      ans = std::max(ans, dp[s2.size()][add][del] + k - add - del);
    }
  }
  if (std::abs(static_cast<int>(s1.size()) - static_cast<int>(s2.size())) > k) {
    return -1;
  }
  if ((static_cast<int>(s2.size()) - ans) < 0) {
    return 0;
  }
  return static_cast<int>(s2.size()) - ans;
}

int main () {
  std::string s1, s2;
  std::cin >> s1 >> s2;
  int k;
  std::cin >> k;
  std::cout <<  MinNumberOfIndexes(s1, s2, k);
}
