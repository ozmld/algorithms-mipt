#include <iostream>

using namespace std;

const int kMaxNumber = 5e5 + 1;

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
      swap(dataset[pointer++], dataset[last_lower++]);
    }
  }
  return last_lower - left_index;
}

int KthOrderStatistic(long long dataset[], long long k, int l, int r) {
  // If size is too small we'll just sort in with any sort method
  if (r - l <= 5) {
    SelectionSort(dataset, l, r);
    return dataset[l + k];
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
  if (k >= not_higher_pivot) {
    return KthOrderStatistic(dataset, k - not_higher_pivot,
                             l + not_higher_pivot, r);
  }
  if (k < less_pivot) {
    return KthOrderStatistic(dataset, k, l, l + less_pivot);
  }
  return pivot;
}

void QuickSort(long long dataset[], int l, int r) {
  if (r - l <= 5) {
    SelectionSort(dataset, l, r);
  } else {
    for (int i = l; i + 5 <= r; i += 5) {
      // Sorting block of 5
      SelectionSort(dataset, i, i + 5);
      // Adding medians to the end of the array to then find median of medians
      dataset[(i - l) / 5 + 4 * kMaxNumber / 5] = dataset[i + 2];
    }
    int blocks_number = (r - l) / 5;
    // Evaluating "median of medians"
    int pivot =
        KthOrderStatistic(dataset, blocks_number / 2, 4 * kMaxNumber / 5,
                          4 * kMaxNumber / 5 + blocks_number);
    // Evaluating number of element <=pivot and <pivot
    int not_higher_pivot = Partition(dataset, l, r, pivot);
    int less_pivot = Partition(dataset, l, l + not_higher_pivot, pivot - 1);
    // Sorting left part (<pivot) and right part (>pivot)
    QuickSort(dataset, l + not_higher_pivot, r);
    QuickSort(dataset, l, l + less_pivot);
  }
}

int main() {
  int n;
  cin >> n;
  long long dataset[kMaxNumber];
  for (int i = 0; i < n; ++i) {
    cin >> dataset[i];
  }
  QuickSort(dataset, 0, n);
  for (int i = 0; i < n; ++i) {
    cout << dataset[i] << "\n";
  }
}
