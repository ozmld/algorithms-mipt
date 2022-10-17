/*
Формат ввода
В первой строке дано число N (1 ≤ N ≤ 3 * 10^5) — число мандрагор.
На второй строке идут N вещественных чисел (0.01 ≤ ai ≤ 103) с двумя
знаками после десятичной точки — громкость i-й мандрагоры.

На третьей строке идет единственное число Q (1 ≤ Q ≤ 105) — число
запросов подсчета среднего шума на подотрезке от Гермионы.

Далее идет Q строк запросов в формате «i j» (0 ≤ i ≤ j ≤ N - 1):
i, j — индексы массива, задающие отрезок для запроса.

Формат вывода
Для каждого запроса на новой строке выведите полученную среднюю
громкость (среднее геометрическое) результат с точностью до не менее
шестого знака после запятой.
*/
#include <cmath>
#include <iomanip>
#include <iostream>

const int kMaxNumber = 3 * 1e5;

double* PrefixLogSumInitialization(int number, double prefix[],
                                   double const dataset[]) {
  // Initialization array of prefix sum of logarithms.
  prefix[0] = 0.;
  for (int i = 0; i < number; ++i) {
    prefix[i + 1] = prefix[i] + log2(dataset[i]);
  }
  return prefix;
}

double FindGeometricMean(int l, int r, double const prefix[]) {
  // Raise 2 to the power of sum of logarithms on [l, r] divided by
  // length of [l,r] to evaluate geometric mean.
  return pow(2, (prefix[r + 1] - prefix[l]) / (r - l + 1));
}

int main() {
  int number;
  std::cin >> number;
  double dataset[kMaxNumber];
  for (int i = 0; i < number; ++i) {
    std::cin >> dataset[i];
  }
  double prefix[kMaxNumber + 1];
  double* new_prefix = PrefixLogSumInitialization(number, prefix, dataset);
  int questions_number;
  std::cin >> questions_number;
  int r, l;
  for (int j = 0; j < questions_number; ++j) {
    std::cin >> l >> r;
    std::cout << std::setprecision(16) << FindGeometricMean(l, r, new_prefix)
              << "\n";
  }
}
