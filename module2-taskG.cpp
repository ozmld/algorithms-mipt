#include <iostream>
#include <vector>

using namespace std;

void CountingSort(vector<long long>& dataset, int dataset_size, int byte) {
  int cnt[2] = {0, 0};
  for (int i = 0; i < dataset_size; ++i) {
    ++cnt[(dataset[i] >> byte) & 1];
  }
  cnt[1] += cnt[0];
  vector<long long> answer(dataset_size, 0);
  for (int i = dataset_size - 1; i >= 0; --i) {
    answer[--cnt[(dataset[i] >> byte) & 1]] = dataset[i];
  }
  for (int i = 0; i < dataset_size; ++i) {
    dataset[i] = answer[i];
  }
}

void LSDSort(vector<long long>& dataset, int dataset_size) {
  // Doing Counting sort for each byte of number
  for (int byte = 0; byte < 64; ++byte) {
    CountingSort(dataset, dataset_size, byte);
  }
}

void InitializeDataset(vector<long long>& dataset, long long dataset_size) {
  long long element;
  for (int i = 0; i < dataset_size; ++i) {
    cin >> element;
    dataset.push_back(element);
  }
}

void PrintAnswer(vector<long long>& dataset, long long dataset_size) {
  for (int i = 0; i < dataset_size; ++i) {
    cout << dataset[i] << "\n";
  }
}
int main() {
  int dataset_size;
  cin >> dataset_size;
  vector<long long> dataset = {};
  InitializeDataset(dataset, dataset_size);
  LSDSort(dataset, dataset_size);
  PrintAnswer(dataset, dataset_size);
}
