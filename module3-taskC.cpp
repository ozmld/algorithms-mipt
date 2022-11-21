#include <iostream>

const int kSize = 2e5 + 1;

class SegmentTree {
 private:
  int tree_[4 * kSize], data_size_;

 public:
  SegmentTree(int data_size) {
    data_size_ = data_size;
    for (int i = 0; i < kSize; ++i) {
      tree_[i] = -1;
    }
  }
  void Build(int data[], int v, int l, int r);
  int FindRightNotLess(int v, int l, int r, int pos, int val);
  void Update(int v, int l, int r, int pos, int val);
};

void SegmentTree::Build(int data[], int v, int l, int r) {
  if (r - l == 1) {
    tree_[v] = data[l];
    return;
  }
  int m = (l + r) / 2;
  Build(data, 2 * v + 1, l, m);
  Build(data, 2 * v + 2, m, r);
  tree_[v] = std::max(tree_[2 * v + 1], tree_[2 * v + 2]);
}

void SegmentTree::Update(int v, int l, int r, int pos, int val) {
  if (r - l == 1) {
    tree_[v] = val;
    return;
  }
  int m = (l + r) / 2;
  if (pos < m) {
    Update(2 * v + 1, l, m, pos, val);
  } else {
    Update(2 * v + 2, m, r, pos, val);
  }
  tree_[v] = std::max(tree_[2 * v + 1], tree_[2 * v + 2]);
}

int SegmentTree::FindRightNotLess(int v, int l, int r, int pos, int val) {
  if (tree_[v] < val) {
    return -1;
  }
  if (r - l == 1) {
    return r;
  }
  int m = (l + r) / 2, ans_left = kSize + 1, ans_right = kSize + 1;
  if (pos >= m) {
    return FindRightNotLess(2 * v + 2, m, r, pos, val);
  }
  ans_left = FindRightNotLess(2 * v + 1, l, m, pos, val);
  if (ans_left == -1) {
    ans_right = FindRightNotLess(2 * v + 2, m, r, pos, val);
    if (ans_right == -1) {
      return -1;
    }
    return ans_right;
  }
  return ans_left;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(NULL);
  std::cout.tie(NULL);
  int n, m;
  std::cin >> n >> m;
  int data[kSize];
  for (int i = 0; i < n; ++i) {
    std::cin >> data[i];
  }
  SegmentTree tree = SegmentTree(n);
  tree.Build(data, 0, 0, n);
  for (int _ = 0; _ < m; ++_) {
    int operation_type, i, x;
    std::cin >> operation_type >> i >> x;
    if (operation_type == 0) {
      tree.Update(0, 0, n, i - 1, x);
    } else {
      std::cout << tree.FindRightNotLess(0, 0, n, i - 1, x) << "\n";
    }
  }
}
