#include <deque>
#include <iostream>

using namespace std;

void DeleteGoblin(deque<long long>& goblins_left_half) {
  cout << goblins_left_half.front() << endl;
  goblins_left_half.pop_front();
}

void AddCommonGoblin(deque<long long>& goblins_right_half, int goblin_index) {
  // Common goblin comes to end of the queue so we are pushing him to the end of
  // right half.
  goblins_right_half.push_back(goblin_index);
}

void AddPriorityGoblin(deque<long long>& goblins_right_half, int goblin_index) {
  // Priority goblin comes right after the middle of the queue so we are pushing
  // him to the beginning of right half.
  goblins_right_half.push_front(goblin_index);
}

void UpdateHalfs(deque<long long>& goblins_left_half,
                 deque<long long>& goblins_right_half) {
  // After Add-Delete operations we should update splitting queue into halves.
  // Size of the left half should be larger than size of the right one.
  if (goblins_left_half.size() < goblins_right_half.size()) {
    goblins_left_half.push_back(goblins_right_half.front());
    goblins_right_half.pop_front();
  }
}

int main() {
  int operations_number;
  cin >> operations_number;
  // Creating 2 stacks for left ad right halves of goblins queue.
  deque<long long> goblins_left_half, goblins_right_half;
  char operation;
  int goblin_index;
  for (long long i = 0; i < operations_number; ++i) {
    cin >> operation;
    if (operation == '-') {
      DeleteGoblin(goblins_left_half);
    } else {
      cin >> goblin_index;
      if (operation == '+') {
        AddCommonGoblin(goblins_right_half, goblin_index);
      } else {
        AddPriorityGoblin(goblins_right_half, goblin_index);
      }
    }
    UpdateHalfs(goblins_left_half, goblins_right_half);
  }
}
