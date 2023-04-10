// В последних миссиях Балласы воспользовались неразберихой в Лос-Сантосе.
// Сиджей готовится к обороне своего города. Его город представляет собой
// клетчатый прямоугольник размера n×m, в котором каждая клетка — отдельный
// район. Балласы могут либо атаковать район, либо пощадить его. При этом
// есть районы, в которых достаточно оборонительных сооружений до следующего
// конца света, и балласы не в силах их захватить, а есть те, в которых
// никакой защиты нет, и им в любом случае придется капитулировать. Балласы
// все ещё справедливы, а это значит, что в любом квадрате размера 2×2 должно
// быть поровну до зубов защищенных и безоружных районов. Теперь балласы хотят
// узнать количество различных вариантов распределения районов города на
// безоружные и излишне защищенные.

#include <iostream>
#include <vector>

const long long kMod = 1e9 + 7;

bool Check(long long mask, std::vector<std::vector<long long>>& table,
           int column) {
  for (uint64_t i = 0; i < table.size(); ++i) {
    int bit = (mask >> i) % 2;
    if (bit == 1 && table[i][column] == -1) {
      return false;
    }
    if (bit == 0 && table[i][column] == 1) {
      return false;
    }
  }
  return true;
}

bool Check(long long mask1, long long mask2, int n) {
  if (mask1 == -1 || mask2 == -1) {
    return false;
  }
  for (int i = 0; i < (n - 1); ++i) {
    int bit1 = (mask1 >> i) % 2, bit2 = (mask1 >> (i + 1)) % 2,
        bit3 = (mask2 >> i) % 2, bit4 = (mask2 >> (i + 1)) % 2;
    if ((bit1 + bit2 + bit3 + bit4) != 2) {
      return false;
    }
  }
  return true;
}
long long ConstructMask(long long mask, int n, int first_bit) {
  long long prevmask = first_bit << (n - 1);
  int bit1 = (mask >> (n - 1)) % 2, bit2 = (mask >> (n - 2)) % 2;
  if ((bit1 + bit2 + first_bit) % 3 == 0) {
    return -1;
  }
  first_bit = 2 - (bit1 + bit2 + first_bit);
  prevmask += first_bit << (n - 2);
  for (int i = n - 2; i > 0; --i) {
    bit1 = (mask >> i) % 2;
    bit2 = (mask >> (i - 1)) % 2;
    if ((bit1 + bit2 + first_bit) % 3 == 0) {
      return -1;
    }
    first_bit = 2 - (bit1 + bit2 + first_bit);
    prevmask += first_bit << (i - 1);
  }
  return prevmask;
}

int Parse(char c) {
  if (c == '+') {
    return 1;
  }
  if (c == '.') {
    return 0;
  }
  if (c == '-') {
    return -1;
  }
  return 0;
}
int NumberOfDifferentInvasions(int n, int m,
                               std::vector<std::vector<long long>>& data) {
  if (n == 1) {
    long long counter = 0;
    for (int i = 0; i < m; ++i) {
      if (data[0][i] == 0) {
        ++counter;
      }
    }
    return (1 << counter) % kMod;
  }
  std::vector<std::vector<long long>> dp(m, std::vector<long long>(1 << n, 0));
  // dp[i][mask] - number of different invasions if we use first i columns
  // and mask of column i is mask. To update dp[i][mask] we construct mask of
  // previous column and check possibility of it and if mask is possible
  // we add to current number of different invasions number of invasions
  // when we use i-1 columns and mask of column i-1 is constructed mask
  for (long long mask = 0; mask < (1 << n); ++mask) {
    if (!Check(mask, data, 0)) {
      dp[0][mask] = 0;
      continue;
    }
    dp[0][mask] = 1;
  }
  for (int i = 1; i < m; ++i) {
    for (long long mask = 0; mask < (1 << n); ++mask) {
      if (!Check(mask, data, i)) {
        continue;
      }
      long long prevmask1 = ConstructMask(mask, n, 1);
      long long prevmask2 = ConstructMask(mask, n, 0);
      if (Check(mask, prevmask2, n)) {
        dp[i][mask] += dp[i - 1][prevmask2];
      }
      if (Check(mask, prevmask1, n)) {
        dp[i][mask] += dp[i - 1][prevmask1];
      }
      dp[i][mask] %= kMod;
    }
  }
  long long number_of_different_invasions = 0;
  for (long long mask = 0; mask < (1 << n); ++mask) {
    number_of_different_invasions += dp[m - 1][mask];
    number_of_different_invasions %= kMod;
  }
  return number_of_different_invasions;
}
int main() {
  long long n, m;
  std::cin >> n >> m;
  std::vector<std::vector<long long>> data(n, std::vector<long long>(m, 0));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      char c;
      std::cin >> c;
      data[i][j] = Parse(c);
    }
  }
  std::cout << NumberOfDifferentInvasions(n, m, data);
}
