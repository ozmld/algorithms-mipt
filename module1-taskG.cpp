#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

const int kMaxNumber = 3 * 1e5;

double* Initialization(int number, double prefix[], double const dataset[]) {
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
  cin >> number;
  double dataset[kMaxNumber];
  for (int i = 0; i < number; ++i) {
    cin >> dataset[i];
  }
  double prefix[kMaxNumber + 1];
  double* new_prefix = Initialization(number, prefix, dataset);
  int questions_number;
  cin >> questions_number;
  int l, r;
  for (int i = 0; i < questions_number; ++i) {
    cin >> l >> r;
    cout << setprecision(16) << FindGeometricMean(l, r, new_prefix) << endl;
  }
}
