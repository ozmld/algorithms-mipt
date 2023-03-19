// Карлу необходимо выполнить для мистера Правды N поручений, каждое из них
// характеризуется двумя числами: необходимое число ресурсов m и награда c.
// Сиджею негде набирать ресурсы, так что он ограничен M единицами ресурсов.
// Какие задания он может выполнить, чтобы максимизировать награду?

#include <iostream>
#include <vector>

struct Item {
  int weight, price;
  Item(int weight, int price) : weight(weight), price(price) {}
};

std::vector<int> FindOptimalSet(int n, int m, std::vector<Item>& items) {
  std::vector<std::vector<int>> dp(
      n + 1, std::vector<int>(m + 1, 0));  // dp[i][j] - max value of items with
                                           // max capacity j if we use first i
                                           // items
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
      if (j < items[i].weight) {
        dp[i][j] = dp[i - 1][j];
        continue;
      }
      dp[i][j] = std::max(dp[i - 1][j],
                          dp[i - 1][j - items[i].weight] + items[i].price);
    }
  }
  std::vector<int> optimal_set = {};
  int i = n, j = m;
  while (dp[i][j] != 0) {
    if (dp[i][j] != dp[i - 1][j]) {
      optimal_set.push_back(i);
      j -= items[i].weight;
    }
    i--;
  }
  return optimal_set;
}
int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<Item> items = {Item(0, 0)};
  for (int i = 0; i < n; ++i) {
    int m;
    std::cin >> m;
    items.push_back(Item(m, 0));
  }
  for (int i = 0; i < n; ++i) {
    int c;
    std::cin >> c;
    items[i + 1].price = c;
  }
  std::vector<int> optimal_set = FindOptimalSet(n, m, items);
  for (size_t i = 0; i < optimal_set.size(); ++i) {
    std::cout << optimal_set[i] << "\n";
  }
}
