#include <iostream>
#include <vector>

using namespace std;

const long long kMod = 1e7 + 4321, kCoef1 = 123, kCoef2 = 45;
const long long kMaxNumber = 5e7;

// Selection sort to sort blocks
void SelectionSort(long long dataset[], long long left, int right) {
  for (int i = left; i < right; ++i) {
    for (int j = i + 1; j < right; ++j) {
      if (dataset[i] > dataset[j]) {
        swap(dataset[i], dataset[j]);
      }
    }
  }
}

long long Partition(long long dataset[], long long left_index,
                    long long right_index, long long pivot) {
  // Index for our current element
  int pointer = left_index;
  // Index for the place for last lower element
  int last_lower = left_index;
  while (pointer < right_index) {
    if (dataset[pointer] > pivot) {
      pointer++;
    } else {
      // If element lower than pivot than we should to place it to
      // last lower element's index
      swap(dataset[pointer], dataset[last_lower]);
      pointer++;
      last_lower++;
    }
  }
  return last_lower - left_index;
}

long long KthOrderStatistic(long long dataset[], long long k, int l, int r) {
  // If size is too small we'll just sort in with any sort method
  if (r - l <= 5) {
    SelectionSort(dataset, l, r);
    return dataset[l + k - 1];
  }
  // Sorting every block of 5 elements by any sort method
  for (int i = l; i + 5 <= r; i += 5) {
    SelectionSort(dataset, i, i + 5);
    // Adding medians to the end of the array to then find median of medians
    dataset[r + (i - l) / 5] = dataset[i + 2];
  }
  int blocks_number = (r - l) / 5;
  // Evaluating "median of medians"
  int pivot =
      KthOrderStatistic(dataset, blocks_number / 2, r, r + blocks_number);
  // Evaluating number of element <=pivot and <pivot
  int not_higher_pivot = Partition(dataset, l, r, pivot);
  int less_pivot = Partition(dataset, l, l + not_higher_pivot, pivot - 1);
  // Recursively runs function from smaller dataset
  if (k > not_higher_pivot) {
    return KthOrderStatistic(dataset, k - not_higher_pivot,
                             l + not_higher_pivot, r);
  }
  if (k <= less_pivot) {
    return KthOrderStatistic(dataset, k, l, l + less_pivot);
  }
  return pivot;
}

void InitializeDataset(long long dataset[], long long n, long long a0,
                       long long a1) {
  dataset[0] = a0;
  dataset[1] = a1;
  // Initializing the dataset by rule from the task
  for (int i = 2; i < n; ++i) {
    dataset[i] = (dataset[i - 1] * kCoef1 + dataset[i - 2] * kCoef2) % kMod;
  }
}

int main() {
  long long n, k, a0, a1;
  cin >> n >> k >> a0 >> a1;
  long long dataset[kMaxNumber];
  InitializeDataset(dataset, n, a0, a1);
  cout << KthOrderStatistic(dataset, k, 0, n) << "\n";
}
